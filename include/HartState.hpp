#pragma once

#include <cstdint>
#include <functional>

#include <RiscV.hpp>
#include <Operands.hpp>
#include <DecodedInstruction.hpp>
#include <Swizzle.hpp>


enum class HartCallbackArgument {
    ChangedPrivilege,
    ChangedMISA,
    ChangedMSTATUS,
    ChangedSATP,
    RequestedIfence,
    RequestedVMfence
};

template<typename XLEN_t>
class HartState {

public:

    struct Fetch {
        __uint32_t encoding;
        DecodedInstruction<XLEN_t> instruction;
        Operands operands;
        XLEN_t virtualPC;
    };

public:

    // TODO, bug, MIP MIE MIDELEG MEDELEG are MXLEN bits wide, not XLEN... etc.

    const __uint32_t maximalExtensions;
    Fetch *currentFetch = nullptr;
    XLEN_t nextFetchVirtualPC;

    XLEN_t regs[RISCV::NumRegs];
    RISCV::PrivilegeMode privilegeMode = RISCV::PrivilegeMode::Machine;
    RISCV::misaReg misa;
    RISCV::mstatusReg mstatus;
    RISCV::interruptReg mie, mip;
    RISCV::causeReg<XLEN_t> mcause, scause, ucause;
    RISCV::tvecReg<XLEN_t> mtvec, stvec, utvec;
    XLEN_t mepc, sepc, uepc;
    XLEN_t mtval, stval, utval;
    XLEN_t mscratch, sscratch, uscratch;
    XLEN_t mideleg, medeleg, sideleg, sedeleg;

    RISCV::satpReg<XLEN_t> satp;
    RISCV::fcsrReg fcsr;
    // TODO when enabling these, move them out!
    // __uint64_t counters[32];
    // __uint32_t mcounteren;
    // __uint32_t scounteren;
    // __uint32_t mcountinhibit;
    // XLEN_t hpmevents[32];
    // RISCV::pmpEntry pmpentry[16];

    // TODO should this just be something the ex_ functions know about?
    // that's lots of argument-copying.... but then, bundle + ptr chase them?
    // but, not a lot of opportunity to "know" what changes outside this object
    std::function<void(HartCallbackArgument)> implCallback;
    void emptyCallback(HartCallbackArgument arg) { return; }

    HartState(__uint32_t allSupportedExtensions)
        : maximalExtensions(allSupportedExtensions) {
        implCallback = std::bind(&HartState::emptyCallback, this, std::placeholders::_1);
        privilegeMode = RISCV::PrivilegeMode::Machine;
    }

    void Reset(XLEN_t resetVector) {
        privilegeMode = RISCV::PrivilegeMode::Machine;
        mstatus.mie = false;
        mstatus.mprv = false;
        misa.extensions = maximalExtensions;
        mcause.exceptionCode = RISCV::TrapCause::NONE;
        mcause.interrupt = false;
        nextFetchVirtualPC = resetVector;
        misa.mxlen = RISCV::xlenTypeToMode<XLEN_t>();
        // TODO much much more thorough reset behaviors
    }

    template<bool Writing>
    inline void MISA(XLEN_t* value) {
        if constexpr (Writing) {
            unsigned int shift = (sizeof(XLEN_t)*8)-2;
            misa.extensions = *value & 0x3ffffff & maximalExtensions;
            misa.mxlen = (RISCV::XlenMode)(*value >> shift);
            implCallback(HartCallbackArgument::ChangedMISA);
        } else {
            unsigned int shift = (sizeof(XLEN_t)*8)-2;
            *value = (((XLEN_t)misa.mxlen) << shift) | misa.extensions;
        }
    }

    template<bool Writing>
    inline void SATP(XLEN_t* value) {
        if constexpr (Writing) {
            satp.pagingMode = (RISCV::PagingMode)swizzle<XLEN_t, ExtendBits::Zero, 31, 31>(*value);
            satp.asid = swizzle<XLEN_t, ExtendBits::Zero, 30, 22>(*value);
            satp.ppn = swizzle<XLEN_t, ExtendBits::Zero, 21, 0>(*value);
            if constexpr (!std::is_same<XLEN_t, __uint32_t>()) {
                satp.pagingMode = (RISCV::PagingMode)swizzle<XLEN_t, ExtendBits::Zero, 63, 60>(*value);
                satp.asid = swizzle<XLEN_t, ExtendBits::Zero, 59, 44>(*value);
                satp.ppn = swizzle<XLEN_t, ExtendBits::Zero, 43, 0>(*value);
            }
            implCallback(HartCallbackArgument::ChangedSATP);
        } else {
            if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                *value = ((satp.pagingMode & 0x000001) << 31)|
                              (( satp.asid & 0x0001ff) << 22)|
                              ((  satp.ppn & 0x3fffff) << 00);
            } else {
                *value = (((__uint64_t)satp.pagingMode & 0x0000000000f) << 60)|
                               (((__uint64_t)satp.asid & 0x0000000ffff) << 44)|
                                (((__uint64_t)satp.ppn & 0xfffffffffff) << 00);
            }
        }
    }

    template<RISCV::PrivilegeMode viewPrivilege, bool Writing>
    inline void XSTATUS(XLEN_t *value) {
        if constexpr (Writing) {
            mstatus.uie = RISCV::uieMask & *value;
            mstatus.upie = RISCV::upieMask & *value;
            if constexpr (viewPrivilege != RISCV::PrivilegeMode::User) {
                mstatus.sie = RISCV::sieMask & *value;
                mstatus.spie = RISCV::spieMask & *value;
                mstatus.spp = (RISCV::PrivilegeMode)((RISCV::sppMask & *value) >> RISCV::sppShift);
                mstatus.fs = (RISCV::FloatingPointState)((RISCV::fsMask & *value) >> RISCV::fsShift);
                mstatus.xs = (RISCV::ExtensionState)((RISCV::xsMask & *value) >> RISCV::xsShift);
                mstatus.sum = RISCV::sumMask & *value;
                mstatus.mxr = RISCV::mxrMask & *value;
                if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                    mstatus.sd = *value & (1 << 31);
                } else {
                    mstatus.sd = *value & ((XLEN_t)1 << 63);
                }
            }
            if constexpr (viewPrivilege == RISCV::PrivilegeMode::Machine) {
                mstatus.mie = RISCV::mieMask & *value;
                mstatus.mpie = RISCV::mpieMask & *value;
                mstatus.mpp = (RISCV::PrivilegeMode)((RISCV::mppMask & *value) >> RISCV::mppShift);
                if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                    mstatus.uxl = RISCV::XlenMode::XL32;
                    mstatus.sxl = RISCV::XlenMode::XL32;
                } else {
                    mstatus.uxl = (RISCV::XlenMode)((RISCV::uxlMask & *value) >> RISCV::uxlShift);
                    mstatus.sxl = (RISCV::XlenMode)((RISCV::sxlMask & *value) >> RISCV::sxlShift);
                }
                mstatus.mprv = RISCV::mprvMask & *value;
                mstatus.tvm = RISCV::tvmMask & *value;
                mstatus.tw = RISCV::twMask & *value;
                mstatus.tsr = RISCV::tsrMask & *value;
            }
            implCallback(HartCallbackArgument::ChangedMSTATUS);
        } else {
            *value = 0;
            *value |= mstatus.uie ? RISCV::uieMask : 0;
            *value |= mstatus.upie ? RISCV::upieMask : 0;
            if constexpr (viewPrivilege != RISCV::PrivilegeMode::User) {
                *value |= mstatus.sie ? RISCV::sieMask : 0;
                *value |= mstatus.spie ? RISCV::spieMask : 0;
                *value |= mstatus.spp << RISCV::sppShift;
                *value |= mstatus.fs << RISCV::fsShift;
                *value |= mstatus.xs << RISCV::xsShift;
                *value |= mstatus.sum ? RISCV::sumMask : 0;
                *value |= mstatus.mxr ? RISCV::mxrMask : 0;
                if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                    *value |= mstatus.sd ? (1 << 31) : 0;
                } else {
                    *value |= mstatus.sd ? ((XLEN_t)1 << 63) : 0;
                }
            }
            if constexpr (viewPrivilege == RISCV::PrivilegeMode::Machine) {
                *value |= mstatus.mie ? RISCV::mieMask : 0;
                *value |= mstatus.mpie ? RISCV::mpieMask : 0;
                *value |= mstatus.mpp << RISCV::mppShift;
                if constexpr (!std::is_same<XLEN_t, __uint32_t>()) {
                    *value |= (__uint64_t)(mstatus.uxl) << RISCV::uxlShift;
                    *value |= (__uint64_t)(mstatus.sxl) << RISCV::sxlShift;
                }
                *value |= mstatus.mprv ? RISCV::mprvMask : 0;
                *value |= mstatus.tvm ? RISCV::tvmMask : 0;
                *value |= mstatus.tw ? RISCV::twMask : 0;
                *value |= mstatus.tsr ? RISCV::tsrMask : 0;
            }
        }
    }

    template<bool YisE, RISCV::PrivilegeMode viewPrivilege, bool Writing>
    inline void XIY(XLEN_t *value) {
        RISCV::interruptReg* miy = &mip;
        if constexpr (YisE) {
            miy = &mie;
        }
        if constexpr (Writing) {
            miy->usi = RISCV::usiMask & *value;
            miy->uti = RISCV::utiMask & *value;
            miy->uei = RISCV::ueiMask & *value;
            if constexpr (viewPrivilege != RISCV::PrivilegeMode::User) {
                miy->ssi = RISCV::ssiMask & *value;
                miy->sti = RISCV::stiMask & *value;
                miy->sei = RISCV::seiMask & *value;
            }
            if constexpr (viewPrivilege == RISCV::PrivilegeMode::Machine) {
                miy->msi = RISCV::msiMask & *value;
                miy->mti = RISCV::mtiMask & *value;
                miy->mei = RISCV::meiMask & *value;
            }
        } else {
            *value = 0;
            *value |= miy->usi ? RISCV::usiMask : 0;
            *value |= miy->uti ? RISCV::utiMask : 0;
            *value |= miy->uei ? RISCV::ueiMask : 0;
            if constexpr (viewPrivilege != RISCV::PrivilegeMode::User) {
                *value |= miy->ssi ? RISCV::ssiMask : 0;
                *value |= miy->sti ? RISCV::stiMask : 0;
                *value |= miy->sei ? RISCV::seiMask : 0;
            }
            if constexpr (viewPrivilege == RISCV::PrivilegeMode::Machine) {
                *value |= miy->msi ? RISCV::msiMask : 0;
                *value |= miy->mti ? RISCV::mtiMask : 0;
                *value |= miy->mei ? RISCV::meiMask : 0;
            }
        }
    }

    template<RISCV::PrivilegeMode viewPrivilege, bool Writing>
    inline void XTVEC(XLEN_t* value) {

        RISCV::tvecReg<XLEN_t>* tvec = &mtvec;
        if constexpr (viewPrivilege == RISCV::PrivilegeMode::Supervisor) {
            tvec = &stvec;
        } else if constexpr (viewPrivilege == RISCV::PrivilegeMode::User) {
            tvec = &utvec;
        }

        if constexpr (Writing) {
            tvec->base = *value & RISCV::tvecBaseMask;
            tvec->mode = (RISCV::tvecMode)(*value & RISCV::tvecModeMask);
        } else {
            *value = ((XLEN_t)tvec->base) | ((XLEN_t)tvec->mode);
        }
    }

    template<RISCV::PrivilegeMode viewPrivilege, bool Writing>
    inline void XCAUSE(XLEN_t* value) {
        RISCV::causeReg<XLEN_t>* xcause = &mcause;
        if constexpr (viewPrivilege == RISCV::PrivilegeMode::Supervisor) {
            xcause = &scause;
        }
        if constexpr (viewPrivilege == RISCV::PrivilegeMode::User) {
            xcause = &ucause;
        }
        if constexpr (Writing) {
            if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                xcause->interrupt = *value & ((XLEN_t)1 << 31);
                xcause->exceptionCode = (RISCV::TrapCause)(*value & ~((XLEN_t)1 << 31));
            } else {
                xcause->interrupt = *value & ((XLEN_t)1 << 63);
                xcause->exceptionCode = (RISCV::TrapCause)(*value & ~((XLEN_t)1 << 63));
            }
        } else {
            *value = 0;
            *value |= xcause->exceptionCode;
            if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                if (xcause->interrupt) {
                    *value |= ((XLEN_t)1 << 31);
                }
            } else {
                if (xcause->interrupt) {
                    *value |= ((XLEN_t)1 << 63);
                }
            }
        }
    }

    template<bool Writing>
    inline void SimpleCSR(XLEN_t& csr, XLEN_t* value) {
        if constexpr (Writing) {
            csr = *value;
        } else {
            *value = csr;
        }
    }

    template<bool Writing>
    inline void CSR(RISCV::CSRAddress csrAddress, XLEN_t* value) {

        if constexpr (!Writing) {
            *value = 0;
        }

        switch (csrAddress) {
            case RISCV::CSRAddress::MISA: MISA<Writing>(value); break;
            case RISCV::CSRAddress::SATP: SATP<Writing>(value); break;
            case RISCV::CSRAddress::MSTATUS: XSTATUS<RISCV::PrivilegeMode::Machine, Writing>(value); break;
            case RISCV::CSRAddress::SSTATUS: XSTATUS<RISCV::PrivilegeMode::Supervisor, Writing>(value); break;
            case RISCV::CSRAddress::USTATUS: XSTATUS<RISCV::PrivilegeMode::User, Writing>(value); break;
            case RISCV::CSRAddress::MIE: XIY<true, RISCV::PrivilegeMode::Machine, Writing>(value); break;
            case RISCV::CSRAddress::SIE: XIY<true, RISCV::PrivilegeMode::Supervisor, Writing>(value); break;
            case RISCV::CSRAddress::UIE: XIY<true, RISCV::PrivilegeMode::User, Writing>(value); break;
            case RISCV::CSRAddress::MIP: XIY<false, RISCV::PrivilegeMode::Machine, Writing>(value); break;
            case RISCV::CSRAddress::SIP: XIY<false, RISCV::PrivilegeMode::Supervisor, Writing>(value); break;
            case RISCV::CSRAddress::UIP: XIY<false, RISCV::PrivilegeMode::User, Writing>(value); break;
            case RISCV::CSRAddress::MTVEC: XTVEC<RISCV::PrivilegeMode::Machine, Writing>(value); break;
            case RISCV::CSRAddress::MSCRATCH: SimpleCSR<Writing>(mscratch, value); break;
            case RISCV::CSRAddress::MEPC: SimpleCSR<Writing>(mepc, value); break;
            case RISCV::CSRAddress::MCAUSE: XCAUSE<RISCV::PrivilegeMode::Machine, Writing>(value); break;
            case RISCV::CSRAddress::MTVAL: SimpleCSR<Writing>(mtval, value); break;
            case RISCV::CSRAddress::MEDELEG: SimpleCSR<Writing>(medeleg, value); break;
            case RISCV::CSRAddress::MIDELEG: SimpleCSR<Writing>(mideleg, value); break;
            case RISCV::CSRAddress::STVEC: XTVEC<RISCV::PrivilegeMode::Supervisor, Writing>(value); break;
            case RISCV::CSRAddress::SSCRATCH: SimpleCSR<Writing>(sscratch, value); break;
            case RISCV::CSRAddress::SEPC: SimpleCSR<Writing>(sepc, value); break;
            case RISCV::CSRAddress::SCAUSE: XCAUSE<RISCV::PrivilegeMode::Supervisor, Writing>(value); break;
            case RISCV::CSRAddress::STVAL: SimpleCSR<Writing>(stval, value); break;
            case RISCV::CSRAddress::SEDELEG: SimpleCSR<Writing>(sedeleg, value); break;
            case RISCV::CSRAddress::SIDELEG: SimpleCSR<Writing>(sideleg, value); break;
            case RISCV::CSRAddress::UTVEC: XTVEC<RISCV::PrivilegeMode::User, Writing>(value); break;
            case RISCV::CSRAddress::USCRATCH: SimpleCSR<Writing>(uscratch, value); break;
            case RISCV::CSRAddress::UEPC: SimpleCSR<Writing>(uepc, value); break;
            case RISCV::CSRAddress::UCAUSE: XCAUSE<RISCV::PrivilegeMode::User, Writing>(value); break;
            case RISCV::CSRAddress::UTVAL: SimpleCSR<Writing>(utval, value); break;
            case RISCV::CSRAddress::MHARTID:
                if constexpr (Writing) {
                    // TODO invalid? read the spec
                } else {
                    *value = 0; // TODO multihart, non-zero mhartid option.
                }
                break;
            case RISCV::CSRAddress::MVENDORID:
                break;
            case RISCV::CSRAddress::MARCHID:
                break;
            case RISCV::CSRAddress::MIMPID:
                break;
            case RISCV::CSRAddress::FFLAGS:
                break;
            case RISCV::CSRAddress::FRM:
                break;
            case RISCV::CSRAddress::FCSR:
                break;
            case RISCV::CSRAddress::SCOUNTEREN:
                break;
            case RISCV::CSRAddress::MCOUNTEREN:
                break;
            case RISCV::CSRAddress::MCOUNTINHIBIT:
                break;
            case RISCV::CSRAddress::TSELECT:
                break;
            case RISCV::CSRAddress::TDATA1:
                break;
            case RISCV::CSRAddress::TDATA2:
                break;
            case RISCV::CSRAddress::TDATA3:
                break;
            case RISCV::CSRAddress::DCSR:
                break;
            case RISCV::CSRAddress::DPC:
                break;
            case RISCV::CSRAddress::DSCRATCH0:
                break;
            case RISCV::CSRAddress::DSCRATCH1:
                break;
            case RISCV::CSRAddress::INVALID_CSR:
                break;
            default:
                BankedCSR<Writing>(csrAddress, value);
                break;
        }
        // TODO error here
    }
    
    template<bool Writing>
    inline void BankedCSR(RISCV::CSRAddress csrAddress, XLEN_t* value) {
        if (csrAddress >= RISCV::CSRAddress::MCYCLE &&
            csrAddress <= RISCV::CSRAddress::MHPMCOUNTER31) {
            // unsigned int counterID = csrAddress - RISCV::CSRAddress::CYCLE;
            // TODO XLEN truncation
            // TODO MTIME carveout
        }

        if (csrAddress >= RISCV::CSRAddress::MCYCLEH &&
            csrAddress <= RISCV::CSRAddress::MHPMCOUNTER31H) {
            // unsigned int counterID = csrAddress - RISCV::CSRAddress::CYCLEH;
            // TODO shifting
            // TODO MTIME carveout
        }

        if (csrAddress >= RISCV::CSRAddress::CYCLE &&
            csrAddress <= RISCV::CSRAddress::HPMCOUNTER31) {
            // unsigned int counterID = csrAddress - RISCV::CSRAddress::CYCLE;
            // TODO XLEN truncation
            // TODO counter passthrough
        }

        if (csrAddress >= RISCV::CSRAddress::CYCLEH &&
            csrAddress <= RISCV::CSRAddress::HPMCOUNTER31H) {
            // unsigned int counterID = csrAddress - RISCV::CSRAddress::CYCLEH;
            // TODO shifting
            // TODO counter passthrough
        }

        if (csrAddress >= RISCV::CSRAddress::MHPMEVENT3 &&
            csrAddress <= RISCV::CSRAddress::MHPMEVENT31) {
            // unsigned int counterID = csrAddress - RISCV::CSRAddress::MHPMEVENT3 + 3;
            // TODO events
        }

        if (csrAddress >= RISCV::CSRAddress::PMPADDR0 &&
            csrAddress <= RISCV::CSRAddress::PMPADDR15) {
            // unsigned int pmpEntryID = csrAddress - RISCV::CSRAddress::PMPADDR0;
            // TODO pmp address
        }

        if (csrAddress >= RISCV::CSRAddress::PMPCFG0 &&
            csrAddress <= RISCV::CSRAddress::PMPCFG3) {
            // TODO four pmp entries at once
        }
    }

    inline XLEN_t ReadCSR(RISCV::CSRAddress csrAddress) {
        XLEN_t value;
        CSR<false>(csrAddress, &value);
        return value;
    }

    inline void WriteCSR(RISCV::CSRAddress csrAddress, XLEN_t value) {
        CSR<true>(csrAddress, &value);
    }

    // -- Mutations --

    inline void RaiseException(RISCV::TrapCause cause, XLEN_t tval) {

        if (cause == RISCV::TrapCause::NONE) {
            return;
        }

        RISCV::PrivilegeMode targetPrivilege = RISCV::DestinedPrivilegeForCause<XLEN_t>(
            cause, medeleg, sedeleg, misa.extensions);
        TakeTrap<false>(cause, targetPrivilege, tval);
    }

    inline void ServiceInterrupts() {

        XLEN_t interruptsForM = 0;
        XLEN_t interruptsForS = 0;
        XLEN_t interruptsForU = 0;

        // TODO do this without bits, just use raw values. Faster...
        XLEN_t mipBits = 0;
        XIY<false, RISCV::PrivilegeMode::Machine, false>(&mipBits);
        XLEN_t mieBits = 0;
        XIY<true, RISCV::PrivilegeMode::Machine, false>(&mieBits);

        for (unsigned int bit = 0; bit < 8*sizeof(XLEN_t); bit++) {

            // A deasserted or disabled interrupt is not serviceable
            if (!(mipBits & (1<<bit)) || !(mieBits & (1<<bit))) {
                continue;
            }

            // Figure out the destined privilege level for the interrupt
            RISCV::PrivilegeMode destinedPrivilege =
                RISCV::DestinedPrivilegeForCause<XLEN_t>(
                    (RISCV::TrapCause)bit, mideleg, sideleg, misa.extensions);

            // Set the interrupt's bit in the correct mask for its privilege
            if (destinedPrivilege == RISCV::PrivilegeMode::Machine) {
                interruptsForM |= 1<<bit;
            } else if (destinedPrivilege == RISCV::PrivilegeMode::Supervisor) {
                interruptsForS |= 1<<bit;
            } else if (destinedPrivilege == RISCV::PrivilegeMode::User) {
                interruptsForU |= 1<<bit;
            }
        }

        // Select the highest-privilege enabled non-empty interrupt vector that is at or higher than our own privilege, and
        RISCV::PrivilegeMode targetPrivilege;
        XLEN_t interruptsToService = 0;
        if (privilegeMode <= RISCV::PrivilegeMode::Machine && interruptsForM != 0 && mstatus.mie) {
            targetPrivilege = RISCV::PrivilegeMode::Machine;
            interruptsToService = interruptsForM;
        } else if (privilegeMode <= RISCV::PrivilegeMode::Supervisor && interruptsForS != 0 && mstatus.sie) {
            targetPrivilege = RISCV::PrivilegeMode::Supervisor;
            interruptsToService = interruptsForS;
        } else if (privilegeMode <= RISCV::PrivilegeMode::User && interruptsForU != 0 && mstatus.uie) {
            targetPrivilege = RISCV::PrivilegeMode::User;
            interruptsToService = interruptsForU;
        } else {
            return;
        }

        RISCV::TrapCause cause = RISCV::highestPriorityInterrupt(interruptsToService);
        TakeTrap<true>(cause, targetPrivilege, 0);
    }

    template<bool isInterrupt>
    inline void TakeTrap(RISCV::TrapCause cause, RISCV::PrivilegeMode targetPrivilege, XLEN_t tval) {

        if (targetPrivilege < privilegeMode) {
            targetPrivilege = privilegeMode;
        }
        
        // When a trap is taken from privilege mode y into privilege mode x,
        // xPIE is set to the value of xIE;
        // xIE is set to 0;
        // and xPP is set to y.

        // When a trap is delegated to a less-privileged mode x:
        // the xcause register is written with the trap cause,
        // the xepc register is written with the virtual address of the
        //     instruction that took the trap,
        // the xtval register is written with an exception-specific datum,
        // the xPP field of mstatus is written with the active privilege mode
        //     at the time of the trap,
        // the xPIE field of mstatus is written with the value of the xIE field
        //     at the time of the trap, and
        // the xIE field of mstatus is cleared.

        switch (targetPrivilege) {
        case RISCV::PrivilegeMode::Machine:
            mcause.exceptionCode = cause;
            mcause.interrupt = isInterrupt;
            mepc = currentFetch->virtualPC;
            mtval = tval;
            mstatus.mpp = privilegeMode;
            mstatus.mpie = mstatus.mie;
            mstatus.mie = false;
            privilegeMode = targetPrivilege;
            nextFetchVirtualPC = mtvec.base;
            if (mtvec.mode == RISCV::tvecMode::Vectored && !isInterrupt) {
                nextFetchVirtualPC += 4*mcause.exceptionCode;
            }
            break;
        case RISCV::PrivilegeMode::Supervisor:
            scause.exceptionCode = cause;
            scause.interrupt = isInterrupt;
            sepc = currentFetch->virtualPC;
            stval = tval;
            mstatus.spp = privilegeMode;
            mstatus.spie = mstatus.sie;
            mstatus.sie = false;
            privilegeMode = targetPrivilege;
            nextFetchVirtualPC = stvec.base;
            if (stvec.mode == RISCV::tvecMode::Vectored && !isInterrupt) {
                nextFetchVirtualPC += 4*scause.exceptionCode;
            }
            break;
        case RISCV::PrivilegeMode::User:
            ucause.exceptionCode = cause;
            ucause.interrupt = isInterrupt;
            uepc = currentFetch->virtualPC;
            utval = tval;
            mstatus.upie = mstatus.uie;
            mstatus.uie = false;
            privilegeMode = targetPrivilege;
            nextFetchVirtualPC = utvec.base;
            if (utvec.mode == RISCV::tvecMode::Vectored && !isInterrupt) {
                nextFetchVirtualPC += 4*ucause.exceptionCode;
            }
            break;
        default:
            // fatal("Trap destined for nonsense privilege mode."); // TODO
            break;
        }
        implCallback(HartCallbackArgument::ChangedPrivilege);
    }

    template<RISCV::PrivilegeMode trapPrivilege>
    inline void ReturnFromTrap() {

        // TODO verify the below logic against the following spec snippet:
        // To return after handling a trap, there are separate trap return instructions per privilege level: MRET, SRET,
        // and URET. MRET is always provided. SRET must be provided if supervisor mode is supported, and should raise an
        // illegal instruction exception otherwise. SRET should also raise an illegal instruction exception when TSR=1
        // in mstatus, as described in Section 3.1.6.4. URET is only provided if user-mode traps are supported, and
        // should raise an illegal instruction otherwise. An x RET instruction can be executed in privilege mode x or
        // higher, where executing a lower-privilege x RET instruction will pop the relevant lower-privilege interrupt
        // enable and privilege mode stack. In addition to manipulating the privilege stack as described in Section
        // 3.1.6.1, x RET sets the pc to the value stored in the x epc register.

        if constexpr (trapPrivilege == RISCV::PrivilegeMode::Machine) {
            mstatus.mie = mstatus.mpie;
            privilegeMode = mstatus.mpp;
            mstatus.mpie = true;
            if (RISCV::vectorHasExtension(misa.extensions, 'U')) {
                mstatus.mpp = RISCV::PrivilegeMode::User;
            } else {
                mstatus.mpp = RISCV::PrivilegeMode::Machine;
            }
            nextFetchVirtualPC = mepc;
        } else if constexpr (trapPrivilege == RISCV::PrivilegeMode::Supervisor) {
            mstatus.sie = mstatus.spie;
            privilegeMode = mstatus.spp;
            mstatus.spie = true;
            if (RISCV::vectorHasExtension(misa.extensions, 'U')) {
                mstatus.spp = RISCV::PrivilegeMode::User;
            } else {
                mstatus.spp = RISCV::PrivilegeMode::Machine;
            }
            nextFetchVirtualPC = sepc;
        } else if constexpr (trapPrivilege == RISCV::PrivilegeMode::User) {
            mstatus.uie = mstatus.upie;
            mstatus.upie = true;
            nextFetchVirtualPC = uepc;
        } else {
            // fatal("Return from nonsense-privilege-mode trap"); // TODO
        }

        implCallback(HartCallbackArgument::ChangedPrivilege);
    }

};
