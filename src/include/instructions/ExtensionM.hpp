#pragma once

#include <RiscV.hpp>

#include <CodePoint.hpp>
#include <Operands.hpp>

// -- mul --

template<typename XLEN_t>
inline void ex_mul(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value * rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
    // TODO - RD should take the lower MXLEN bits of the result. Does it?
    // TODO - signed/unsigned multiply?
}

inline void print_mul(Operands operands, std::ostream *out) {
    *out << "mul "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_mul = {
    r_operands_from,
    ex_mul<__uint32_t>,
    ex_mul<__uint64_t>,
    ex_mul<__uint128_t>,
    print_mul,
    4
};

// -- mulh --

template<typename XLEN_t>
inline void ex_mulh(Operands operands, HartState *state) {
    // TODO - implement mulh
}

inline void print_mulh(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: mulh" << std::endl;
}

constexpr CodePoint inst_mulh = {
    r_operands_from,
    ex_mulh<__uint32_t>,
    ex_mulh<__uint64_t>,
    ex_mulh<__uint128_t>,
    print_mulh,
    4
};

// -- mulhsu --

template<typename XLEN_t>
inline void ex_mulhsu(Operands operands, HartState *state) {
    // TODO - implement mulhsu
}

inline void print_mulhsu(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: mulhsu" << std::endl;
}

constexpr CodePoint inst_mulhsu = {
    r_operands_from,
    ex_mulhsu<__uint32_t>,
    ex_mulhsu<__uint64_t>,
    ex_mulhsu<__uint128_t>,
    print_mulhsu,
    4
};

// -- mulhu --

template<typename XLEN_t>
inline void ex_mulhu(Operands operands, HartState *state) {
    // TODO - implement mulhu
}

inline void print_mulhu(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: mulhu" << std::endl;
}

constexpr CodePoint inst_mulhu = {
    r_operands_from,
    ex_mulhu<__uint32_t>,
    ex_mulhu<__uint64_t>,
    ex_mulhu<__uint128_t>,
    print_mulhu,
    4
};

// -- div --

template<typename XLEN_t>
inline void ex_div(Operands operands, HartState *state) {
    typedef typename std::make_signed<XLEN_t>::type SXLEN_t;
    SXLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    SXLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs2_value == 0 ? -1 : (XLEN_t)(rs1_value / rs2_value);
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_div(Operands operands, std::ostream *out) {
    *out << "div "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_div = {
    r_operands_from,
    ex_div<__uint32_t>,
    ex_div<__uint64_t>,
    ex_div<__uint128_t>,
    print_div,
    4
};

// -- divu --

template<typename XLEN_t>
inline void ex_divu(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs2_value == 0 ? ~(XLEN_t)0 : (rs1_value / rs2_value);
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_divu(Operands operands, std::ostream *out) {
    *out << "divu "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_divu = {
    r_operands_from,
    ex_divu<__uint32_t>,
    ex_divu<__uint64_t>,
    ex_divu<__uint128_t>,
    print_divu,
    4
};

// -- rem --

template<typename XLEN_t>
inline void ex_rem(Operands operands, HartState *state) {
    typedef typename std::make_signed<XLEN_t>::type SXLEN_t;
    SXLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    SXLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs2_value == 0 ? rs1_value : (rs1_value % rs2_value);
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_rem(Operands operands, std::ostream *out) {
    *out << "rem "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_rem = {
    r_operands_from,
    ex_rem<__uint32_t>,
    ex_rem<__uint64_t>,
    ex_rem<__uint128_t>,
    print_rem,
    4
};

// -- remu --

template<typename XLEN_t>
inline void ex_remu(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs2_value == 0 ? rs1_value : rs1_value % rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_remu(Operands operands, std::ostream *out) {
    *out << "remu "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_remu = {
    r_operands_from,
    ex_remu<__uint32_t>,
    ex_remu<__uint64_t>,
    ex_remu<__uint128_t>,
    print_remu,
    4
};
