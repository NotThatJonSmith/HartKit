#pragma once

#include <instructions/BaseISA.hpp>

#pragma mark -- Common Compressed ISA Fields --

#define C_FUNCT2    ExtendBits::Zero, 6, 5
#define C_FUNCT3    ExtendBits::Zero, 15, 13
#define C_FUNCT4    ExtendBits::Zero, 15, 12
#define C_FUNCT6    ExtendBits::Zero, 15, 10
#define CR_RD_RS1   ExtendBits::Zero, 11, 7
#define CR_RS2      ExtendBits::Zero, 6, 2
#define CS_RD_RS1   ExtendBits::Zero, 11, 7
#define CS_RS1X     ExtendBits::Zero, 9, 7
#define CS_RS2X     ExtendBits::Zero, 4, 2
#define CSS_RS2     ExtendBits::Zero, 6, 2
#define CI_RD_RS1   ExtendBits::Zero, 11, 7
#define CIW_RDX     ExtendBits::Zero, 4, 2
#define CL_RDX      ExtendBits::Zero, 4, 2
#define CL_RS1X     ExtendBits::Zero, 9, 7
#define CA_RS2X     ExtendBits::Zero, 4, 2
#define CA_RDX_RS1X ExtendBits::Zero, 9, 7
#define CB_RDX_RS1X ExtendBits::Zero, 9, 7
#define CIW_RDX_RS1X ExtendBits::Zero, 11, 7
#define CB_SHAMT    ExtendBits::Zero, 12, 12, 6, 2
#define CI_SHAMT   ExtendBits::Zero, 12, 12, 6, 2

inline Operands operands_cregop(__uint32_t inst) {
    Operands result;
    result.R.rd = swizzle<__uint32_t, CA_RDX_RS1X>(inst)+8;
    result.R.rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(inst)+8;
    result.R.rs2 = swizzle<__uint32_t, CA_RS2X>(inst)+8;
    return result;
}

inline Operands operands_cimmop(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CB_RDX_RS1X>(inst);
    result.I.rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(inst);
    result.I.imm.u = swizzle<__uint32_t, CB_RDX_RS1X>(inst);
    return result;
}

#pragma mark -- caddi4spn --

inline Operands operands_caddi4spn(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CIW_RDX>(inst)+8;
    result.I.rs1 = 2;
    result.I.imm.s = swizzle<__uint32_t, ExtendBits::Zero, 10, 7, 12, 11, 5, 5, 6, 6, 2>(inst);
    return result;
}

inline void print_caddi4spn(Operands operands, std::ostream *out) {
    *out << "(C.ADDI4SPN) ";
    print_addi(operands, out);
}

constexpr CodePoint inst_caddi4spn = {
    operands_caddi4spn,
    ex_addi<__uint32_t>,
    ex_addi<__uint64_t>,
    ex_addi<__uint128_t>,
    print_caddi4spn,
    2
};

#pragma mark -- clw --

inline Operands operands_clw(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CL_RDX>(inst)+8;
    result.I.rs1 = swizzle<__uint32_t, CL_RS1X>(inst)+8;
    // Yes, this is correct. It's zero extended into the signed immediate.
    result.I.imm.s = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(inst);
    return result;
}

inline void print_clw(Operands operands, std::ostream *out) {
    *out << "(C.LW) ";
    print_lw(operands, out);
}

constexpr CodePoint inst_clw = {
    operands_clw,
    ex_lw<__uint32_t>,
    ex_lw<__uint64_t>,
    ex_lw<__uint128_t>,
    print_clw,
    2
};

#pragma mark -- csw --

inline Operands operands_csw(__uint32_t inst) {
    Operands result;
    result.S.rs1 = swizzle<__uint32_t, CS_RS1X>(inst)+8;
    result.S.rs2 = swizzle<__uint32_t, CS_RS2X>(inst)+8;
    result.S.imm.s = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(inst);
    return result;
}

inline void print_csw(Operands operands, std::ostream *out) {
    *out << "(C.SW) ";
    print_sw(operands, out);
}

constexpr CodePoint inst_csw = {
    operands_csw,
    ex_sw<__uint32_t>,
    ex_sw<__uint64_t>,
    ex_sw<__uint128_t>,
    print_csw,
    2
};

#pragma mark -- caddi --

inline Operands operands_caddi(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.I.rs1 = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.I.imm.s = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(inst);
    return result;
}

inline void print_caddi(Operands operands, std::ostream *out) {
    *out << "(C.ADDI) ";
    print_addi(operands, out);
}

constexpr CodePoint inst_caddi = {
    operands_caddi,
    ex_addi<__uint32_t>,
    ex_addi<__uint64_t>,
    ex_addi<__uint128_t>,
    print_caddi,
    2
};

#pragma mark -- cjal --

inline Operands operands_cjal(__uint32_t inst) {
    Operands result;
    result.U.rd = 1;
    result.U.imm.s = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(inst);
    return result;
}

inline void print_cjal(Operands operands, std::ostream *out) {
    *out << "(C.JAL) ";
    print_jal(operands, out);
}

constexpr CodePoint inst_cjal = {
    operands_cjal,
    ex_jal<__uint32_t>,
    ex_jal<__uint64_t>,
    ex_jal<__uint128_t>,
    print_cjal,
    2
};

#pragma mark -- cli --

inline Operands operands_cli(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.I.rs1 = 0;
    result.I.imm.s = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(inst);
    return result;
}

inline void print_cli(Operands operands, std::ostream *out) {
    *out << "(C.LI) ";
    print_addi(operands, out);
}

constexpr CodePoint inst_cli = {
    operands_cli,
    ex_addi<__uint32_t>,
    ex_addi<__uint64_t>,
    ex_addi<__uint128_t>,
    print_cli,
    2
};

#pragma mark -- clui --

inline Operands operands_clui(__uint32_t inst) {
    Operands result;
    result.U.rd = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.U.imm.u = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2, 12>(inst);
    return result;
}

inline void print_clui(Operands operands, std::ostream *out) {
    *out << "(C.LUI) ";
    print_lui(operands, out);
}

constexpr CodePoint inst_clui = {
    operands_clui,
    ex_lui<__uint32_t>,
    ex_lui<__uint64_t>,
    ex_lui<__uint128_t>,
    print_clui,
    2
};

#pragma mark -- caddi16sp --

inline Operands operands_caddi16sp(__uint32_t inst) {
    Operands result;
    result.I.rd = 2;
    result.I.rs1 = 2;
    result.I.imm.s = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 4, 3, 5, 5, 2, 2, 6, 6, 4>(inst);
    return result;
}

inline void print_caddi16sp(Operands operands, std::ostream *out) {
    *out << "(C.ADDI16SP) ";
    print_addi(operands, out);
}

constexpr CodePoint inst_caddi16sp = {
    operands_caddi16sp,
    ex_addi<__uint32_t>,
    ex_addi<__uint64_t>,
    ex_addi<__uint128_t>,
    print_caddi16sp,
    2
};

#pragma mark -- csub --

inline void print_csub(Operands operands, std::ostream *out) {
    *out << "(C.SUB) ";
    print_sub(operands, out);
}

constexpr CodePoint inst_csub = {
    operands_cregop,
    ex_sub<__uint32_t>,
    ex_sub<__uint64_t>,
    ex_sub<__uint128_t>,
    print_csub,
    2
};

#pragma mark -- cxor --

inline void print_cxor(Operands operands, std::ostream *out) {
    *out << "(C.XOR) ";
    print_xor(operands, out);
}

constexpr CodePoint inst_cxor = {
    operands_cregop,
    ex_xor<__uint32_t>,
    ex_xor<__uint64_t>,
    ex_xor<__uint128_t>,
    print_cxor,
    2
};

#pragma mark -- cor --

inline void print_cor(Operands operands, std::ostream *out) {
    *out << "(C.OR) ";
    print_or(operands, out);
}

constexpr CodePoint inst_cor = {
    operands_cregop,
    ex_or<__uint32_t>,
    ex_or<__uint64_t>,
    ex_or<__uint128_t>,
    print_cor,
    2
};

#pragma mark -- cand --

inline void print_cand(Operands operands, std::ostream *out) {
    *out << "(C.AND) ";
    print_and(operands, out);
}

constexpr CodePoint inst_cand = {
    operands_cregop,
    ex_and<__uint32_t>,
    ex_and<__uint64_t>,
    ex_and<__uint128_t>,
    print_cand,
    2
};

#pragma mark -- cj --

inline Operands operands_cj(__uint32_t inst) {
    Operands result;
    result.U.imm.s = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(inst);
    result.U.rd = 0;
    return result;
}

inline void print_cj(Operands operands, std::ostream *out) {
    *out << "(C.J) ";
    print_jal(operands, out);
}

constexpr CodePoint inst_cj = {
    operands_cj,
    ex_jal<__uint32_t>,
    ex_jal<__uint64_t>,
    ex_jal<__uint128_t>,
    print_cj,
    2
};

#pragma mark -- cbeqz --

inline Operands operands_cbeqz(__uint32_t inst) {
    Operands result;
    result.S.imm.s = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(inst);
    result.S.rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.S.rs2 = 0;
    return result;
}

inline void print_cbeqz(Operands operands, std::ostream *out) {
    *out << "(C.BEQZ) ";
    print_beq(operands, out);
}

constexpr CodePoint inst_cbeqz = {
    operands_cbeqz,
    ex_beq<__uint32_t>,
    ex_beq<__uint64_t>,
    ex_beq<__uint128_t>,
    print_cbeqz,
    2
};

#pragma mark -- cbnez --

inline Operands operands_cbnez(__uint32_t inst) {
    Operands result;
    result.S.rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.S.rs2 = 0;
    result.S.imm.s = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(inst);
    return result;
}

inline void print_cbnez(Operands operands, std::ostream *out) {
    *out << "(C.BNEZ) ";
    print_bne(operands, out);
}

constexpr CodePoint inst_cbnez = {
    operands_cbnez,
    ex_bne<__uint32_t>,
    ex_bne<__uint64_t>,
    ex_bne<__uint128_t>,
    print_cbnez,
    2
};

#pragma mark -- candi --

inline Operands operands_candi(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.I.rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.I.imm.s = swizzle<__int32_t, ExtendBits::Sign, 12, 12, 6, 2>(inst);
    return result;
}

inline void print_candi(Operands operands, std::ostream *out) {
    *out << "(C.ANDI) ";
    print_andi(operands, out);
}

constexpr CodePoint inst_candi = {
    operands_candi,
    ex_andi<__uint32_t>,
    ex_andi<__uint64_t>,
    ex_andi<__uint128_t>,
    print_candi,
    2
};

#pragma mark -- cslli --

inline Operands operands_cslli(__uint32_t inst) {
    Operands result;
    result.I.imm.u = swizzle<__uint32_t, CI_SHAMT>(inst);
    result.I.rd = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.I.rs1 = result.I.rd;
    return result;
}

inline Operands operands_cslli_128(__uint32_t inst) {
    Operands result;
    result.I.imm.u = swizzle<__uint32_t, CI_SHAMT>(inst);
    if (result.I.imm.u == 0)
        result.I.imm.u = 64;
    result.I.rd = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.I.rs1 = result.I.rd;
    return result;
}

inline void print_cslli(Operands operands, std::ostream *out) {
    *out << "(C.SLLI) ";
    print_slli(operands, out);
}

constexpr CodePoint inst_cslli = {
    operands_cslli,
    ex_slli<__uint32_t>,
    ex_slli<__uint64_t>,
    ex_slli<__uint128_t>,
    print_cslli,
    2
};

constexpr CodePoint inst_cslli_128 = {
    operands_cslli_128,
    ex_slli<__uint32_t>,
    ex_slli<__uint64_t>,
    ex_slli<__uint128_t>,
    print_cslli,
    2
};

#pragma mark -- clwsp --

inline Operands operands_clwsp(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.I.rs1 = 2;
    result.I.imm.s = swizzle<__uint32_t, ExtendBits::Zero, 3, 2, 12, 12, 6, 4, 2>(inst);
    return result;
}

inline void print_clwsp(Operands operands, std::ostream *out) {
    *out << "(C.LWSP) ";
    print_lw(operands, out);
}

constexpr CodePoint inst_clwsp = {
    operands_clwsp,
    ex_lw<__uint32_t>,
    ex_lw<__uint64_t>,
    ex_lw<__uint128_t>,
    print_clwsp,
    2
};

#pragma mark -- cswsp --

inline Operands operands_cswsp(__uint32_t inst) {
    Operands result;
    result.S.rs1 = 2;
    result.S.rs2 = swizzle<__uint32_t, CSS_RS2>(inst);
    result.S.imm.s = swizzle<__uint32_t, ExtendBits::Zero, 8, 7, 12, 9, 2>(inst);
    return result;
}

inline void print_cswsp(Operands operands, std::ostream *out) {
    *out << "(C.SWSP) ";
    print_sw(operands, out);
}

constexpr CodePoint inst_cswsp = {
    operands_cswsp,
    ex_sw<__uint32_t>,
    ex_sw<__uint64_t>,
    ex_sw<__uint128_t>,
    print_cswsp,
    2
};

#pragma mark -- cjalr --

inline Operands operands_cjalr(__uint32_t inst) {
    Operands result;
    result.I.imm.s = 0;
    result.I.rd = 1;
    result.I.rs1 = swizzle<__uint32_t, CI_RD_RS1>(inst);
    return result;
}

inline void print_cjalr(Operands operands, std::ostream *out) {
    *out << "(C.JALR) ";
    print_jalr(operands, out);
}

constexpr CodePoint inst_cjalr = {
    operands_cjalr,
    ex_jalr<__uint32_t>,
    ex_jalr<__uint64_t>,
    ex_jalr<__uint128_t>,
    print_cjalr,
    2
};

#pragma mark -- cadd --

inline Operands operands_cadd(__uint32_t inst) {
    Operands result;
    result.R.rd = swizzle<__uint32_t, CR_RD_RS1>(inst);
    result.R.rs1 = swizzle<__uint32_t, CR_RD_RS1>(inst);
    result.R.rs2 = swizzle<__uint32_t, CR_RS2>(inst);
    return result;
}

inline void print_cadd(Operands operands, std::ostream *out) {
    *out << "(C.ADD) ";
    print_add(operands, out);
}

constexpr CodePoint inst_cadd = {
    operands_cadd,
    ex_add<__uint32_t>,
    ex_add<__uint64_t>,
    ex_add<__uint128_t>,
    print_cadd,
    2
};

#pragma mark -- cjr --

inline Operands operands_cjr(__uint32_t inst) {
    Operands result;
    result.I.rs1 = swizzle<__uint32_t, CI_RD_RS1>(inst);
    result.I.imm.s = 0;
    result.I.rd = 0;
    return result;
}

inline void print_cjr(Operands operands, std::ostream *out) {
    *out << "(C.JR) ";
    print_jalr(operands, out);
}

constexpr CodePoint inst_cjr = {
    operands_cjr,
    ex_jalr<__uint32_t>,
    ex_jalr<__uint64_t>,
    ex_jalr<__uint128_t>,
    print_cjr,
    2
};

#pragma mark -- cmv --

inline Operands operands_cmv(__uint32_t inst) {
    Operands result;
    result.R.rd = swizzle<__uint32_t, CR_RD_RS1>(inst);
    result.R.rs1 = 0;
    result.R.rs2 = swizzle<__uint32_t, CR_RS2>(inst);
    return result;
}

inline void print_cmv(Operands operands, std::ostream *out) {
    *out << "(C.MV) ";
    print_add(operands, out);
}

constexpr CodePoint inst_cmv = {
    operands_cmv,
    ex_add<__uint32_t>,
    ex_add<__uint64_t>,
    ex_add<__uint128_t>,
    print_cmv,
    2
};

#pragma mark -- cebreak --

inline void print_cebreak(Operands operands, std::ostream *out) {
    *out << "(C.EBREAK) ";
    print_ebreak(operands, out);
}

constexpr CodePoint inst_cebreak = {
    no_operands_from,
    ex_ebreak<__uint32_t>,
    ex_ebreak<__uint64_t>,
    ex_ebreak<__uint128_t>,
    print_cebreak,
    2
};

#pragma mark -- csrli --

inline Operands operands_csrli_csrai_32_64(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.I.rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.I.imm.u = swizzle<__uint32_t, CB_SHAMT>(inst);
    return result;
}

inline void print_csrli(Operands operands, std::ostream *out) {
    *out << "(C.SRLI) ";
    print_srli(operands, out);
}

constexpr CodePoint inst_csrli = {
    operands_csrli_csrai_32_64,
    ex_srli<__uint32_t>,
    ex_srli<__uint64_t>,
    ex_srli<__uint128_t>,
    print_csrli,
    2
};

#pragma mark -- csrli_128 --

inline Operands operands_csrli_csrai_128(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.I.rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(inst)+8;
    result.I.imm.u = swizzle<__uint32_t, CB_SHAMT>(inst);
    if (result.I.imm.u == 0)
        result.I.imm.u = 64;
    return result;
}

constexpr CodePoint inst_csrli_128 = {
    operands_csrli_csrai_128,
    ex_srli<__uint32_t>,
    ex_srli<__uint64_t>,
    ex_srli<__uint128_t>,
    print_csrli,
    2
};

#pragma mark -- csrai --

inline void print_csrai(Operands operands, std::ostream *out) {
    *out << "(C.SRAI) ";
    print_srai(operands, out);
}

constexpr CodePoint inst_csrai = {
    operands_csrli_csrai_32_64,
    ex_srai<__uint32_t>,
    ex_srai<__uint64_t>,
    ex_srai<__uint128_t>,
    print_csrai,
    2
};

#pragma mark -- csrai_128 --

constexpr CodePoint inst_csrai_128 = {
    operands_csrli_csrai_128,
    ex_srai<__uint32_t>,
    ex_srai<__uint64_t>,
    ex_srai<__uint128_t>,
    print_csrai,
    2
};

#pragma mark -- illegal compressed instruction --

constexpr CodePoint illegal2ByteInstruction = {
    no_operands_from,
    ex_illegal<__uint32_t>,
    ex_illegal<__uint64_t>,
    ex_illegal<__uint128_t>,
    print_illegal,
    2
};

#pragma mark -- reserved compressed instruction --

constexpr CodePoint reserved2ByteInstruction = {
    no_operands_from,
    ex_illegal<__uint32_t>,
    ex_illegal<__uint64_t>,
    ex_illegal<__uint128_t>,
    print_illegal,
    2
};

#pragma mark -- unimplemented compressed instruction --

constexpr CodePoint unimplemented2ByteInstruction = {
    no_operands_from,
    ex_illegal<__uint32_t>,
    ex_illegal<__uint64_t>,
    ex_illegal<__uint128_t>,
    print_illegal,
    2
};
