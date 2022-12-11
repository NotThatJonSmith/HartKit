#pragma once

#include <RiscV.hpp>
#include <SignedXLEN.hpp>

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mul(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value * rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    // TODO - RD should take the lower MXLEN bits of the result. Does it?
    // TODO - signed/unsigned multiply?
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulh(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    // TODO - implement mulh
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulhsu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    // TODO - implement mulhsu
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_mulhu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    // TODO - implement mulhu
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_div(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    SXLEN_t rs1_value = state->regs[rs1];
    SXLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? -1 : (XLEN_t)(rs1_value / rs2_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_divu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? ~(XLEN_t)0 : (rs1_value / rs2_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_rem(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    SXLEN_t rs1_value = state->regs[rs1];
    SXLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? rs1_value : (rs1_value % rs2_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_remu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs2_value == 0 ? rs1_value : rs1_value % rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}
