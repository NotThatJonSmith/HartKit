#pragma once

#include <cstdint>

#include <RiscV.hpp>

#include <HartSpec.hpp>

#include <Register.hpp>
#include <HartSpec.hpp>

#include <Swizzle.hpp>
#include <MMU.hpp>

#include <Operands.hpp>
#include <Instruction.hpp>

class HartState {

public:

    HartState();
    void Reset(HartSpec* spec);

    #pragma mark -- Change Notification Callbacks --

    std::function<void(void)> notifySoftwareChangedSATP;
    std::function<void(void)> notifySoftwareChangedMSTATUS;
    std::function<void(void)> notifySoftwareChangedMISA;
    std::function<void(void)> notifyPrivilegeChanged;
    std::function<void(void)> notifyInstructionFenceRequested;
    std::function<void(void)> notifyVMFenceRequested;
    static void emptyNotifyHandler();

    #pragma mark -- Operating State --

    struct Fetch {
        __uint32_t encoding;
        Instruction instruction;
        Operands operands;
        Register virtualPC;
    };
    Fetch *currentFetch = nullptr;
    Register* nextFetchVirtualPC;
    Register regs[RISCV::NumRegs];
    RegisterLikeObject *csrs[RISCV::NumCSRs];
    RISCV::PrivilegeMode privilegeMode = RISCV::PrivilegeMode::Machine;

    MMU* mmu;

    #pragma mark -- MISA Fields --

    RISCV::XlenMode mxlen;
    __uint32_t extensions;

    #pragma mark -- MSTATUS Fields --

    bool userInterruptsEnabled;
    bool supervisorInterruptsEnabled;
    bool machineInterruptsEnabled;
    bool userPreviousInterruptsEnabled;
    bool supervisorPreviousInterruptsEnabled;
    bool machinePreviousInterruptsEnabled;
    RISCV::PrivilegeMode supervisorPreviousPrivilege;
    RISCV::PrivilegeMode machinePreviousPrivilege;
    RISCV::FloatingPointState floatingPointState;
    RISCV::ExtensionState extensionState;
    bool modifyMemoryPrivilege;
    bool supervisorUserMemoryAccess;
    bool makeExecutableReadable;
    bool trapVirtualMemory;
    bool timeoutWait;
    bool trapSRET;
    RISCV::XlenMode sxlen;
    RISCV::XlenMode uxlen;
    bool stateDirty;

    #pragma mark -- SATP Fields --

    RISCV::PagingMode pagingMode;
    Register ppn;
    Register asid;

    #pragma mark -- Memory --

    /*
     * Transact against the attached MMU object. Raise an exception if
     * appropriate. Return whether the transaction was successful or not. Useful
     * for instructions to access memory, but prefetching simulators may do
     * their own transactions directly against the MMU if they like,
     * for example, to prefetch transactions but defer traps until the simulated
     * hart actually attempts execution.
     */
    template<typename XLEN_t, CASK::AccessType accessType>
    inline bool Transact(XLEN_t address, XLEN_t size, char* buf) {
        TransactionResult<XLEN_t> transactionResult =
            mmu->Transact<XLEN_t, accessType>(address, size, buf);
        if (transactionResult.trapCause != RISCV::TrapCause::NONE) {
            RaiseException<XLEN_t>(transactionResult.trapCause, address);
            return false;
        }
        return size == transactionResult.transferredSize;
    }

    #pragma mark -- Queries --

    RISCV::XlenMode GetXLEN();

    template<typename XLEN_t>
    inline void SetMISA(XLEN_t value) {
        unsigned int shift = (sizeof(XLEN_t)*8)-2;
        extensions = value & 0x3ffffff;
        mxlen = (RISCV::XlenMode)(value >> shift);
        notifySoftwareChangedMISA();
    }

    template<typename XLEN_t>
    inline XLEN_t GetMISA() {
        unsigned int shift = (sizeof(XLEN_t)*8)-2;
        return (((XLEN_t)mxlen) << shift) | extensions;
    }

    template<typename XLEN_t>
    inline XLEN_t GetSATP() {
        if constexpr (std::is_same<XLEN_t, __uint64_t>() || std::is_same<XLEN_t, __uint128_t>()) {
            return (((__uint64_t)pagingMode          & 0x0000000000f) << 60)|
                   ((            asid.Read<XLEN_t>() & 0x0000000ffff) << 44)| 
                   ((            ppn.Read<XLEN_t>()  & 0xfffffffffff) << 00);
        }
        return ((pagingMode          & 0x000001) << 31)|
               ((asid.Read<XLEN_t>() & 0x0001ff) << 22)| 
               ((ppn.Read<XLEN_t>()  & 0x3fffff) << 00);
    }

    template<typename XLEN_t>
    inline void SetSATP(XLEN_t value) {
        pagingMode = (RISCV::PagingMode)swizzle<XLEN_t, ExtendBits::Zero, 31, 31>(value);
        asid.Write<XLEN_t>(swizzle<XLEN_t, ExtendBits::Zero, 30, 22>(value));
        ppn.Write<XLEN_t>(swizzle<XLEN_t, ExtendBits::Zero, 21, 0>(value));
        if constexpr (!std::is_same<XLEN_t, __uint32_t>()) {
            pagingMode = (RISCV::PagingMode)swizzle<XLEN_t, ExtendBits::Zero, 63, 60>(value);
            asid.Write<XLEN_t>(swizzle<XLEN_t, ExtendBits::Zero, 59, 44>(value));
            ppn.Write<XLEN_t>(swizzle<XLEN_t, ExtendBits::Zero, 43, 0>(value));
        }
        notifySoftwareChangedSATP();
    }

    template <typename XLEN_t, RISCV::PrivilegeMode instructionPrivilege>
    inline XLEN_t GetMSTATUS() {

        XLEN_t value = 0;
        value |= userInterruptsEnabled ? RISCV::uieMask : 0;
        value |= userPreviousInterruptsEnabled ? RISCV::upieMask : 0;

        if constexpr (instructionPrivilege == RISCV::PrivilegeMode::Machine || instructionPrivilege == RISCV::PrivilegeMode::Supervisor) {
            value |= supervisorInterruptsEnabled ? RISCV::sieMask : 0;
            value |= supervisorPreviousInterruptsEnabled ? RISCV::spieMask : 0;
            value |= supervisorPreviousPrivilege << RISCV::sppShift;
            value |= floatingPointState << RISCV::fsShift;
            value |= extensionState << RISCV::xsShift;
            value |= supervisorUserMemoryAccess ? RISCV::sumMask : 0;
            value |= makeExecutableReadable ? RISCV::mxrMask : 0;
            if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                value |= stateDirty ? (1 << 31) : 0;
            } else {
                value |= stateDirty ? ((XLEN_t)1 << 63) : 0;
            }
        }

        if constexpr (instructionPrivilege == RISCV::PrivilegeMode::Machine) {
            value |= machineInterruptsEnabled ? RISCV::mieMask : 0;
            value |= machinePreviousInterruptsEnabled ? RISCV::mpieMask : 0;
            value |= machinePreviousPrivilege << RISCV::mppShift;
            if constexpr (!std::is_same<XLEN_t, __uint32_t>()) {
                value |= (__uint64_t)(uxlen) << RISCV::uxlShift;
                value |= (__uint64_t)(sxlen) << RISCV::sxlShift;
            }
            value |= modifyMemoryPrivilege ? RISCV::mprvMask : 0;
            value |= trapVirtualMemory ? RISCV::tvmMask : 0;
            value |= timeoutWait ? RISCV::twMask : 0;
            value |= trapSRET ? RISCV::tsrMask : 0;
        }

        return value;
    }

    template <typename XLEN_t, RISCV::PrivilegeMode instructionPrivilege>
    inline void SetMSTATUS(XLEN_t value) {

        userInterruptsEnabled = RISCV::uieMask & value;
        userPreviousInterruptsEnabled = RISCV::upieMask & value;
        
        if constexpr (instructionPrivilege == RISCV::PrivilegeMode::Machine || instructionPrivilege == RISCV::PrivilegeMode::Supervisor) {
            supervisorInterruptsEnabled = RISCV::sieMask & value;
            supervisorPreviousInterruptsEnabled = RISCV::spieMask & value;
            supervisorPreviousPrivilege = (RISCV::PrivilegeMode)((RISCV::sppMask & value) >> RISCV::sppShift);
            floatingPointState = (RISCV::FloatingPointState)((RISCV::fsMask & value) >> RISCV::fsShift);
            extensionState = (RISCV::ExtensionState)((RISCV::xsMask & value) >> RISCV::xsShift);
            supervisorUserMemoryAccess = RISCV::sumMask & value;
            makeExecutableReadable = RISCV::mxrMask & value;
            if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                stateDirty = value & (1 << 31);
            } else {
                stateDirty = value & ((XLEN_t)1 << 63);
            }
        }

        if constexpr (instructionPrivilege == RISCV::PrivilegeMode::Machine) {
            machineInterruptsEnabled = RISCV::mieMask & value;
            machinePreviousInterruptsEnabled = RISCV::mppMask & value;
            machinePreviousPrivilege = (RISCV::PrivilegeMode)((RISCV::mppMask & value) >> RISCV::mppShift);
            if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
                uxlen = RISCV::XlenMode::XL32;
                sxlen = RISCV::XlenMode::XL32;
            } else {
                uxlen = (RISCV::XlenMode)((RISCV::uxlMask & value) >> RISCV::uxlShift);
                sxlen = (RISCV::XlenMode)((RISCV::sxlMask & value) >> RISCV::sxlShift);
            }
            modifyMemoryPrivilege = RISCV::mprvMask & value;
            trapVirtualMemory = RISCV::tvmMask & value;
            timeoutWait = RISCV::twMask & value;
            trapSRET = RISCV::tsrMask & value;
        }

        notifySoftwareChangedMSTATUS();
    }

    #pragma mark -- Mutations -- 

    template<typename XLEN_t>
    inline void RaiseException(RISCV::TrapCause cause, XLEN_t tval) {

        if (cause == RISCV::TrapCause::NONE) {
            return;
        }

        RISCV::PrivilegeMode targetPrivilege = RISCV::DestinedPrivilegeForCause<XLEN_t>(
            cause,
            csrs[RISCV::CSRAddress::MEDELEG]->Get<XLEN_t>(),
            csrs[RISCV::CSRAddress::SEDELEG]->Get<XLEN_t>(),
            extensions);
        TakeTrap<XLEN_t>(cause, targetPrivilege, tval, false);
    }

    void ServiceInterrupts();

    template<typename XLEN_t>
    inline void ServiceInterrupts() {

        // TODO, small bug, MIP MIE MIDELEG MEDELEG are MXLEN bits wide, not XLEN...
        XLEN_t mip = csrs[RISCV::CSRAddress::MIP]->Get<XLEN_t>();
        XLEN_t mie = csrs[RISCV::CSRAddress::MIE]->Get<XLEN_t>();
        XLEN_t mideleg = csrs[RISCV::CSRAddress::MIDELEG]->Get<XLEN_t>();
        XLEN_t sideleg = csrs[RISCV::CSRAddress::SIDELEG]->Get<XLEN_t>();
        
        XLEN_t interruptsForM = 0;
        XLEN_t interruptsForS = 0;
        XLEN_t interruptsForU = 0;
        for (unsigned int bit = 0; bit < 8*sizeof(XLEN_t); bit++) {

            // A deasserted or disabled interrupt is not serviceable
            if (!(mip & (1<<bit)) || !(mie & (1<<bit))) {
                continue;
            }

            // Figure out the destined privilege level for the interrupt
            RISCV::PrivilegeMode destinedPrivilege =
                RISCV::DestinedPrivilegeForCause<XLEN_t>(
                    (RISCV::TrapCause)bit,
                    mideleg, sideleg, extensions);

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
        if (privilegeMode <= RISCV::PrivilegeMode::Machine && interruptsForM != 0 && machineInterruptsEnabled) {
            targetPrivilege = RISCV::PrivilegeMode::Machine;
            interruptsToService = interruptsForM;
        } else if (privilegeMode <= RISCV::PrivilegeMode::Supervisor && interruptsForS != 0 && supervisorInterruptsEnabled) {
            targetPrivilege = RISCV::PrivilegeMode::Supervisor;
            interruptsToService = interruptsForS;
        } else if (privilegeMode <= RISCV::PrivilegeMode::User && interruptsForU != 0 && userInterruptsEnabled) {
            targetPrivilege = RISCV::PrivilegeMode::User;
            interruptsToService = interruptsForU;
        } else {
            return;
        }

        RISCV::TrapCause cause = RISCV::highestPriorityInterrupt(interruptsToService);
        TakeTrap(cause, targetPrivilege, 0, true);
    }

    template<typename XLEN_t>
    inline void TakeTrap(RISCV::TrapCause cause, RISCV::PrivilegeMode targetPrivilege, XLEN_t tval, bool isInterrupt) {

        if (targetPrivilege < privilegeMode) {
            targetPrivilege = privilegeMode;
        }

        RISCV::TrapCSRs trapToCSRs = RISCV::trapCSRsForPrivilege[targetPrivilege];
        XLEN_t mcauseInterruptBit = 1 << ((__uint32_t)mxlen-1);

        // When a trap is delegated to a less-privileged mode x, the xcause
        // register is written with the trap cause,
        if (isInterrupt) {
            csrs[trapToCSRs.cause]->Set<XLEN_t>(cause | mcauseInterruptBit);
        } else {
            csrs[trapToCSRs.cause]->Set<XLEN_t>(cause);
        }

        // ...the xepc register is written with the virtual address of the
        // instruction that took the trap,
        csrs[trapToCSRs.epc]->Set<XLEN_t>(currentFetch->virtualPC.Read<XLEN_t>());

        // ...the xtval register is written with an exception-specific datum,
        if (!isInterrupt) {
            csrs[trapToCSRs.tval]->Set<XLEN_t>(tval);
        }

        // ...the xPP field of mstatus is written with the active privilege mode
        // at the time of the trap,
        if (targetPrivilege == RISCV::PrivilegeMode::Machine) {
            machinePreviousPrivilege = privilegeMode;
        } else if (targetPrivilege == RISCV::PrivilegeMode::Supervisor) {
            supervisorPreviousPrivilege = privilegeMode;
        } // else - destined for U mode, and UPP is implicitly always 0.

        // ...the xPIE field of mstatus is written with the value of the xIE
        // field at the time of the trap, and the xIE field of mstatus is
        // cleared.
        switch (targetPrivilege) {
        case RISCV::PrivilegeMode::Machine:
            machinePreviousInterruptsEnabled = machineInterruptsEnabled;
            machineInterruptsEnabled = false;
            break;
        case RISCV::PrivilegeMode::Supervisor:
            supervisorPreviousInterruptsEnabled = supervisorInterruptsEnabled;
            supervisorInterruptsEnabled = false;
            break;
        case RISCV::PrivilegeMode::User:
            userPreviousInterruptsEnabled = userInterruptsEnabled;
            userInterruptsEnabled = false;
            break;
        default:
            // fatal("Trap destined for nonsense privilege mode."); // TODO
            break;
        }

        // Read the CSRs needed to get a destination
        XLEN_t tvec_value = csrs[trapToCSRs.tvec]->Get<XLEN_t>();
        XLEN_t tvec_base = tvec_value & RISCV::tvecBaseMask;
        RISCV::tvecMode tvec_mode = (RISCV::tvecMode)(tvec_value & RISCV::tvecModeMask);
        XLEN_t tcause_value = csrs[trapToCSRs.cause]->Get<XLEN_t>();

        // Calculate the destination
        XLEN_t trap_destination = tvec_base;
        if (tvec_mode == RISCV::tvecMode::Vectored && !isInterrupt) {
            trap_destination += 4*tcause_value;
        }

        // Actually take the trap.
        privilegeMode = targetPrivilege;
        notifyPrivilegeChanged();
        nextFetchVirtualPC->Write<XLEN_t>(trap_destination);
    }

    template<typename XLEN_t, RISCV::PrivilegeMode trapPrivilege>
    inline void ReturnFromTrap() {

        // TODO verify this logic
        // To return after handling a trap, there are separate trap return instructions per privilege level: MRET, SRET,
        // and URET. MRET is always provided. SRET must be provided if supervisor mode is supported, and should raise an
        // illegal instruction exception otherwise. SRET should also raise an illegal instruction exception when TSR=1
        // in mstatus, as described in Section 3.1.6.4. URET is only provided if user-mode traps are supported, and
        // should raise an illegal instruction otherwise. An x RET instruction can be executed in privilege mode x or
        // higher, where executing a lower-privilege x RET instruction will pop the relevant lower-privilege interrupt
        // enable and privilege mode stack. In addition to manipulating the privilege stack as described in Section
        // 3.1.6.1, x RET sets the pc to the value stored in the x epc register.

        if constexpr (trapPrivilege == RISCV::PrivilegeMode::Machine) {
            machineInterruptsEnabled = machinePreviousInterruptsEnabled;
            privilegeMode = machinePreviousPrivilege;
            machinePreviousInterruptsEnabled = true;
            if (RISCV::vectorHasExtension(extensions, 'U')) {
                machinePreviousPrivilege = RISCV::PrivilegeMode::User;
            } else {
                machinePreviousPrivilege = RISCV::PrivilegeMode::Machine;
            }
            nextFetchVirtualPC->Write<XLEN_t>(csrs[RISCV::CSRAddress::MEPC]->Get<XLEN_t>());
        } else if constexpr (trapPrivilege == RISCV::PrivilegeMode::Supervisor) {
            supervisorInterruptsEnabled = supervisorPreviousInterruptsEnabled;
            privilegeMode = supervisorPreviousPrivilege;
            supervisorPreviousInterruptsEnabled = true;
            if (RISCV::vectorHasExtension(extensions, 'U')) {
                supervisorPreviousPrivilege = RISCV::PrivilegeMode::User;
            } else {
                supervisorPreviousPrivilege = RISCV::PrivilegeMode::Machine;
            }
            nextFetchVirtualPC->Write<XLEN_t>(csrs[RISCV::CSRAddress::SEPC]->Get<XLEN_t>());
        } else if constexpr (trapPrivilege == RISCV::PrivilegeMode::User) {
            userInterruptsEnabled = userPreviousInterruptsEnabled;
            userPreviousInterruptsEnabled = true;
            nextFetchVirtualPC->Write<XLEN_t>(csrs[RISCV::CSRAddress::UEPC]->Get<XLEN_t>());
        } else {
            // fatal("Return from nonsense-privilege-mode trap"); // TODO
        }

        notifyPrivilegeChanged();
    }
};
