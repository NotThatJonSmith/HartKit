#include <HartState.hpp>

#include <RegisterMSTATUS.hpp>

void HartState::emptyNotifyHandler() {
}

HartState::HartState() : nextFetchVirtualPC(new Register()) {

    notifySoftwareChangedMISA = HartState::emptyNotifyHandler;
    notifySoftwareChangedMSTATUS = HartState::emptyNotifyHandler;
    notifySoftwareChangedSATP = HartState::emptyNotifyHandler;
    notifyPrivilegeChanged = HartState::emptyNotifyHandler;
    notifyInstructionFenceRequested = HartState::emptyNotifyHandler;
    notifyVMFenceRequested = HartState::emptyNotifyHandler;

    privilegeMode = RISCV::PrivilegeMode::Machine;

    // TODO this is a grody hack...
    regs[0].zero = true;

    // TODO MHARTID done with a  hardcoded/hardwired reg type.
    // TODO - BUG - SIE and UIE are masked off views of MIE

    csrs[RISCV::CSRAddress::MSTATUS]    = new RegisterMSTATUS(this);
    csrs[RISCV::CSRAddress::SSTATUS]    = new RegisterSSTATUS(this);
    csrs[RISCV::CSRAddress::USTATUS]    = new RegisterUSTATUS(this);
    csrs[RISCV::CSRAddress::MISA]       = new RegisterMISA(this);
    csrs[RISCV::CSRAddress::SATP]       = new RegisterSATP(this);
    csrs[RISCV::CSRAddress::MCAUSE]     = new Register();
    csrs[RISCV::CSRAddress::SCAUSE]     = new Register();
    csrs[RISCV::CSRAddress::UCAUSE]     = new Register();
    csrs[RISCV::CSRAddress::MEPC]       = new Register();
    csrs[RISCV::CSRAddress::SEPC]       = new Register();
    csrs[RISCV::CSRAddress::UEPC]       = new Register();
    csrs[RISCV::CSRAddress::MIE]        = new Register();
    csrs[RISCV::CSRAddress::SIE]        = new Register();
    csrs[RISCV::CSRAddress::UIE]        = new Register();
    csrs[RISCV::CSRAddress::MIP]        = new Register();
    csrs[RISCV::CSRAddress::SIP]        = new Register();
    csrs[RISCV::CSRAddress::UIP]        = new Register();
    csrs[RISCV::CSRAddress::MEDELEG]    = new Register();
    csrs[RISCV::CSRAddress::SEDELEG]    = new Register();
    csrs[RISCV::CSRAddress::MIDELEG]    = new Register();
    csrs[RISCV::CSRAddress::SIDELEG]    = new Register();
    csrs[RISCV::CSRAddress::MTVEC]      = new Register();
    csrs[RISCV::CSRAddress::STVEC]      = new Register();
    csrs[RISCV::CSRAddress::UTVEC]      = new Register();
    csrs[RISCV::CSRAddress::MTVAL]      = new Register();
    csrs[RISCV::CSRAddress::STVAL]      = new Register();
    csrs[RISCV::CSRAddress::UTVAL]      = new Register();
    csrs[RISCV::CSRAddress::MSCRATCH]   = new Register();
    csrs[RISCV::CSRAddress::SSCRATCH]   = new Register();
    csrs[RISCV::CSRAddress::MHARTID]    = new Register();
    csrs[RISCV::CSRAddress::MCOUNTEREN] = new Register();
    csrs[RISCV::CSRAddress::SCOUNTEREN] = new Register();
    csrs[RISCV::CSRAddress::PMPADDR0]   = new Register();
    csrs[RISCV::CSRAddress::PMPCFG0]    = new Register();
    csrs[RISCV::CSRAddress::TIME]       = new Register();
    csrs[RISCV::CSRAddress::TIMEH]      = new Register();
    csrs[RISCV::CSRAddress::CYCLE]      = new Register();
    csrs[RISCV::CSRAddress::CYCLEH]     = new Register();
    csrs[RISCV::CSRAddress::FCSR]       = new Register();
    csrs[RISCV::CSRAddress::INSTRET]    = new Register();
    csrs[RISCV::CSRAddress::INSTRETH]   = new Register();

}

void HartState::Reset(HartSpec* spec) {
    privilegeMode = RISCV::PrivilegeMode::Machine;
    machineInterruptsEnabled = false;
    modifyMemoryPrivilege = false;
    extensions = spec->maximalExtensions;
    mxlen = spec->WidestXlen(RISCV::Machine, RISCV::XlenMode::XL128);
    csrs[RISCV::CSRAddress::MCAUSE]->Set<__uint32_t>(0);
    csrs[RISCV::CSRAddress::MCAUSE]->Set<__uint64_t>(0);
    csrs[RISCV::CSRAddress::MCAUSE]->Set<__uint128_t>(0);
    nextFetchVirtualPC->Set<__uint32_t>(spec->resetVector.Get<__uint32_t>());
    nextFetchVirtualPC->Set<__uint64_t>(spec->resetVector.Get<__uint64_t>());
    nextFetchVirtualPC->Set<__uint128_t>(spec->resetVector.Get<__uint128_t>());
}

RISCV::XlenMode HartState::GetXLEN() {
    switch (privilegeMode) {
    case RISCV::PrivilegeMode::Machine:
        return mxlen;
        break;
    case RISCV::PrivilegeMode::Supervisor:
        return sxlen;
        break;
    case RISCV::PrivilegeMode::User:
        return uxlen;
        break;
    default:
        return RISCV::XlenMode::None;
        break;
    }
    return RISCV::XlenMode::None;
}

void HartState::ServiceInterrupts() {
    switch (GetXLEN()) {
    case RISCV::XlenMode::XL32:
        ServiceInterrupts<__uint32_t>();
        break;
    case RISCV::XlenMode::XL64:
        ServiceInterrupts<__uint64_t>();
        break;
    case RISCV::XlenMode::XL128:
        ServiceInterrupts<__uint128_t>();
        break;
    case RISCV::XlenMode::None:
        // TODO fatal("ServiceInterrupts called on hart whose operating XLEN mode is None.");
        break;
    default:
        // TODO fatal("ServiceInterrupts called on hart in nonsense XLEN mode.");
        break;
    }
}