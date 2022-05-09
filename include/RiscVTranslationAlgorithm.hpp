#pragma once

#include <RiscV.hpp>
#include <Swizzle.hpp>
#include <Translation.hpp>
#include <Transactor.hpp>
#include <HartState.hpp>


template<typename XLEN_t, IOVerb verb>
inline Translation<XLEN_t> PageFault(XLEN_t virt_addr) {
    if constexpr (verb == IOVerb::Read) {
        return { virt_addr, 0, 0, RISCV::TrapCause::LOAD_PAGE_FAULT };
    } else if constexpr (verb == IOVerb::Write) {
        return { virt_addr, 0, 0, RISCV::TrapCause::STORE_AMO_PAGE_FAULT };
    } else {
        return { virt_addr, 0, 0, RISCV::TrapCause::INSTRUCTION_PAGE_FAULT };
    }
}

template<typename XLEN_t, IOVerb verb>
static inline Translation<XLEN_t> TranslationAlgorithm(
        XLEN_t virt_addr,
        Transactor<XLEN_t>* transactor,
        XLEN_t root_ppn,
        RISCV::PagingMode currentPagingMode,
        RISCV::PrivilegeMode translationPrivilege,
        bool mxrBit,
        bool sumBit
    ) {
    
    unsigned int i = 0;
    XLEN_t vpn[4];
    XLEN_t ptesize = 0;
    XLEN_t pagesize = 0;

    if (translationPrivilege == RISCV::PrivilegeMode::Machine ||
        currentPagingMode == RISCV::PagingMode::Bare) {
        return { virt_addr, virt_addr, (XLEN_t)~0, RISCV::TrapCause::NONE };
    } else if (currentPagingMode == RISCV::PagingMode::Sv32) {
        i = 1;
        vpn[1] = swizzle<XLEN_t, ExtendBits::Zero, 31, 22>(virt_addr);
        vpn[0] = swizzle<XLEN_t, ExtendBits::Zero, 21, 12>(virt_addr);
        ptesize = 4;
        pagesize = 1 << 12;
    } else if (currentPagingMode == RISCV::PagingMode::Sv39) {
        i = 2;
        vpn[2] = swizzle<XLEN_t, ExtendBits::Zero, 38, 30>(virt_addr);
        vpn[1] = swizzle<XLEN_t, ExtendBits::Zero, 29, 21>(virt_addr);
        vpn[0] = swizzle<XLEN_t, ExtendBits::Zero, 20, 12>(virt_addr);
        ptesize = 8;
        pagesize = 1 << 12;
    } else if (currentPagingMode == RISCV::PagingMode::Sv48) {
        i = 3;
        vpn[3] = swizzle<XLEN_t, ExtendBits::Zero, 47, 39>(virt_addr);
        vpn[2] = swizzle<XLEN_t, ExtendBits::Zero, 38, 30>(virt_addr);
        vpn[1] = swizzle<XLEN_t, ExtendBits::Zero, 29, 21>(virt_addr);
        vpn[0] = swizzle<XLEN_t, ExtendBits::Zero, 20, 12>(virt_addr);
        ptesize = 8;
        pagesize = 1 << 12;
    } // TODO wider paging modes

    // 1. Let a be satp.ppn × PAGESIZE, and let i = LEVELS − 1. (For Sv32,
    //    PAGESIZE=2**12 and LEVELS=2.)

    XLEN_t a = root_ppn * pagesize;
    XLEN_t pte = 0; // TODO PTE should be Sv** determined, not XLEN_t sized...

    while (true) {

        // 2. Let pte be the value of the PTE at address a+va.vpn[i]×PTESIZE.
        //    (For Sv32, PTESIZE=4.) If accessing pte violates a PMA or PMP
        //    check, raise an access exception corresponding to the original
        //    access type.

        XLEN_t pteaddr = a + (vpn[i] * ptesize);
        transactor->Read(pteaddr, ptesize, (char*)&pte);

        // TODO PMA & PMP checks

        // 3. If pte.v = 0, or if pte.r = 0 and pte.w = 1, stop and raise a
        //    page-fault exception corresponding to the original access
        //    type.

        if ( !(pte & RISCV::PTEBit::V) ||
            (!(pte & RISCV::PTEBit::R) && (pte & RISCV::PTEBit::W))) {
            return PageFault<XLEN_t, verb>(virt_addr);
        }

        // 4. Otherwise, the PTE is valid. If pte.r = 1 or pte.x = 1, go to
        //    step 5. Otherwise, this PTE is a pointer to the next level of
        //    the page table. Let i = i−1. If i < 0, stop and raise a
        //    page-fault exception corresponding to the original access
        //    type. Otherwise, let a = pte.ppn×PAGESIZE and go to step 2.
        
        if ((pte & RISCV::PTEBit::R) || (pte & RISCV::PTEBit::X)) {
            break;
        }

        if (i == 0) {
            return PageFault<XLEN_t, verb>(virt_addr);
        }
        i = i-1;

        if (currentPagingMode == RISCV::PagingMode::Sv39 ||
            currentPagingMode == RISCV::PagingMode::Sv48) {
            a = swizzle<XLEN_t, ExtendBits::Zero, 53, 10>(pte) * pagesize;
        } else {
            a = swizzle<XLEN_t, ExtendBits::Zero, 31, 10>(pte) * pagesize;
        }

    }

    // 5. A leaf PTE has been found. Determine if the requested memory
    //    access is allowed by the pte.r, pte.w, pte.x, and pte.u bits,
    //    given the current privilege mode and the value of the SUM and MXR
    //    fields of the mstatus register. If not, stop and raise a
    //    page-fault exception corresponding to the original access type.
    //
    // Spec says:
    //
    // Attempting to fetch an instruction from a page that does not have
    // execute permissions raises a fetch page-fault exception. Attempting 
    // to execute a load or load-reserved instruction whose effective 
    // address lies within a page without read permissions raises a load 
    // page-fault exception. Attempting to execute a store, 
    // store-conditional (regardless of success), or AMO instruction whose 
    // effective address lies within a page without write permissions raises
    // a store page-fault exception.
    //
    // The MXR (Make eXecutable Readable) bit modifies the privilege with
    // which loads access virtual memory. When MXR=0, only loads from pages
    // marked readable (R=1 in Figure 4.15) will succeed. When MXR=1, loads
    // from pages marked either readable or executable (R=1 or X=1) will
    // succeed. MXR has no effect when page-based virtual memory is not in
    // effect.

    // TODO this could be constexpr if not for pedantry wrt mxrBit
    if (verb == IOVerb::Read) {
        if (!(pte & RISCV::PTEBit::R) &&
            !((pte & RISCV::PTEBit::X) && mxrBit)) {
            return PageFault<XLEN_t, verb>(virt_addr);
        }
    } else if (verb == IOVerb::Write) {
        if (!(pte & RISCV::PTEBit::W)) {
            return PageFault<XLEN_t, verb>(virt_addr);
        }
    } else {
        if (!(pte & RISCV::PTEBit::X)) {
            return PageFault<XLEN_t, verb>(virt_addr);
        }
    }
    
    // The U bit indicates whether the page is accessible to user mode.
    // U-mode software may only access the page when U=1. If the SUM bit in
    // the sstatus register is set, supervisor mode software may also access
    // pages with U=1. However, supervisor code normally operates with the
    // SUM bit clear, in which case, supervisor code will fault on accesses
    // to user-mode pages. Irrespective of SUM, the supervisor may not
    // execute code on pages with U=1.

    if (translationPrivilege == RISCV::PrivilegeMode::User &&
        !(pte & RISCV::PTEBit::U)) {
        return PageFault<XLEN_t, verb>(virt_addr);
    }

    // The SUM (permit Supervisor User Memory access) bit modifies the
    // privilege with which S-mode loads and stores access virtual memory.
    // When SUM=0, S-mode memory accesses to pages that are accessible by
    // U-mode (U=1 in Figure 4.15) will fault. When SUM=1, these accesses
    // are permitted. SUM has no effect when page-based virtual memory is
    // not in effect, nor when executing in U-mode. Note that S-mode can
    // never execute instructions from user pages, regardless of the state
    // of SUM.

    if (translationPrivilege == RISCV::PrivilegeMode::Supervisor &&
        (pte & RISCV::PTEBit::U) &&
        !sumBit) {
        return PageFault<XLEN_t, verb>(virt_addr);
    }

    XLEN_t ppn[4];
    if (currentPagingMode == RISCV::PagingMode::Sv32) {
        ppn[1] = swizzle<XLEN_t, ExtendBits::Zero, 31, 20>(pte);
        ppn[0] = swizzle<XLEN_t, ExtendBits::Zero, 19, 10>(pte);
    } else if (currentPagingMode == RISCV::PagingMode::Sv39) {
        ppn[2] = swizzle<XLEN_t, ExtendBits::Zero, 53, 28>(pte);
        ppn[1] = swizzle<XLEN_t, ExtendBits::Zero, 27, 19>(pte);
        ppn[0] = swizzle<XLEN_t, ExtendBits::Zero, 18, 10>(pte);
    } else if (currentPagingMode == RISCV::PagingMode::Sv48) {
        ppn[3] = swizzle<XLEN_t, ExtendBits::Zero, 53, 37>(pte);
        ppn[2] = swizzle<XLEN_t, ExtendBits::Zero, 36, 28>(pte);
        ppn[1] = swizzle<XLEN_t, ExtendBits::Zero, 27, 19>(pte);
        ppn[0] = swizzle<XLEN_t, ExtendBits::Zero, 18, 10>(pte);
    } // TODO all Sv** modes

    // 6. If i > 0 and pte.ppn[i − 1 : 0] != 0, this is a misaligned
    //    superpage; stop and raise a page-fault exception corresponding to
    //    the original access type.

    if (i > 0) {
        for (unsigned int lv = 0; lv < i; lv++) {
            if (ppn[lv] != 0) {
                return PageFault<XLEN_t, verb>(virt_addr);
            }
        }
    }

    // 7. If pte.a = 0, or if the memory access is a store and pte.d = 0,
    //    either raise a page-fault exception corresponding to the original
    //    access type, or:
    //    * Set pte.a to 1 and, if the memory access is a store, also set
    //      pte.d to 1.
    //    * If this access violates a PMA or PMP check, raise an access
    //      exception corresponding to the original access type.
    //    * This update and the loading of pte in step 2 must be atomic; in
    //      particular, no intervening store to the PTE may be perceived to
    //      have occurred in-between.

    // TODO the alternative A/D bit scheme mentioned in the spec.

    if (!(pte & RISCV::PTEBit::A)) {
        return PageFault<XLEN_t, verb>(virt_addr);
    }

    if constexpr (verb == IOVerb::Write) {
        if (!(pte & RISCV::PTEBit::D)) {
            return PageFault<XLEN_t, verb>(virt_addr);
        }
    }

    // 8. The translation is successful. The translated physical address is
    //    given as follows:
    //    * pa.pgoff = va.pgoff.
    //    * If i > 0, then this is a superpage translation and
    //      pa.ppn[i − 1 : 0] = va.vpn[i − 1 : 0].
    //    * pa.ppn[LEVELS − 1 : i] = pte.ppn[LEVELS − 1 : i].

    // TODO this is where we can determine a better validThrough.
    // Determine how big the page is given how super this page is :P

    for (; i > 0; i--) {
        ppn[i-1] = vpn[i-1];
    }

    XLEN_t page_offset = swizzle<XLEN_t, ExtendBits::Zero, 11, 0>(virt_addr);
    XLEN_t phys_addr = 0;
    if (currentPagingMode == RISCV::PagingMode::Sv32) {
        phys_addr = (ppn[1] << 22) | (ppn[0] << 12) | page_offset;
    } else if (currentPagingMode == RISCV::PagingMode::Sv39) {
        phys_addr = (ppn[2] << 30) | (ppn[1] << 21) | (ppn[0] << 12) | page_offset;
    }/* else if (currentPagingMode == RISCV::PagingMode::Sv48) {
        phys_addr = (ppn[3] << 39) | (ppn[2] << 30) | (ppn[1] << 21) | (ppn[0] << 12) | page_offset;
    }
    */ // TODO we're temporarily breaking Sv48 because of this shift overflow warning-as-error. There is a clean way...

    // XLEN_t virt_page_start = virt_addr & ~(pagesize - 1);
    XLEN_t virt_valid_through = virt_addr | (pagesize - 1);
    // TODO this is inefficient for superpages because it only uses the size of regular pages.

    return { virt_addr, phys_addr, virt_valid_through, RISCV::TrapCause::NONE };

}
