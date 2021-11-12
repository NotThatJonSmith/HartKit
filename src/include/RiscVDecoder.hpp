#pragma once

/*
 * This file contains a "naive" implementation of a RISC-V ISA decoder. It's an
 * exhaustive tabular walkthrough of the whole encoding space of RISC-V. The
 * trick to making everything go fast is that this is all constexpr, so the
 * compiler can precompute tables that feed into faster decoder strategies.
 */

#include <CodePoint.hpp>
#include <RiscV.hpp>
#include <instructions/BaseISA.hpp>
#include <instructions/ExtensionC.hpp>
#include <instructions/ExtensionA.hpp>
#include <instructions/ExtensionM.hpp>
#include <instructions/PrivilegedISA.hpp>

// -- Common Decoder Fields --

#define QUADRANT    ExtendBits::Zero, 1, 0
#define OPCODE      ExtendBits::Zero, 6, 2
#define OP_MINOR    ExtendBits::Zero, 31, 25, 14, 12
#define FUNCT3      ExtendBits::Zero, 14, 12
#define FUNCT7      ExtendBits::Zero, 31, 25
#define SHAMT       ExtendBits::Zero, 24, 20
#define FUNCT5      ExtendBits::Zero, 31, 27

constexpr CodePoint decode_load(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::LB: return inst_lb;
    case RISCV::MinorOpcode::LH: return inst_lh;
    case RISCV::MinorOpcode::LW: return inst_lw;
    case RISCV::MinorOpcode::LBU: return inst_lbu;
    case RISCV::MinorOpcode::LHU: return inst_lhu;
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_load_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_custom_0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_misc_mem(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::FENCE: return inst_fence;
    case RISCV::MinorOpcode::FENCE_I: return inst_fencei;
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_op_imm(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::ADDI: return inst_addi;
    case RISCV::MinorOpcode::SLLI: return inst_slli;
    case RISCV::MinorOpcode::SLTI: return inst_slti;
    case RISCV::MinorOpcode::SLTIU: return inst_sltiu;
    case RISCV::MinorOpcode::XORI: return inst_xori;
    case RISCV::MinorOpcode::SRI:
        switch(swizzle<__uint32_t, FUNCT7>(inst)) {
        case RISCV::SubMinorOpcode::SRAI: return inst_srai;
        case RISCV::SubMinorOpcode::SRLI: return inst_srli;
        default: return illegal4ByteInstruction;
        }
    case RISCV::MinorOpcode::ORI: return inst_ori;
    case RISCV::MinorOpcode::ANDI: return inst_andi;
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_op_imm_32(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) { // TODO op_imm, __uint32_t extensionsVector_32
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_long_48b_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_store(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::SB: return inst_sb;
    case RISCV::MinorOpcode::SH: return inst_sh;
    case RISCV::MinorOpcode::SW: return inst_sw;
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_store_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) { // TODO store, __uint32_t extensionsVector_fp
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_custom_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_amo(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::AmoWidth::AMO_W:
        switch (swizzle<__uint32_t, FUNCT5>(inst)) {
        case RISCV::MinorOpcode::AMOADD: return inst_amoaddw;
        case RISCV::MinorOpcode::AMOSWAP: return inst_amoswapw;
        case RISCV::MinorOpcode::LR: return inst_lrw;
        case RISCV::MinorOpcode::SC: return inst_scw;
        case RISCV::MinorOpcode::AMOXOR: return inst_amoxorw;
        case RISCV::MinorOpcode::AMOOR: return inst_amoorw;
        case RISCV::MinorOpcode::AMOAND: return inst_amoandw;
        case RISCV::MinorOpcode::AMOMIN: return inst_amominw;
        case RISCV::MinorOpcode::AMOMAX: return inst_amomaxw;
        case RISCV::MinorOpcode::AMOMINU: return inst_amominuw;
        case RISCV::MinorOpcode::AMOMAXU: return inst_amomaxuw;
        default: return illegal4ByteInstruction;
        }
    case RISCV::AmoWidth::AMO_D:
        switch (swizzle<__uint32_t, FUNCT5>(inst)) {
        case RISCV::MinorOpcode::AMOADD: return inst_amoaddd;
        case RISCV::MinorOpcode::AMOSWAP: return inst_amoswapd;
        case RISCV::MinorOpcode::LR: return inst_lrd;
        case RISCV::MinorOpcode::SC: return inst_scd;
        case RISCV::MinorOpcode::AMOXOR: return inst_amoxord;
        case RISCV::MinorOpcode::AMOOR: return inst_amoord;
        case RISCV::MinorOpcode::AMOAND: return inst_amoandd;
        case RISCV::MinorOpcode::AMOMIN: return inst_amomind;
        case RISCV::MinorOpcode::AMOMAX: return inst_amomaxd;
        case RISCV::MinorOpcode::AMOMINU: return inst_amominud;
        case RISCV::MinorOpcode::AMOMAXU: return inst_amomaxud;
        default: return illegal4ByteInstruction;
        }
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_op(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, OP_MINOR>(inst)) {
    case RISCV::MinorOpcode::ADD: return inst_add;
    case RISCV::MinorOpcode::SUB: return inst_sub;
    case RISCV::MinorOpcode::SLL: return inst_sll;
    case RISCV::MinorOpcode::SLT: return inst_slt;
    case RISCV::MinorOpcode::SLTU: return inst_sltu;
    case RISCV::MinorOpcode::XOR: return inst_xor;
    case RISCV::MinorOpcode::SRA: return inst_sra;
    case RISCV::MinorOpcode::SRL: return inst_srl;
    case RISCV::MinorOpcode::OR: return inst_or;
    case RISCV::MinorOpcode::AND: return inst_and;
    case RISCV::MinorOpcode::MUL: return inst_mul;
    case RISCV::MinorOpcode::MULH: return inst_mulh;
    case RISCV::MinorOpcode::MULHSU: return inst_mulhsu;
    case RISCV::MinorOpcode::MULHU: return inst_mulhu;
    case RISCV::MinorOpcode::DIV: return inst_div;
    case RISCV::MinorOpcode::DIVU: return inst_divu;
    case RISCV::MinorOpcode::REM: return inst_rem;
    case RISCV::MinorOpcode::REMU: return inst_remu;
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_op_32(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_long_64b(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_madd(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_msub(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_nmsub(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_nmadd(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_op_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_reserved_0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_custom_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_long_48b_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_branch(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::BEQ: return inst_beq;
    case RISCV::MinorOpcode::BNE: return inst_bne;
    case RISCV::MinorOpcode::BLT: return inst_blt;
    case RISCV::MinorOpcode::BGE: return inst_bge;
    case RISCV::MinorOpcode::BLTU: return inst_bltu;
    case RISCV::MinorOpcode::BGEU: return inst_bgeu;
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_reserved_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_system(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::PRIV:
        switch (swizzle<__uint32_t, FUNCT7>(inst)) {
        case RISCV::SubMinorOpcode::ECALL_EBREAK_URET:
            switch (swizzle<__uint32_t, RS2>(inst)) {
            case RISCV::SubSubMinorOpcode::ECALL: return inst_ecall;
            case RISCV::SubSubMinorOpcode::EBREAK: return inst_ebreak;
            case RISCV::SubSubMinorOpcode::URET: return inst_uret;
            default: return illegal4ByteInstruction;
            }
        case RISCV::SubMinorOpcode::SRET_WFI:
            switch (swizzle<__uint32_t, RS2>(inst)) {
            case RISCV::SubSubMinorOpcode::WFI: return inst_wfi;
            case RISCV::SubSubMinorOpcode::SRET: return inst_sret;
            default: return illegal4ByteInstruction;
            }
        case RISCV::SubMinorOpcode::MRET: return inst_mret;
        case RISCV::SFENCE_VMA: return inst_sfencevma;
        default: return illegal4ByteInstruction;
        }
    case RISCV::MinorOpcode::CSRRW: return inst_csrrw;
    case RISCV::MinorOpcode::CSRRS: return inst_csrrs;
    case RISCV::MinorOpcode::CSRRC: return inst_csrrc;
    case RISCV::MinorOpcode::CSRRWI: return inst_csrrwi;
    case RISCV::MinorOpcode::CSRRSI: return inst_csrrsi;
    case RISCV::MinorOpcode::CSRRCI: return inst_csrrci;
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_reserved_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_custom_3(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_long_80b(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return unimplemented4ByteInstruction;
}

constexpr CodePoint decode_compressed_q0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0:
        if (swizzle<__uint32_t, ExtendBits::Zero, 10, 7, 12, 11, 5, 5, 6, 6, 2>(inst) == 0)
            return illegal2ByteInstruction;
        return inst_caddi4spn;
    case 1:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return unimplemented2ByteInstruction; // C.FLD TODO
        return unimplemented2ByteInstruction; // C.LQ TODO
    case 2: return inst_clw;
    case 3:
        if (mxlen == RISCV::XlenMode::XL32)
            return unimplemented2ByteInstruction; // C.FLW TODO
        return unimplemented2ByteInstruction; // C.LD TODO
    case 4: return reserved2ByteInstruction;
    case 5:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return unimplemented2ByteInstruction; // C.FSD TODO
        return unimplemented2ByteInstruction; // C.SQ TODO
    case 6: return inst_csw;
    case 7:
        if (mxlen == RISCV::XlenMode::XL32)
            return unimplemented2ByteInstruction; // C.FSW TODO
        return unimplemented2ByteInstruction; // C.SD TODO
    default: return illegal2ByteInstruction;
    }
}

constexpr CodePoint decode_compressed_q1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0: return inst_caddi; // Note that NOP is the same instruction
    case 1:
        if (mxlen == RISCV::XlenMode::XL32)
            return inst_cjal;
        return unimplemented2ByteInstruction; // C.ADDIW TODO
    case 2: return inst_cli;
    case 3:
        if (swizzle<__uint32_t, CI_RD_RS1>(inst) != 2)
            return inst_clui;
        if (swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 4, 3, 5, 5, 2, 2, 6, 6, 4>(inst) != 0)
            return inst_caddi16sp;
        return reserved2ByteInstruction;
    case 4:
        switch (swizzle<__uint32_t, ExtendBits::Zero, 11, 10>(inst)) {
        case 0:
            if (mxlen == RISCV::XlenMode::XL32)
                if (swizzle<__uint32_t, CB_SHAMT>(inst) & 1 << 5)
                    return reserved2ByteInstruction;
            if (mxlen == RISCV::XlenMode::XL128)
                return inst_csrli_128;
            return inst_csrli;
        case 1:
            if (mxlen == RISCV::XlenMode::XL32)
                if (swizzle<__uint32_t, CB_SHAMT>(inst) & 1 << 5)
                    return reserved2ByteInstruction;
            if (mxlen == RISCV::XlenMode::XL128)
                return inst_csrai_128;
            return inst_csrai;
        case 2: return inst_candi;
        case 3:
            switch(swizzle<__uint32_t, ExtendBits::Zero, 12, 12, 6, 5>(inst)) {
            case 0: return inst_csub;
            case 1: return inst_cxor;
            case 2: return inst_cor;
            case 3: return inst_cand;
            case 4: return unimplemented2ByteInstruction; // C.SUBW TODO
            case 5: return unimplemented2ByteInstruction; // C.ADDW TODO
            case 6: return reserved2ByteInstruction;
            case 7: return reserved2ByteInstruction;
            default: return illegal4ByteInstruction;
            }
        default: return illegal4ByteInstruction;
        }
    case 5: return inst_cj;
    case 6: return inst_cbeqz;
    case 7: return inst_cbnez;
    default: return illegal2ByteInstruction;
    }
}

constexpr CodePoint decode_compressed_q2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0:
        if (mxlen == RISCV::XlenMode::XL32)
            if (swizzle<__uint32_t, CI_SHAMT>(inst) & 1 << 5)
                return reserved2ByteInstruction;
        if (mxlen == RISCV::XlenMode::XL128)
            return inst_cslli_128;
        return inst_cslli;
    case 1:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return unimplemented2ByteInstruction; // C.FLDSP TODO
        return unimplemented2ByteInstruction; // C.LQSP TODO
    case 2: return inst_clwsp;
    case 3: return unimplemented2ByteInstruction; // C.FLWSP C.LDSP TODO
    case 4:
        if (inst & 1 << 12) {
            if (swizzle<__uint32_t, ExtendBits::Zero, 6, 2>(inst) == 0) {
                if (swizzle<__uint32_t, ExtendBits::Zero, 11, 7>(inst) == 0)
                    return inst_cebreak;
                return inst_cjalr;
            }
            return inst_cadd;
        } else {
            if (swizzle<__uint32_t, ExtendBits::Zero, 6, 2>(inst) == 0)
                return inst_cjr;
            return inst_cmv;
        }
    case 5: return unimplemented2ByteInstruction; // C.FSDSP C.SQSP TODO
    case 6: return inst_cswsp;
    case 7: return unimplemented2ByteInstruction; // C.FSWSP C.SDSP TODO
    default: return illegal2ByteInstruction;
    }
}
constexpr CodePoint decode_uncompressed(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, OPCODE>(inst)) {
    case RISCV::MajorOpcode::LOAD: return decode_load(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LOAD_FP: return decode_load_fp(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_0: return decode_custom_0(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MISC_MEM: return decode_misc_mem(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP_IMM: return decode_op_imm(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::AUIPC: return inst_auipc;
    case RISCV::MajorOpcode::OP_IMM_32: return decode_op_imm_32(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_48B_1: return decode_long_48b_1(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::STORE: return decode_store(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::STORE_FP: return decode_store_fp(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_1: return decode_custom_1(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::AMO: return decode_amo(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP: return decode_op(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LUI: return inst_lui;
    case RISCV::MajorOpcode::OP_32: return decode_op_32(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_64B: return decode_long_64b(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MADD: return decode_madd(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MSUB: return decode_msub(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::NMSUB: return decode_nmsub(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::NMADD: return decode_nmadd(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP_FP: return decode_op_fp(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::RESERVED_0: return decode_reserved_0(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_2: return decode_custom_2(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_48B_2: return decode_long_48b_2(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::BRANCH: return decode_branch(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::JALR: return inst_jalr;
    case RISCV::MajorOpcode::RESERVED_1: return decode_reserved_1(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::JAL: return inst_jal;
    case RISCV::MajorOpcode::SYSTEM: return decode_system(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::RESERVED_2: return decode_reserved_2(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_3: return decode_custom_3(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_80B: return decode_long_80b(inst, extensionsVector, mxlen);
    default: return illegal4ByteInstruction;
    }
}

constexpr CodePoint decode_instruction(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, QUADRANT>(inst)) {
    case RISCV::OpcodeQuadrant::UNCOMPRESSED: return decode_uncompressed(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q0: return decode_compressed_q0(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q1: return decode_compressed_q1(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q2: return decode_compressed_q2(inst, extensionsVector, mxlen);
    default: return illegal4ByteInstruction;
    }
}

constexpr Instruction decode_full(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen, RISCV::XlenMode currentXlen) {
    CodePoint decoded = decode_instruction(inst, extensionsVector, mxlen);
    Instruction exact = {};
    exact.disassemble = decoded.disassemble;
    exact.getOperands = decoded.getOperands;
    exact.width = decoded.width;
    switch (currentXlen) {
    case RISCV::XlenMode::XL32:
        exact.execute = decoded.execute32;
        break;
    case RISCV::XlenMode::XL64:
        exact.execute = decoded.execute64;
        break;
    case RISCV::XlenMode::XL128:
        exact.execute = decoded.execute128;
        break;
    case RISCV::XlenMode::None:
        break;
    }
    default: return illegal4ByteInstruction;
    return exact;
}
