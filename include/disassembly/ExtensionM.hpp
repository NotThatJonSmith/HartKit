#pragma once

#include <RiscV.hpp>
#include <SignedXLEN.hpp>

template<typename XLEN_t>
inline void print_mul(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "mul "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_mulh(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: mulh" << std::endl;
}

template<typename XLEN_t>
inline void print_mulhsu(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: mulhsu" << std::endl;
}

template<typename XLEN_t>
inline void print_mulhu(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: mulhu" << std::endl;
}

template<typename XLEN_t>
inline void print_div(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "div "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_divu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "divu "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_rem(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "rem "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_remu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "remu "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}
