#pragma once

#include <Swizzle.hpp>

#include <RiscV.hpp>

#include <CodePoint.hpp>
#include <Operands.hpp>
#include <HartState.hpp>

#include <SignedXLEN.hpp>

// -- Common Base ISA Fields --

#define RD          ExtendBits::Zero, 11, 7
#define RS1         ExtendBits::Zero, 19, 15
#define RS2         ExtendBits::Zero, 24, 20
#define I_IMM       31, 20
#define S_IMM       31, 25, 11, 7
#define B_IMM       ExtendBits::Sign, 31, 31, 7, 7, 30, 25, 11, 8, 1
#define U_IMM       31, 12, 12
#define J_IMM       31, 31, 19, 12, 20, 20, 30, 21, 1

// -- add --

inline Operands r_operands_from(__uint32_t inst) {
    Operands result;
    result.R.rd = swizzle<__uint32_t, RD>(inst);
    result.R.rs1 = swizzle<__uint32_t, RS1>(inst);
    result.R.rs2 = swizzle<__uint32_t, RS2>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_add(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value + rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_add(Operands operands, std::ostream *out) {
    *out << "add "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_add = {
    r_operands_from,
    ex_add<__uint32_t>,
    ex_add<__uint64_t>,
    ex_add<__uint128_t>,
    print_add,
    4
};

// -- sub --

template<typename XLEN_t>
inline void ex_sub(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value - rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_sub(Operands operands, std::ostream *out) {
    *out << "sub "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_sub = {
    r_operands_from,
    ex_sub<__uint32_t>,
    ex_sub<__uint64_t>,
    ex_sub<__uint128_t>,
    print_sub,
    4
};

// -- sll --

template<typename XLEN_t>
inline void ex_sll(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value << rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_sll(Operands operands, std::ostream *out) {
    *out << "sll "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_sll = {
    r_operands_from,
    ex_sll<__uint32_t>,
    ex_sll<__uint64_t>,
    ex_sll<__uint128_t>,
    print_sll,
    4
};

// -- slt --

template<typename XLEN_t>
inline void ex_slt(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    XLEN_t rd_value = rs1_value < rs2_value ? 1 : 0;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_slt(Operands operands, std::ostream *out) {
    *out << "slt "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_slt = {
    r_operands_from,
    ex_slt<__uint32_t>,
    ex_slt<__uint64_t>,
    ex_slt<__uint128_t>,
    print_slt,
    4
};

// -- sltu --

template<typename XLEN_t>
inline void ex_sltu(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value < rs2_value ? 1 : 0;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_sltu(Operands operands, std::ostream *out) {
    *out << "sltu "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_sltu = {
    r_operands_from,
    ex_sltu<__uint32_t>,
    ex_sltu<__uint64_t>,
    ex_sltu<__uint128_t>,
    print_sltu,
    4
};

// -- xor --

template<typename XLEN_t>
inline void ex_xor(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value ^ rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_xor(Operands operands, std::ostream *out) {
    *out << "xor "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_xor = {
    r_operands_from,
    ex_xor<__uint32_t>,
    ex_xor<__uint64_t>,
    ex_xor<__uint128_t>,
    print_xor,
    4
};

// -- sra --

template<typename XLEN_t>
inline void ex_sra(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value >> rs2_value;
    constexpr XLEN_t xlen_bits = sizeof(XLEN_t) * 8;
    if (rs1_value & ((XLEN_t)1 << (xlen_bits - 1))) {
        rd_value |= ((1 << rs2_value)-1) << (xlen_bits-rs2_value);
    }
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_sra(Operands operands, std::ostream *out) {
    *out << "sra "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_sra = {
    r_operands_from,
    ex_sra<__uint32_t>,
    ex_sra<__uint64_t>,
    ex_sra<__uint128_t>,
    print_sra,
    4
};

// -- srl --

template<typename XLEN_t>
inline void ex_srl(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value >> rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_srl(Operands operands, std::ostream *out) {
    *out << "srl "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_srl = {
    r_operands_from,
    ex_srl<__uint32_t>,
    ex_srl<__uint64_t>,
    ex_srl<__uint128_t>,
    print_srl,
    4
};

// -- or --

template<typename XLEN_t>
inline void ex_or(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value | rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_or(Operands operands, std::ostream *out) {
    *out << "or "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_or = {
    r_operands_from,
    ex_or<__uint32_t>,
    ex_or<__uint64_t>,
    ex_or<__uint128_t>,
    print_or,
    4
};

// -- and --

template<typename XLEN_t>
inline void ex_and(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value & rs2_value;
    state->regs[operands.R.rd].Write<XLEN_t>(rd_value);
}

inline void print_and(Operands operands, std::ostream *out) {
    *out << "and "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_and = {
    r_operands_from,
    ex_and<__uint32_t>,
    ex_and<__uint64_t>,
    ex_and<__uint128_t>,
    print_and,
    4
};

// -- addi --

inline Operands i_operands_from(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, RD>(inst);
    result.I.rs1 = swizzle<__uint32_t, RS1>(inst);
    result.I.imm.s = (int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_addi(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value = operands.I.imm.s;
    XLEN_t rd_value = rs1_value + imm_value;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_addi(Operands operands, std::ostream *out) {
    *out << "addi "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.s << std::endl;
}

constexpr CodePoint inst_addi = {
    i_operands_from,
    ex_addi<__uint32_t>,
    ex_addi<__uint64_t>,
    ex_addi<__uint128_t>,
    print_addi,
    4
};

// -- slli --

inline Operands i_shift_operands_from(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, RD>(inst);
    result.I.rs1 = swizzle<__uint32_t, RS1>(inst);
    result.I.imm.u = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_slli(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value << operands.I.imm.u;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_slli(Operands operands, std::ostream *out) {
    *out << "slli "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.u << std::endl;
}

constexpr CodePoint inst_slli = {
    i_shift_operands_from,
    ex_slli<__uint32_t>,
    ex_slli<__uint64_t>,
    ex_slli<__uint128_t>,
    print_slli,
    4
};

// -- srli --

template<typename XLEN_t>
inline void ex_srli(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    XLEN_t rd_value = rs1_value >> operands.I.imm.u;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_srli(Operands operands, std::ostream *out) {
    *out << "srli "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.u << std::endl;
}

constexpr CodePoint inst_srli = {
    i_shift_operands_from,
    ex_srli<__uint32_t>,
    ex_srli<__uint64_t>,
    ex_srli<__uint128_t>,
    print_srli,
    4
};

// -- srai --

template<typename XLEN_t>
inline void ex_srai(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    __uint16_t imm_value = operands.I.imm.u;
    XLEN_t rd_value = rs1_value >> imm_value;
    // Spec says: the original sign bit is copied into the vacated upper bits
    if (rs1_value & ((XLEN_t)1 << ((sizeof(XLEN_t)*8)-1)))
        rd_value |= (XLEN_t)((1 << imm_value)-1) << ((sizeof(XLEN_t)*8)-imm_value);
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_srai(Operands operands, std::ostream *out) {
    *out << "srai "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.u << std::endl;
}

constexpr CodePoint inst_srai = {
    i_shift_operands_from,
    ex_srai<__uint32_t>,
    ex_srai<__uint64_t>,
    ex_srai<__uint128_t>,
    print_srai,
    4
};

// -- slti --

template<typename XLEN_t>
inline void ex_slti(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    SXLEN_t imm_value = operands.I.imm.s;
    XLEN_t rd_value = rs1_value < imm_value ? 1 : 0;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_slti(Operands operands, std::ostream *out) {
    *out << "slti "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.u << std::endl;
}

constexpr CodePoint inst_slti = {
    i_operands_from,
    ex_slti<__uint32_t>,
    ex_slti<__uint64_t>,
    ex_slti<__uint128_t>,
    print_slti,
    4
};

// -- sltiu --

template<typename XLEN_t>
inline void ex_sltiu(Operands operands, HartState *state) {
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    XLEN_t imm_value = operands.I.imm.u;
    XLEN_t rd_value = rs1_value < imm_value ? 1 : 0;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value); 
}

inline void print_sltiu(Operands operands, std::ostream *out) {
    *out << "sltiu "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.u << std::endl;
}

constexpr CodePoint inst_sltiu = {
    i_operands_from,
    ex_sltiu<__uint32_t>,
    ex_sltiu<__uint64_t>,
    ex_sltiu<__uint128_t>,
    print_sltiu,
    4
};

// -- xori --

template<typename XLEN_t>
inline void ex_xori(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value_signed = operands.I.imm.s;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value ^ imm_value;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_xori(Operands operands, std::ostream *out) {
    *out << "xori "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.u << std::endl;
}

constexpr CodePoint inst_xori = {
    i_operands_from,
    ex_xori<__uint32_t>,
    ex_xori<__uint64_t>,
    ex_xori<__uint128_t>,
    print_xori,
    4
};

// -- ori --

template<typename XLEN_t>
inline void ex_ori(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value_signed = operands.I.imm.s;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value | imm_value;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_ori(Operands operands, std::ostream *out) {
    *out << "ori "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.u << std::endl;
}

constexpr CodePoint inst_ori = {
    i_operands_from,
    ex_ori<__uint32_t>,
    ex_ori<__uint64_t>,
    ex_ori<__uint128_t>,
    print_ori,
    4
};

// -- andi --

template<typename XLEN_t>
inline void ex_andi(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value_signed = operands.I.imm.s;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value & imm_value;
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_andi(Operands operands, std::ostream *out) {
    *out << "andi "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.s << std::endl;
}

constexpr CodePoint inst_andi = {
    i_operands_from,
    ex_andi<__uint32_t>,
    ex_andi<__uint64_t>,
    ex_andi<__uint128_t>,
    print_andi,
    4
};

// -- lui --

inline Operands u_operands_from(__uint32_t inst) {
    Operands result;
    result.U.rd = swizzle<__uint32_t, RD>(inst);
    result.U.imm.u = swizzle<__uint32_t, ExtendBits::Zero, U_IMM>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_lui(Operands operands, HartState *state) {
    XLEN_t imm_value = operands.U.imm.u;
    XLEN_t rd_value = imm_value;
    state->regs[operands.U.rd].Write<XLEN_t>(rd_value);
}

inline void print_lui(Operands operands, std::ostream *out) {
    *out << "lui "
         << RISCV::regName(operands.U.rd) << ", "
         << (operands.U.imm.u >> 12) << std::endl;
}

constexpr CodePoint inst_lui = {
    u_operands_from,
    ex_lui<__uint32_t>,
    ex_lui<__uint64_t>,
    ex_lui<__uint128_t>,
    print_lui,
    4
};

// -- auipc --

template<typename XLEN_t>
inline void ex_auipc(Operands operands, HartState *state) {
    XLEN_t pc_value = state->currentFetch->virtualPC.Read<XLEN_t>();
    XLEN_t imm_value = operands.U.imm.u;
    XLEN_t rd_value = pc_value + imm_value;
    state->regs[operands.U.rd].Write<XLEN_t>(rd_value);
}

inline void print_auipc(Operands operands, std::ostream *out) {
    *out << "auipc "
         << RISCV::regName(operands.U.rd) << ", "
         << operands.U.imm.u << std::endl;
}

constexpr CodePoint inst_auipc = {
    u_operands_from,
    ex_auipc<__uint32_t>,
    ex_auipc<__uint64_t>,
    ex_auipc<__uint128_t>,
    print_auipc,
    4
};

// -- jal --

inline Operands j_operands_from(__uint32_t inst) {
    Operands result;
    result.U.rd = swizzle<__uint32_t, RD>(inst);
    result.U.imm.s = swizzle<__uint32_t, ExtendBits::Sign, J_IMM>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_jal(Operands operands, HartState *state) {
    XLEN_t next_pc_value = state->nextFetchVirtualPC->Read<XLEN_t>();
    state->regs[operands.U.rd].Write<XLEN_t>(next_pc_value);
    XLEN_t pc_value = state->currentFetch->virtualPC.Read<XLEN_t>();
    XLEN_t new_pc_value = pc_value + operands.U.imm.s;
    state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
}

inline void print_jal(Operands operands, std::ostream *out) {
    *out << "jal "
         << RISCV::regName(operands.U.rd) << ", "
         << operands.U.imm.s << std::endl;
}

constexpr CodePoint inst_jal = {
    j_operands_from,
    ex_jal<__uint32_t>,
    ex_jal<__uint64_t>,
    ex_jal<__uint128_t>,
    print_jal,
    4
};

// -- jalr --

template<typename XLEN_t>
inline void ex_jalr(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rd_value = state->nextFetchVirtualPC->Read<XLEN_t>();
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value = operands.I.imm.s;
    imm_value &= ~(XLEN_t)1;
    XLEN_t new_pc_value = rs1_value + imm_value;
    state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
    state->regs[operands.I.rd].Write<XLEN_t>(rd_value);
}

inline void print_jalr(Operands operands, std::ostream *out) {
    *out << "jalr "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::regName(operands.I.rs1) << ", "
         << operands.I.imm.s << std::endl;
}

constexpr CodePoint inst_jalr = {
    i_operands_from,
    ex_jalr<__uint32_t>,
    ex_jalr<__uint64_t>,
    ex_jalr<__uint128_t>,
    print_jalr,
    4
};

// -- beq --

inline Operands b_operands_from(__uint32_t inst) {
    Operands result;
    result.S.rs1 = swizzle<__uint32_t, RS1>(inst);
    result.S.rs2 = swizzle<__uint32_t, RS2>(inst);
    result.S.imm.s = swizzle<__uint32_t, B_IMM>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_beq(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    SXLEN_t imm_value = operands.S.imm.s;
    if (rs1_value == rs2_value) {
        XLEN_t new_pc_value = state->currentFetch->virtualPC.Read<XLEN_t>() + imm_value;
        state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
    }
}

inline void print_beq(Operands operands, std::ostream *out) {
    *out << "beq "
         << RISCV::regName(operands.S.rs1) << ", "
         << RISCV::regName(operands.S.rs2) << ", "
         << operands.S.imm.s << std::endl;
}

constexpr CodePoint inst_beq = {
    b_operands_from,
    ex_beq<__uint32_t>,
    ex_beq<__uint64_t>,
    ex_beq<__uint128_t>,
    print_beq,
    4
};

// -- bne --

template<typename XLEN_t>
inline void ex_bne(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    SXLEN_t imm_value = operands.S.imm.s;
    if (rs1_value != rs2_value) {
        XLEN_t new_pc_value = state->currentFetch->virtualPC.Read<XLEN_t>() + imm_value;
        state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
    }
}

inline void print_bne(Operands operands, std::ostream *out) {
    *out << "bne "
         << RISCV::regName(operands.S.rs1) << ", "
         << RISCV::regName(operands.S.rs2) << ", "
         << operands.S.imm.s << std::endl;
}

constexpr CodePoint inst_bne = {
    b_operands_from,
    ex_bne<__uint32_t>,
    ex_bne<__uint64_t>,
    ex_bne<__uint128_t>,
    print_bne,
    4
};

// -- blt --

template<typename XLEN_t>
inline void ex_blt(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    SXLEN_t imm_value = operands.S.imm.s;
    if (rs1_value < rs2_value) {
        XLEN_t new_pc_value = state->currentFetch->virtualPC.Read<XLEN_t>() + imm_value;
        state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
    }
}

inline void print_blt(Operands operands, std::ostream *out) {
    *out << "blt "
         << RISCV::regName(operands.S.rs1) << ", "
         << RISCV::regName(operands.S.rs2) << ", "
         << operands.S.imm.s << std::endl;
}

constexpr CodePoint inst_blt = {
    b_operands_from,
    ex_blt<__uint32_t>,
    ex_blt<__uint64_t>,
    ex_blt<__uint128_t>,
    print_blt,
    4
};

// -- bge --

template<typename XLEN_t>
inline void ex_bge(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    SXLEN_t imm_value = operands.S.imm.s;
    if (rs1_value >= rs2_value) {
        XLEN_t new_pc_value = state->currentFetch->virtualPC.Read<XLEN_t>() + imm_value;
        state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
    }
}

inline void print_bge(Operands operands, std::ostream *out) {
    *out << "bge "
         << RISCV::regName(operands.S.rs1) << ", "
         << RISCV::regName(operands.S.rs2) << ", "
         << operands.S.imm.s << std::endl;
}

constexpr CodePoint inst_bge = {
    b_operands_from,
    ex_bge<__uint32_t>,
    ex_bge<__uint64_t>,
    ex_bge<__uint128_t>,
    print_bge,
    4
};

// -- bltu --

template<typename XLEN_t>
inline void ex_bltu(Operands operands, HartState *state) {
    XLEN_t unsigned_rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    XLEN_t unsigned_rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t imm_value = operands.S.imm.s;
    if (unsigned_rs1_value < unsigned_rs2_value) {
        XLEN_t new_pc_value = state->currentFetch->virtualPC.Read<XLEN_t>() + imm_value;
        state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
    }
}

inline void print_bltu(Operands operands, std::ostream *out) {
    *out << "bltu "
         << RISCV::regName(operands.S.rs1) << ", "
         << RISCV::regName(operands.S.rs2) << ", "
         << operands.S.imm.s << std::endl;
}

constexpr CodePoint inst_bltu = {
    b_operands_from,
    ex_bltu<__uint32_t>,
    ex_bltu<__uint64_t>,
    ex_bltu<__uint128_t>,
    print_bltu,
    4
};

// -- bgeu --

template<typename XLEN_t>
inline void ex_bgeu(Operands operands, HartState *state) {
    XLEN_t unsigned_rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    XLEN_t unsigned_rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t imm_value = operands.S.imm.s;
    if (unsigned_rs1_value >= unsigned_rs2_value) {
        XLEN_t new_pc_value = state->currentFetch->virtualPC.Read<XLEN_t>() + imm_value;
        state->nextFetchVirtualPC->Write<XLEN_t>(new_pc_value);
    }
}

inline void print_bgeu(Operands operands, std::ostream *out) {
    *out << "bgeu "
         << RISCV::regName(operands.S.rs1) << ", "
         << RISCV::regName(operands.S.rs2) << ", "
         << operands.S.imm.s << std::endl;
}

constexpr CodePoint inst_bgeu = {
    b_operands_from,
    ex_bgeu<__uint32_t>,
    ex_bgeu<__uint64_t>,
    ex_bgeu<__uint128_t>,
    print_bgeu,
    4
};

// -- lb --

template<typename XLEN_t>
inline void ex_lb(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint8_t word;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value = operands.I.imm.s;
    XLEN_t read_address = rs1_value + imm_value;
    XLEN_t read_size = 1;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(read_address, read_size, (char*)&word);
    if (!success) {
        return;
    }
    SXLEN_t sign_extended_word = (SXLEN_t)word;
    state->regs[operands.I.rd].Write<XLEN_t>(sign_extended_word);
}

inline void print_lb(Operands operands, std::ostream *out) {
    *out << "lb "
         << RISCV::regName(operands.I.rd) << ",("
         << operands.I.imm.s << ")"
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_lb = {
    i_operands_from,
    ex_lb<__uint32_t>,
    ex_lb<__uint64_t>,
    ex_lb<__uint128_t>,
    print_lb,
    4
};

// -- lh --

template<typename XLEN_t>
inline void ex_lh(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint16_t word;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value = operands.I.imm.s;
    XLEN_t read_address = rs1_value + imm_value;
    XLEN_t read_size = 2;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(read_address, read_size, (char*)&word);
    if (!success) {
        return;
    }
    SXLEN_t sign_extended_word = (SXLEN_t)word;
    state->regs[operands.I.rd].Write<XLEN_t>(sign_extended_word);
}

inline void print_lh(Operands operands, std::ostream *out) {
    *out << "lh "
         << RISCV::regName(operands.I.rd) << ",("
         << operands.I.imm.s << ")"
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_lh = {
    i_operands_from,
    ex_lh<__uint32_t>,
    ex_lh<__uint64_t>,
    ex_lh<__uint128_t>,
    print_lh,
    4
};

// -- lw --

// TODO endianness-agnostic impl; for now x86 and RV being both LE save us
template<typename XLEN_t>
inline void ex_lw(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t word;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value = operands.I.imm.s;
    XLEN_t read_address = rs1_value + imm_value;
    XLEN_t read_size = 4;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(read_address, read_size, (char*)&word);
    if (!success) {
        return;
    }
    state->regs[operands.I.rd].Write<XLEN_t>(word);
}

inline void print_lw(Operands operands, std::ostream *out) {
    *out << "lw "
         << RISCV::regName(operands.I.rd) << ",("
         << operands.I.imm.s << ")"
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_lw = {
    i_operands_from,
    ex_lw<__uint32_t>,
    ex_lw<__uint64_t>,
    ex_lw<__uint128_t>,
    print_lw,
    4
};

// -- lbu --

template<typename XLEN_t>
inline void ex_lbu(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint8_t word;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value = operands.I.imm.s;
    XLEN_t read_address = rs1_value + imm_value;
    XLEN_t read_size = 1;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(read_address, read_size, (char*)&word);
    if (!success) {
        return;
    }
    SXLEN_t signed_word = word;
    XLEN_t unsigned_word = *((XLEN_t*)(&signed_word));
    state->regs[operands.I.rd].Write<XLEN_t>(unsigned_word);
}

inline void print_lbu(Operands operands, std::ostream *out) {
    *out << "lbu "
         << RISCV::regName(operands.I.rd) << ",("
         << operands.I.imm.u << ")"
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_lbu = {
    i_operands_from,
    ex_lbu<__uint32_t>,
    ex_lbu<__uint64_t>,
    ex_lbu<__uint128_t>,
    print_lbu,
    4
};

// -- lhu --

template<typename XLEN_t>
inline void ex_lhu(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint16_t word;
    XLEN_t rs1_value = state->regs[operands.I.rs1].Read<XLEN_t>();
    SXLEN_t imm_value = operands.I.imm.s;
    XLEN_t read_address = rs1_value + imm_value;
    XLEN_t read_size = 2;
    TransactionResult<XLEN_t> transactionResult = state->mmu->Read<XLEN_t>(read_address, read_size, (char*)&word);
    if (transactionResult.trapCause != RISCV::TrapCause::NONE) {
        state->RaiseException<XLEN_t>(transactionResult.trapCause, read_address);
        return;
    }
    SXLEN_t signed_word = word;
    XLEN_t unsigned_word = *((XLEN_t*)(&signed_word));
    state->regs[operands.I.rd].Write<XLEN_t>(unsigned_word);
}

inline void print_lhu(Operands operands, std::ostream *out) {
    *out << "lhu "
         << RISCV::regName(operands.I.rd) << ",("
         << operands.I.imm.u << ")"
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_lhu = {
    i_operands_from,
    ex_lhu<__uint32_t>,
    ex_lhu<__uint64_t>,
    ex_lhu<__uint128_t>,
    print_lhu,
    4
};

// -- sb --

inline Operands s_operands_from(__uint32_t inst) {
    Operands result;
    result.S.rs1 = swizzle<__uint32_t, RS1>(inst);
    result.S.rs2 = swizzle<__uint32_t, RS2>(inst);
    result.S.imm.u = (int32_t)swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_sb(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    SXLEN_t imm_value = operands.S.imm.s;
    XLEN_t write_addr = rs1_value + imm_value;
    __uint8_t write_value = rs2_value & (XLEN_t)0xff;
    XLEN_t write_size = sizeof(write_value);
    state->Transact<XLEN_t, CASK::AccessType::W>(write_addr, write_size, (char*)&write_value);
}

inline void print_sb(Operands operands, std::ostream *out) {
    *out << "sb "
         << RISCV::regName(operands.S.rs2) << ",("
         << operands.S.imm.s << ")"
         << RISCV::regName(operands.S.rs1) << std::endl;
}

constexpr CodePoint inst_sb = {
    s_operands_from,
    ex_sb<__uint32_t>,
    ex_sb<__uint64_t>,
    ex_sb<__uint128_t>,
    print_sb,
    4
};

// -- sh --

template<typename XLEN_t>
inline void ex_sh(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    SXLEN_t imm_value = operands.S.imm.s;
    XLEN_t write_addr = rs1_value + imm_value;
    __uint16_t write_value = rs2_value & (XLEN_t)0xffff;
    XLEN_t write_size = sizeof(write_value);
    state->Transact<XLEN_t, CASK::AccessType::W>(write_addr, write_size, (char*)&write_value);
}

inline void print_sh(Operands operands, std::ostream *out) {
    *out << "sh "
         << RISCV::regName(operands.S.rs2) << ",("
         << operands.S.imm.s << ")"
         << RISCV::regName(operands.S.rs1) << std::endl;
}

constexpr CodePoint inst_sh = {
    s_operands_from,
    ex_sh<__uint32_t>,
    ex_sh<__uint64_t>,
    ex_sh<__uint128_t>,
    print_sh,
    4
};

// -- sw --

template<typename XLEN_t>
inline void ex_sw(Operands operands, HartState *state) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t rs1_value = state->regs[operands.S.rs1].Read<XLEN_t>();
    XLEN_t rs2_value = state->regs[operands.S.rs2].Read<XLEN_t>();
    SXLEN_t imm_value = operands.S.imm.s;
    XLEN_t write_addr = rs1_value + imm_value;
    __uint32_t write_value = rs2_value & (XLEN_t)0xffffffff;
    XLEN_t write_size = sizeof(write_value);
    state->Transact<XLEN_t, CASK::AccessType::W>(write_addr, write_size, (char*)&write_value);
}

inline void print_sw(Operands operands, std::ostream *out) {
    *out << "sw "
         << RISCV::regName(operands.S.rs2) << ",("
         << operands.S.imm.s << ")"
         << RISCV::regName(operands.S.rs1) << std::endl;
}

constexpr CodePoint inst_sw = {
    s_operands_from,
    ex_sw<__uint32_t>,
    ex_sw<__uint64_t>,
    ex_sw<__uint128_t>,
    print_sw,
    4
};

// -- fence --

template<typename XLEN_t>
inline void ex_fence(Operands operands, HartState *state) {
    // NOP for now.
}

inline void print_fence(Operands operands, std::ostream *out) {
    *out << "fence"
         << std::endl;
}

constexpr CodePoint inst_fence = {
    i_operands_from,
    ex_fence<__uint32_t>,
    ex_fence<__uint64_t>,
    ex_fence<__uint128_t>,
    print_fence,
    4
};

// -- fencei --

inline Operands no_operands_from(__uint32_t inst) {
    return { { } };
}

template<typename XLEN_t>
inline void ex_fencei(Operands operands, HartState *state) {
    state->notifyInstructionFenceRequested();
}

inline void print_fencei(Operands operands, std::ostream *out) {
    *out << "fence.i"
         << std::endl;
}

constexpr CodePoint inst_fencei = {
    no_operands_from,
    ex_fencei<__uint32_t>,
    ex_fencei<__uint64_t>,
    ex_fencei<__uint128_t>,
    print_fencei,
    4
};

// -- ecall --

template<typename XLEN_t>
inline void ex_ecall(Operands operands, HartState *state) {
    RISCV::TrapCause cause =
        state->privilegeMode == RISCV::PrivilegeMode::Machine ? RISCV::TrapCause::ECALL_FROM_M_MODE :
        state->privilegeMode == RISCV::PrivilegeMode::Supervisor ? RISCV::TrapCause::ECALL_FROM_S_MODE :
        RISCV::TrapCause::ECALL_FROM_U_MODE;
    state->RaiseException<XLEN_t>(cause, state->currentFetch->encoding);
}

inline void print_ecall(Operands operands, std::ostream *out) {
    *out << "ecall"
         << std::endl;
}

constexpr CodePoint inst_ecall = {
    no_operands_from,
    ex_ecall<__uint32_t>,
    ex_ecall<__uint64_t>,
    ex_ecall<__uint128_t>,
    print_ecall,
    4
};

// -- ebreak --

template<typename XLEN_t>
inline void ex_ebreak(Operands operands, HartState *state) {
    state->RaiseException<XLEN_t>(RISCV::TrapCause::BREAKPOINT, state->currentFetch->encoding);
}

inline void print_ebreak(Operands operands, std::ostream *out) {
    *out << "ebreak"
         << std::endl;
}

constexpr CodePoint inst_ebreak = {
    no_operands_from,
    ex_ebreak<__uint32_t>,
    ex_ebreak<__uint64_t>,
    ex_ebreak<__uint128_t>,
    print_ebreak,
    4
};

// -- csrrw --

inline Operands csr_operands_from(__uint32_t inst) {
    Operands result;
    result.I.rd = swizzle<__uint32_t, RD>(inst);
    result.I.rs1 = swizzle<__uint32_t, RS1>(inst);
    result.I.imm.u = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(inst);
    return result;
}

template<typename XLEN_t>
inline void ex_csrrw(Operands operands, HartState *state) {
    XLEN_t regVal = state->regs[operands.I.rs1].Read<XLEN_t>();
    RISCV::CSRAddress csr = (RISCV::CSRAddress)operands.I.imm.u;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        // TODO zero is one option, currentFetch encoding is another, spec struct controls. Implement throughout ISA...
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    if (operands.I.rd != 0) {
        XLEN_t csrValue = state->csrs[csr]->Read<XLEN_t>();
        state->regs[operands.I.rd].Write<XLEN_t>(csrValue);
    }
    state->csrs[csr]->Write<XLEN_t>(regVal);
}

inline void print_csrrw(Operands operands, std::ostream *out) {
    *out << "csrrw "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::csrName(operands.I.imm.u) << ", "
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_csrrw = {
    csr_operands_from,
    ex_csrrw<__uint32_t>,
    ex_csrrw<__uint64_t>,
    ex_csrrw<__uint128_t>,
    print_csrrw,
    4
};

// -- csrrs --

template<typename XLEN_t>
inline void ex_csrrs(Operands operands, HartState *state) {
    XLEN_t regVal = state->regs[operands.I.rs1].Read<XLEN_t>();
    RISCV::CSRAddress csr = (RISCV::CSRAddress)operands.I.imm.u;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if ((operands.I.rs1 != 0 && readOnly) || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->csrs[csr]->Read<XLEN_t>();
    state->regs[operands.I.rd].Write<XLEN_t>(csrValue);
    if (operands.I.rs1 != 0) {
        state->csrs[csr]->Write<XLEN_t>(csrValue | regVal);
    }
}

inline void print_csrrs(Operands operands, std::ostream *out) {
    *out << "csrrs "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::csrName(operands.I.imm.u) << ", "
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_csrrs = {
    csr_operands_from,
    ex_csrrs<__uint32_t>,
    ex_csrrs<__uint64_t>,
    ex_csrrs<__uint128_t>,
    print_csrrs,
    4
};

// -- csrrc --

template<typename XLEN_t>
inline void ex_csrrc(Operands operands, HartState *state) {
    XLEN_t regVal = state->regs[operands.I.rs1].Read<XLEN_t>();
    RISCV::CSRAddress csr = (RISCV::CSRAddress)operands.I.imm.u;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if ((operands.I.rs1 != 0 && readOnly) || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->csrs[csr]->Read<XLEN_t>();
    state->regs[operands.I.rd].Write<XLEN_t>(csrValue);
    if (operands.I.rs1 != 0) {
        state->csrs[csr]->Write<XLEN_t>(~csrValue & regVal);
    }
}

inline void print_csrrc(Operands operands, std::ostream *out) {
    *out << "csrrc "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::csrName(operands.I.imm.u) << ", "
         << RISCV::regName(operands.I.rs1) << std::endl;
}

constexpr CodePoint inst_csrrc = {
    csr_operands_from,
    ex_csrrc<__uint32_t>,
    ex_csrrc<__uint64_t>,
    ex_csrrc<__uint128_t>,
    print_csrrc,
    4
};

// -- csrrwi --

template<typename XLEN_t>
inline void ex_csrrwi(Operands operands, HartState *state) {
    RISCV::CSRAddress csr = (RISCV::CSRAddress)operands.I.imm.u;
    XLEN_t imm_value = operands.I.rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    if (operands.I.rd != 0) {
        XLEN_t csrValue = state->csrs[csr]->Read<XLEN_t>();
        state->regs[operands.I.rd].Write<XLEN_t>(csrValue);
    }
    state->csrs[csr]->Write<XLEN_t>(imm_value);
}

inline void print_csrrwi(Operands operands, std::ostream *out) {
    *out << "csrrwi "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::csrName(operands.I.imm.u) << ", "
         << operands.I.rs1 << std::endl;
}

constexpr CodePoint inst_csrrwi = {
    csr_operands_from,
    ex_csrrwi<__uint32_t>,
    ex_csrrwi<__uint64_t>,
    ex_csrrwi<__uint128_t>,
    print_csrrwi,
    4
};

// -- csrrsi --

template<typename XLEN_t>
inline void ex_csrrsi(Operands operands, HartState *state) {
    RISCV::CSRAddress csr = (RISCV::CSRAddress)operands.I.imm.u;
    XLEN_t imm_value = operands.I.rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->csrs[csr]->Read<XLEN_t>();
    state->regs[operands.I.rd].Write<XLEN_t>(csrValue);
    state->csrs[csr]->Write<XLEN_t>(csrValue | imm_value);
}

inline void print_csrrsi(Operands operands, std::ostream *out) {
    *out << "csrrsi "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::csrName(operands.I.imm.u) << ", "
         << operands.I.rs1 << std::endl;
}

constexpr CodePoint inst_csrrsi = {
    csr_operands_from,
    ex_csrrsi<__uint32_t>,
    ex_csrrsi<__uint64_t>,
    ex_csrrsi<__uint128_t>,
    print_csrrsi,
    4
};

// -- csrrci --

template<typename XLEN_t>
inline void ex_csrrci(Operands operands, HartState *state) {
    RISCV::CSRAddress csr = (RISCV::CSRAddress)operands.I.imm.u;
    XLEN_t imm_value = operands.I.rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->csrs[csr]->Read<XLEN_t>();
    state->regs[operands.I.rd].Write<XLEN_t>(csrValue);
    state->csrs[csr]->Write<XLEN_t>(~csrValue & imm_value);
}

inline void print_csrrci(Operands operands, std::ostream *out) {
    *out << "csrrci "
         << RISCV::regName(operands.I.rd) << ", "
         << RISCV::csrName(operands.I.imm.u) << ", "
         << operands.I.rs1 << std::endl;
}

constexpr CodePoint inst_csrrci = {
    csr_operands_from,
    ex_csrrci<__uint32_t>,
    ex_csrrci<__uint64_t>,
    ex_csrrci<__uint128_t>,
    print_csrrci,
    4
};

// -- illegal instruction --

template<typename XLEN_t>
inline void ex_illegal(Operands operands, HartState *state) {
    // TODO X need help from the hart w/ either the instruction or a function to know my own encoding
    state->RaiseException<XLEN_t>(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
}

inline void print_illegal(Operands operands, std::ostream *out) {
    *out << "Illegal Instruction" << std::endl;
}

constexpr CodePoint illegal4ByteInstruction = {
    no_operands_from,
    ex_illegal<__uint32_t>,
    ex_illegal<__uint64_t>,
    ex_illegal<__uint128_t>,
    print_illegal,
    4
};

// -- reserved instruction --

constexpr CodePoint reserved4ByteInstruction = {
    no_operands_from,
    ex_illegal<__uint32_t>,
    ex_illegal<__uint64_t>,
    ex_illegal<__uint128_t>,
    print_illegal,
    4
};

// -- unimplemented instruction --

template<typename XLEN_t>
inline void ex_unimplemented(Operands operands, HartState *state) {
    exit(1);
}

inline void print_unimplemented(Operands operands, std::ostream *out) {
    *out << "Unimplemented Instruction" << std::endl;
}

constexpr CodePoint unimplemented4ByteInstruction = {
    no_operands_from,
    ex_unimplemented<__uint32_t>,
    ex_unimplemented<__uint64_t>,
    ex_unimplemented<__uint128_t>,
    print_unimplemented,
    4
};