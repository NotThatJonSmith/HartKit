#pragma once

#include <Swizzle.hpp>

#include <RiscV.hpp>

#include <SignedXLEN.hpp>

#define RD          ExtendBits::Zero, 11, 7
#define RS1         ExtendBits::Zero, 19, 15
#define RS2         ExtendBits::Zero, 24, 20
#define I_IMM       31, 20
#define S_IMM       31, 25, 11, 7
#define B_IMM       ExtendBits::Sign, 31, 31, 7, 7, 30, 25, 11, 8, 1
#define U_IMM       31, 12, 12
#define J_IMM       31, 31, 19, 12, 20, 20, 30, 21, 1

template<typename XLEN_t>
inline void print_add(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "add "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_sub(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "sub "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_sll(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "sll "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_slt(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "slt "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_sltu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "sltu "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_xor(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "xor "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_sra(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "sra "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_srl(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "srl "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_or(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "or "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_and(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    *out << "and "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t>
inline void print_addi(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "addi "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_slli(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(encoding);
    *out << "slli "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_srli(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(encoding);
    *out << "srli "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_srai(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(encoding);
    *out << "srai "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_slti(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "slti "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_sltiu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "sltiu "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_xori(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "xori "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_ori(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "ori "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_andi(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "andi "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_lui(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, U_IMM>(encoding);
    *out << "lui "
         << RISCV::regName(rd) << ", "
         << (imm >> 12) << std::endl;
}

template<typename XLEN_t>
inline void print_auipc(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, U_IMM>(encoding);
    *out << "auipc "
         << RISCV::regName(rd) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_jal(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, J_IMM>(encoding);
    *out << "jal "
         << RISCV::regName(rd) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_jalr(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "jalr "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_beq(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    *out << "beq "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_bne(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    *out << "bne "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_blt(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    *out << "blt "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_bge(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    *out << "bge "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_bltu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    *out << "bltu "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_bgeu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    *out << "bgeu "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << ", "
         << imm << std::endl;
}

template<typename XLEN_t>
inline void print_lb(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "lb "
         << RISCV::regName(rd) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_lh(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "lh "
         << RISCV::regName(rd) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

// TODO endianness-agnostic impl; for now x86 and RV being both LE save us
template<typename XLEN_t>
inline void print_lw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "lw "
         << RISCV::regName(rd) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_lbu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "lbu "
         << RISCV::regName(rd) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_lhu(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    *out << "lhu "
         << RISCV::regName(rd) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_sb(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(encoding);
    *out << "sb "
         << RISCV::regName(rs2) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_sh(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(encoding);
    *out << "sh "
         << RISCV::regName(rs2) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_sw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(encoding);
    *out << "sw "
         << RISCV::regName(rs2) << ",(" << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_fence(__uint32_t encoding, std::ostream* out) {
    *out << "fence" << std::endl;
}

template<typename XLEN_t>
inline void print_fencei(__uint32_t encoding, std::ostream* out) {
    *out << "fence.i" << std::endl;
}

template<typename XLEN_t>
inline void print_ecall(__uint32_t encoding, std::ostream* out) {
    *out << "ecall" << std::endl;
}

template<typename XLEN_t>
inline void print_ebreak(__uint32_t encoding, std::ostream* out) {
    *out << "ebreak" << std::endl;
}

template<typename XLEN_t>
inline void print_csrrw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    *out << "csrrw "
         << RISCV::regName(rd) << ", "
         << RISCV::csrName(imm) << ", "
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_csrrs(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    *out << "csrrs "
         << RISCV::regName(rd) << ", "
         << RISCV::csrName(imm) << ", "
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_csrrc(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    *out << "csrrc "
         << RISCV::regName(rd) << ", "
         << RISCV::csrName(imm) << ", "
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t>
inline void print_csrrwi(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    *out << "csrrwi "
         << RISCV::regName(rd) << ", "
         << RISCV::csrName(imm) << ", "
         << rs1 << std::endl;
}

template<typename XLEN_t>
inline void print_csrrsi(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    *out << "csrrsi "
         << RISCV::regName(rd) << ", "
         << RISCV::csrName(imm) << ", "
         << rs1 << std::endl;
}

template<typename XLEN_t>
inline void print_csrrci(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    *out << "csrrci "
         << RISCV::regName(rd) << ", "
         << RISCV::csrName(imm) << ", "
         << rs1 << std::endl;
}

template<typename XLEN_t>
inline void print_illegal(__uint32_t encoding, std::ostream* out) {
    *out << "Illegal Instruction" << std::endl;
}

template<typename XLEN_t>
inline void print_reserved(__uint32_t encoding, std::ostream* out) {
    *out << "Reserved Instruction" << std::endl;
}

template<typename XLEN_t>
inline void print_unimplemented(__uint32_t encoding, std::ostream* out) {
    *out << "Unimplemented Instruction" << std::endl;
}
