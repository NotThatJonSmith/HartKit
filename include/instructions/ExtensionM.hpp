#pragma once

#include <RiscV.hpp>
#include <SignedXLEN.hpp>

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mul(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
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
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulh(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: mulh" << std::endl;
        return;
    }
    // TODO - implement mulh
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulhsu(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: mulhsu" << std::endl;
        return;
    }
    // TODO - implement mulhsu
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulhu(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: mulhu" << std::endl;
        return;
    }
    // TODO - implement mulhu
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_div(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
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
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_divu(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
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
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_rem(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
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
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_remu(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
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
}
