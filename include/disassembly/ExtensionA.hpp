#pragma once

#include <RiscV.hpp>

template<typename XLEN_t>
inline void print_lrw(__uint32_t encoding, std::ostream* out) {
    // Does rd have to contain 0? If nonzero, is it an illegal instruction?
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "lrw "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_lrd(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: lrd" << std::endl;
}

template<typename XLEN_t>
inline void print_scw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "scw "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_scd(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: scd" << std::endl;
}

template<typename XLEN_t>
inline void print_amoaddw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "amoadd.w "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
        return;
}

template<typename XLEN_t>
inline void print_amoaddd(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amoaddd" << std::endl;
}

template<typename XLEN_t>
inline void print_amoswapw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "amoswap.w "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_amoswapd(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amoswapd" << std::endl;
}

template<typename XLEN_t>
inline void print_amoxorw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "amoxor.w "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_amoxord(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amoxord" << std::endl;
}

template<typename XLEN_t>
inline void print_amoorw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "amoor.w "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_amoord(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amoord" << std::endl;
}

template<typename XLEN_t>
inline void print_amoandw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "amoand.w x"
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_amoandd(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amoandd" << std::endl;
}

template<typename XLEN_t>
inline void print_amominw(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amominw" << std::endl;
}

template<typename XLEN_t>
inline void print_amomind(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amomind" << std::endl;
}

template<typename XLEN_t>
inline void print_amomaxw(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amomaxw" << std::endl;
}

template<typename XLEN_t>
inline void print_amomaxd(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amomaxd" << std::endl;
}

template<typename XLEN_t>
inline void print_amominuw(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amominuw" << std::endl;
}

template<typename XLEN_t>
inline void print_amominud(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amominud" << std::endl;
}

template<typename XLEN_t>
inline void print_amomaxuw(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amomaxuw"
         << std::endl;
}

template<typename XLEN_t>
inline void print_amomaxud(__uint32_t encoding, std::ostream* out) {
    *out << "WARNING: instruction not implemented: amomaxud" << std::endl;
}
