#pragma once

#include <RiscV.hpp>
#include <SignedXLEN.hpp>

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mul(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "mul "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value * rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    // TODO - RD should take the lower MXLEN bits of the result. Does it?
    // TODO - signed/unsigned multiply?
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulh(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: mulh" << std::endl;
        return;
    }
    // TODO - implement mulh
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulhsu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: mulhsu" << std::endl;
        return;
    }
    // TODO - implement mulhsu
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulhu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: mulhu" << std::endl;
        return;
    }
    // TODO - implement mulhu
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_div(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "div "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    SXLEN_t rs1_value = state->regs[rs1];
    SXLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? -1 : (XLEN_t)(rs1_value / rs2_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_divu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "divu "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? ~(XLEN_t)0 : (rs1_value / rs2_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_rem(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "rem "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    SXLEN_t rs1_value = state->regs[rs1];
    SXLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? rs1_value : (rs1_value % rs2_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_remu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "remu "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? rs1_value : rs1_value % rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}
