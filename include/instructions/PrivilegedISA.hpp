#pragma once

#include <RiscV.hpp>

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_wfi(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // NOP for now. TODO something smarter with the hart's interrupt pins
    if constexpr (out != nullptr) {
        *out << "wfi" << std::endl;
        return;
    }
    state->pc += 4;
}

// TODO URET is only provided if user-mode traps are supported, and should raise an illegal encodingruction otherwise.
template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_uret(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "uret" << std::endl;
        return;
    }
    if (state->privilegeMode < RISCV::PrivilegeMode::User) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0); // TODO access to my encoding...
        return;
    }
    state->template ReturnFromTrap<RISCV::PrivilegeMode::User>();
}

// TODO SRET must be provided if supervisor mode is supported, and should raise an
// illegal encodingruction exception otherwise. SRET should also raise an illegal encodingruction exception when TSR=1
// in mstatus, as described in Section 3.1.6.4. 
template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sret(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "sret" << std::endl;
        return;
    }
    if (state->privilegeMode < RISCV::PrivilegeMode::Supervisor) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0); // TODO access to my encoding...
        return;
    }
    state->template ReturnFromTrap<RISCV::PrivilegeMode::Supervisor>();
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mret(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "mret" << std::endl;
        return;
    }
    if (state->privilegeMode < RISCV::PrivilegeMode::Machine) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0); // TODO access to my encoding...
        return;
    }
    state->template ReturnFromTrap<RISCV::PrivilegeMode::Machine>();
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sfencevma(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "sfence.vma" << std::endl;
        return;
    }
    state->implCallback(HartCallbackArgument::RequestedVMfence);
    state->pc += 4;
}
