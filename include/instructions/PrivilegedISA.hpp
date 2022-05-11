#pragma once

#include <RiscV.hpp>

#include <CodePoint.hpp>

// -- wfi --

template<typename XLEN_t>
inline void ex_wfi(Operands operands, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // NOP for now. TODO something smarter with the hart's interrupt pins
}

inline void print_wfi(Operands operands, std::ostream *out) {
    *out << "wfi" << std::endl;
}

constexpr CodePoint inst_wfi = {
    no_operands_from,
    ex_wfi<__uint32_t>,
    ex_wfi<__uint64_t>,
    ex_wfi<__uint128_t>,
    print_wfi,
    4
};

// -- uret --

// TODO URET is only provided if user-mode traps are supported, and should raise an illegal instruction otherwise.
template<typename XLEN_t>
inline void ex_uret(Operands operands, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if (state->privilegeMode < RISCV::PrivilegeMode::User) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0); // TODO access to my encoding...
        return;
    }
    state->template ReturnFromTrap<RISCV::PrivilegeMode::User>();
}

inline void print_uret(Operands operands, std::ostream *out) {
    *out << "uret" << std::endl;
}

constexpr CodePoint inst_uret = {
    no_operands_from,
    ex_uret<__uint32_t>,
    ex_uret<__uint64_t>,
    ex_uret<__uint128_t>,
    print_uret,
    4
};

// -- sret --

// TODO SRET must be provided if supervisor mode is supported, and should raise an
// illegal instruction exception otherwise. SRET should also raise an illegal instruction exception when TSR=1
// in mstatus, as described in Section 3.1.6.4. 
template<typename XLEN_t>
inline void ex_sret(Operands operands, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if (state->privilegeMode < RISCV::PrivilegeMode::Supervisor) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0); // TODO access to my encoding...
        return;
    }
    state->template ReturnFromTrap<RISCV::PrivilegeMode::Supervisor>();
}

inline void print_sret(Operands operands, std::ostream *out) {
    *out << "sret" << std::endl;
}

constexpr CodePoint inst_sret = {
    no_operands_from,
    ex_sret<__uint32_t>,
    ex_sret<__uint64_t>,
    ex_sret<__uint128_t>,
    print_sret,
    4
};

// -- mret --

template<typename XLEN_t>
inline void ex_mret(Operands operands, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if (state->privilegeMode < RISCV::PrivilegeMode::Machine) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0); // TODO access to my encoding...
        return;
    }
    state->template ReturnFromTrap<RISCV::PrivilegeMode::Machine>();
}

inline void print_mret(Operands operands, std::ostream *out) {
    *out << "mret" << std::endl;
}

constexpr CodePoint inst_mret = {
    no_operands_from,
    ex_mret<__uint32_t>,
    ex_mret<__uint64_t>,
    ex_mret<__uint128_t>,
    print_mret,
    4
};

// -- sfencevma --

template<typename XLEN_t>
inline void ex_sfencevma(Operands operands, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    state->notifyVMFenceRequested();
}

inline void print_sfencevma(Operands operands, std::ostream *out) {
    *out << "sfence.vma" << std::endl;
}

constexpr CodePoint inst_sfencevma = {
    no_operands_from,
    ex_sfencevma<__uint32_t>,
    ex_sfencevma<__uint64_t>,
    ex_sfencevma<__uint128_t>,
    print_sfencevma,
    4
};
