#pragma once

#include <cstdint>
#include <functional>

#include <RiscV.hpp>
#include <Operands.hpp>
#include <DecodedInstruction.hpp>
#include <Swizzle.hpp>


template<typename XLEN_t>
class HartState {

public:

    // Broken out into an inner class, and we maintain a pointer to the "current" one, so optimized simulators can
    // prefetch into a buffer maintained outside this object.

    struct Fetch {
        __uint32_t encoding;
        DecodedInstruction<XLEN_t> instruction;
        Operands operands;
        XLEN_t virtualPC;
    };

public:

    // -- Startup state --
    const __uint32_t maximalExtensions;

    // -- Operating State --

    Fetch *currentFetch = nullptr;
    XLEN_t nextFetchVirtualPC;
    XLEN_t regs[RISCV::NumRegs];
    RISCV::PrivilegeMode privilegeMode = RISCV::PrivilegeMode::Machine;

    // -- MISA, MSTATUS, and SATP Fields --

    RISCV::XlenMode mxlen;
    __uint32_t extensions;
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
    RISCV::PagingMode pagingMode;
    XLEN_t ppn;
    XLEN_t asid;

    // TODO, bug, MIP MIE MIDELEG MEDELEG are MXLEN bits wide, not XLEN...

    XLEN_t mcause;
    XLEN_t mepc;
    XLEN_t mtvec;
    XLEN_t mtval;
    XLEN_t mip;
    XLEN_t mie;
    XLEN_t mideleg;
    XLEN_t medeleg;

    XLEN_t scause;
    XLEN_t sepc;
    XLEN_t stvec;
    XLEN_t stval;
    XLEN_t sip;
    XLEN_t sie;
    XLEN_t sideleg;
    XLEN_t sedeleg;

    XLEN_t ucause;
    XLEN_t uepc;
    XLEN_t utvec;
    XLEN_t utval;
    XLEN_t uip;
    XLEN_t uie;

    // -- Change Notification Callbacks --

    // TODO should these be some other object the ex_ functions know about?
    // that's lots of argument-copying.... but then, bundle + ptr chase them?
    std::function<void(void)> notifySoftwareChangedSATP;
    std::function<void(void)> notifySoftwareChangedMSTATUS;
    std::function<void(void)> notifySoftwareChangedMISA;
    std::function<void(void)> notifyPrivilegeChanged;
    std::function<void(void)> notifyInstructionFenceRequested;
    std::function<void(void)> notifyVMFenceRequested;
    static void emptyNotifyHandler() {}

    struct CSRAccessors {
        std::function<XLEN_t(void)> Read;
        std::function<void(XLEN_t)> Write;
    };
    CSRAccessors csrs[RISCV::NumCSRs];
    
private:

    XLEN_t csr_scratch[RISCV::NumRegs];

public:

    HartState(__uint32_t allSupportedExtensions)
        : maximalExtensions(allSupportedExtensions) {

        notifySoftwareChangedMISA = HartState::emptyNotifyHandler;
        notifySoftwareChangedMSTATUS = HartState::emptyNotifyHandler;
        notifySoftwareChangedSATP = HartState::emptyNotifyHandler;
        notifyPrivilegeChanged = HartState::emptyNotifyHandler;
        notifyInstructionFenceRequested = HartState::emptyNotifyHandler;
        notifyVMFenceRequested = HartState::emptyNotifyHandler;

        privilegeMode = RISCV::PrivilegeMode::Machine;

        // TODO MHARTID done with a hardcoded/hardwired reg type.
        // TODO - BUG - SIE and UIE are masked off views of MIE

        // for (RISCV::CSRAddress csr_address = RISCV)

        // TODO some mechanism by which it's assumed that all the other CSRs are simple state? Below will get ugly.
        csrs[RISCV::CSRAddress::MSTATUS] = {
            std::bind(&HartState::GetMSTATUS<RISCV::PrivilegeMode::Machine>, this),
            std::bind(&HartState::SetMSTATUS<RISCV::PrivilegeMode::Machine>, this, std::placeholders::_1)
        };

        csrs[RISCV::CSRAddress::SSTATUS] = {
            std::bind(&HartState::GetMSTATUS<RISCV::PrivilegeMode::Supervisor>, this),
            std::bind(&HartState::SetMSTATUS<RISCV::PrivilegeMode::Supervisor>, this, std::placeholders::_1)
        };
        csrs[RISCV::CSRAddress::USTATUS] = {
            std::bind(&HartState::GetMSTATUS<RISCV::PrivilegeMode::User>, this),
            std::bind(&HartState::SetMSTATUS<RISCV::PrivilegeMode::User>, this, std::placeholders::_1)
        };
        csrs[RISCV::CSRAddress::MISA] = {
            std::bind(&HartState::GetMISA, this),
            std::bind(&HartState::SetMISA, this, std::placeholders::_1)
        };
        csrs[RISCV::CSRAddress::SATP] = {
            std::bind(&HartState::GetSATP, this),
            std::bind(&HartState::SetSATP, this, std::placeholders::_1)
        };
        // csrs[RISCV::CSRAddress::MCAUSE]     = new SimpleCSR();
        // csrs[RISCV::CSRAddress::SCAUSE]     = new SimpleCSR();
        // csrs[RISCV::CSRAddress::UCAUSE]     = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MEPC]       = new SimpleCSR();
        // csrs[RISCV::CSRAddress::SEPC]       = new SimpleCSR();
        // csrs[RISCV::CSRAddress::UEPC]       = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MIE]        = new SimpleCSR();
        // csrs[RISCV::CSRAddress::SIE]        = new SimpleCSR();
        // csrs[RISCV::CSRAddress::UIE]        = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MIP]        = new SimpleCSR();
        // csrs[RISCV::CSRAddress::SIP]        = new SimpleCSR();
        // csrs[RISCV::CSRAddress::UIP]        = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MEDELEG]    = new SimpleCSR();
        // csrs[RISCV::CSRAddress::SEDELEG]    = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MIDELEG]    = new SimpleCSR();
        // csrs[RISCV::CSRAddress::SIDELEG]    = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MTVEC]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::STVEC]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::UTVEC]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MTVAL]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::STVAL]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::UTVAL]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MSCRATCH]   = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MSCRATCH]   = {[&]{return csr_scratch[RISCV::CSRAddress::MSCRATCH];}, [&](XLEN_t value){csr_scratch[RISCV::CSRAddress::MSCRATCH] = value;}};
        // csrs[RISCV::CSRAddress::SSCRATCH]   = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MHARTID]    = new SimpleCSR();
        // csrs[RISCV::CSRAddress::MCOUNTEREN] = new SimpleCSR();
        // csrs[RISCV::CSRAddress::SCOUNTEREN] = new SimpleCSR();
        // csrs[RISCV::CSRAddress::PMPADDR0]   = new SimpleCSR();
        // csrs[RISCV::CSRAddress::PMPCFG0]    = new SimpleCSR();
        // csrs[RISCV::CSRAddress::TIME]       = new SimpleCSR();
        // csrs[RISCV::CSRAddress::TIMEH]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::CYCLE]      = new SimpleCSR();
        // csrs[RISCV::CSRAddress::CYCLEH]     = new SimpleCSR();
        // csrs[RISCV::CSRAddress::FCSR]       = new SimpleCSR();
        // csrs[RISCV::CSRAddress::INSTRET]    = new SimpleCSR();
        // csrs[RISCV::CSRAddress::INSTRETH]   = new SimpleCSR();
    }

    void Reset(XLEN_t resetVector) {
        privilegeMode = RISCV::PrivilegeMode::Machine;
        machineInterruptsEnabled = false;
        modifyMemoryPrivilege = false;
        extensions = maximalExtensions;
        mcause = 0;
        nextFetchVirtualPC = resetVector;
        mxlen = RISCV::xlenTypeToMode<XLEN_t>();
    }

    // -- Queries --

    inline void SetMISA(XLEN_t value) {
        unsigned int shift = (sizeof(XLEN_t)*8)-2;
        extensions = value & 0x3ffffff & maximalExtensions;
        mxlen = (RISCV::XlenMode)(value >> shift);
        notifySoftwareChangedMISA();
    }

    inline XLEN_t GetMISA() {
        unsigned int shift = (sizeof(XLEN_t)*8)-2;
        return (((XLEN_t)mxlen) << shift) | extensions;
    }

    inline XLEN_t GetSATP() {
        if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
            return ((pagingMode & 0x000001) << 31)|
                        (( asid & 0x0001ff) << 22)| 
                        ((  ppn & 0x3fffff) << 00);
        }
        return (((__uint64_t)pagingMode & 0x0000000000f) << 60)|
                                (( asid & 0x0000000ffff) << 44)| 
                                ((  ppn & 0xfffffffffff) << 00);
    }

    inline void SetSATP(XLEN_t value) {
        pagingMode = (RISCV::PagingMode)swizzle<XLEN_t, ExtendBits::Zero, 31, 31>(value);
        asid = swizzle<XLEN_t, ExtendBits::Zero, 30, 22>(value);
        ppn = swizzle<XLEN_t, ExtendBits::Zero, 21, 0>(value);
        if constexpr (!std::is_same<XLEN_t, __uint32_t>()) {
            pagingMode = (RISCV::PagingMode)swizzle<XLEN_t, ExtendBits::Zero, 63, 60>(value);
            asid = swizzle<XLEN_t, ExtendBits::Zero, 59, 44>(value);
            ppn = swizzle<XLEN_t, ExtendBits::Zero, 43, 0>(value);
        }
        notifySoftwareChangedSATP();
    }

    template <RISCV::PrivilegeMode instructionPrivilege>
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

    template <RISCV::PrivilegeMode instructionPrivilege>
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

    // -- Mutations -- 

    inline void RaiseException(RISCV::TrapCause cause, XLEN_t tval) {

        if (cause == RISCV::TrapCause::NONE) {
            return;
        }

        RISCV::PrivilegeMode targetPrivilege = RISCV::DestinedPrivilegeForCause<XLEN_t>(
            cause, medeleg, sedeleg, extensions);
        TakeTrap(cause, targetPrivilege, tval, false);
    }

    inline void ServiceInterrupts() {
        
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
                    (RISCV::TrapCause)bit, mideleg, sideleg, extensions);

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

    inline void TakeTrap(RISCV::TrapCause cause, RISCV::PrivilegeMode targetPrivilege, XLEN_t tval, bool isInterrupt) {

        if (targetPrivilege < privilegeMode) {
            targetPrivilege = privilegeMode;
        }

        XLEN_t interruptBit = 1 << ((__uint32_t)mxlen-1);

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

        XLEN_t trap_destination;
        RISCV::tvecMode tvec_mode;

        switch (targetPrivilege) {
        case RISCV::PrivilegeMode::Machine:

            mcause = cause;
            if (isInterrupt) {
                mcause |= interruptBit;
            }
            mepc = currentFetch->virtualPC;
            if (!isInterrupt) {
                mtval = tval;
            }
            machinePreviousPrivilege = privilegeMode;
            machinePreviousInterruptsEnabled = machineInterruptsEnabled;
            machineInterruptsEnabled = false;

            // Calculate the destination
            tvec_mode = (RISCV::tvecMode)(mtvec & RISCV::tvecModeMask);
            trap_destination = mtvec & RISCV::tvecBaseMask;
            if (tvec_mode == RISCV::tvecMode::Vectored && !isInterrupt) {
                trap_destination += 4*mcause;
            }

            break;

        case RISCV::PrivilegeMode::Supervisor:

            scause = cause;
            if (isInterrupt) {
                scause |= interruptBit;
            }
            sepc = currentFetch->virtualPC;
            if (!isInterrupt) {
                stval = tval;
            }
            supervisorPreviousPrivilege = privilegeMode;
            supervisorPreviousInterruptsEnabled = supervisorInterruptsEnabled;
            supervisorInterruptsEnabled = false;

            // Calculate the destination
            tvec_mode = (RISCV::tvecMode)(stvec & RISCV::tvecModeMask);
            trap_destination = stvec & RISCV::tvecBaseMask;
            if (tvec_mode == RISCV::tvecMode::Vectored && !isInterrupt) {
                trap_destination += 4*scause;
            }

            break;

        case RISCV::PrivilegeMode::User:

            ucause = cause;
            if (isInterrupt) {
                ucause |= interruptBit;
            }
            uepc = currentFetch->virtualPC;
            if (!isInterrupt) {
                utval = tval;
            }
            userPreviousInterruptsEnabled = userInterruptsEnabled;
            userInterruptsEnabled = false;

            // Calculate the destination
            tvec_mode = (RISCV::tvecMode)(utvec & RISCV::tvecModeMask);
            trap_destination = utvec & RISCV::tvecBaseMask;
            if (tvec_mode == RISCV::tvecMode::Vectored && !isInterrupt) {
                trap_destination += 4*ucause;
            }

            break;

        default:
            // fatal("Trap destined for nonsense privilege mode."); // TODO
            trap_destination = 0;
            tvec_mode = RISCV::tvecMode::Vectored;
            break;
        }

        // Actually take the trap.
        privilegeMode = targetPrivilege;
        notifyPrivilegeChanged();
        nextFetchVirtualPC = trap_destination;
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
            machineInterruptsEnabled = machinePreviousInterruptsEnabled;
            privilegeMode = machinePreviousPrivilege;
            machinePreviousInterruptsEnabled = true;
            if (RISCV::vectorHasExtension(extensions, 'U')) {
                machinePreviousPrivilege = RISCV::PrivilegeMode::User;
            } else {
                machinePreviousPrivilege = RISCV::PrivilegeMode::Machine;
            }
            nextFetchVirtualPC = mepc;
        } else if constexpr (trapPrivilege == RISCV::PrivilegeMode::Supervisor) {
            supervisorInterruptsEnabled = supervisorPreviousInterruptsEnabled;
            privilegeMode = supervisorPreviousPrivilege;
            supervisorPreviousInterruptsEnabled = true;
            if (RISCV::vectorHasExtension(extensions, 'U')) {
                supervisorPreviousPrivilege = RISCV::PrivilegeMode::User;
            } else {
                supervisorPreviousPrivilege = RISCV::PrivilegeMode::Machine;
            }
            nextFetchVirtualPC = sepc;
        } else if constexpr (trapPrivilege == RISCV::PrivilegeMode::User) {
            userInterruptsEnabled = userPreviousInterruptsEnabled;
            userPreviousInterruptsEnabled = true;
            nextFetchVirtualPC = uepc;
        } else {
            // fatal("Return from nonsense-privilege-mode trap"); // TODO
        }

        notifyPrivilegeChanged();
    }

};
