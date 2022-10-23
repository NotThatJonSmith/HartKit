#pragma once

/*
 * This file contains a "naive" implementation of a RISC-V ISA decoder. It's an
 * exhaustive tabular walkthrough of the whole encoding space of RISC-V. The
 * trick to making everything go fast is that this is all constexpr, so the
 * compiler can precompute tables that feed into faster decoder strategies.
 */

#include <DecodedInstruction.hpp>
#include <RiscV.hpp>
#include <instructions/BaseISA.hpp>
#include <instructions/ExtensionC.hpp>
#include <instructions/ExtensionA.hpp>
#include <instructions/ExtensionM.hpp>
#include <instructions/PrivilegedISA.hpp>

#define QUADRANT    ExtendBits::Zero, 1, 0
#define OPCODE      ExtendBits::Zero, 6, 2
#define OP_MINOR    ExtendBits::Zero, 31, 25, 14, 12
#define FUNCT3      ExtendBits::Zero, 14, 12
#define FUNCT7      ExtendBits::Zero, 31, 25
#define SHAMT       ExtendBits::Zero, 24, 20
#define FUNCT5      ExtendBits::Zero, 31, 27

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_load(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::LB: return ex_lb<XLEN_t, out>;
    case RISCV::MinorOpcode::LH: return ex_lh<XLEN_t, out>;
    case RISCV::MinorOpcode::LW: return ex_lw<XLEN_t, out>;
    case RISCV::MinorOpcode::LBU: return ex_lbu<XLEN_t, out>;
    case RISCV::MinorOpcode::LHU: return ex_lhu<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_load_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_custom_0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_misc_mem(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::FENCE: return ex_fence<XLEN_t, out>;
    case RISCV::MinorOpcode::FENCE_I: return ex_fencei<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_op_imm(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::ADDI: return ex_addi<XLEN_t, out>;
    case RISCV::MinorOpcode::SLLI: return ex_slli<XLEN_t, out>;
    case RISCV::MinorOpcode::SLTI: return ex_slti<XLEN_t, out>;
    case RISCV::MinorOpcode::SLTIU: return ex_sltiu<XLEN_t, out>;
    case RISCV::MinorOpcode::XORI: return ex_xori<XLEN_t, out>;
    case RISCV::MinorOpcode::SRI:
        switch(swizzle<__uint32_t, FUNCT7>(inst)) {
        case RISCV::SubMinorOpcode::SRAI: return ex_srai<XLEN_t, out>;
        case RISCV::SubMinorOpcode::SRLI: return ex_srli<XLEN_t, out>;
        default: return ex_illegal;
        }
    case RISCV::MinorOpcode::ORI: return ex_ori<XLEN_t, out>;
    case RISCV::MinorOpcode::ANDI: return ex_andi<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_op_imm_32(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) { // TODO op_imm, __uint32_t extensionsVector_32
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_long_48b_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_store(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::SB: return ex_sb<XLEN_t, out>;
    case RISCV::MinorOpcode::SH: return ex_sh<XLEN_t, out>;
    case RISCV::MinorOpcode::SW: return ex_sw<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_store_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) { // TODO store, __uint32_t extensionsVector_fp
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_custom_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_amo(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::AmoWidth::AMO_W:
        switch (swizzle<__uint32_t, FUNCT5>(inst)) {
        case RISCV::MinorOpcode::AMOADD: return ex_amoaddw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOSWAP: return ex_amoswapw<XLEN_t, out>;
        case RISCV::MinorOpcode::LR: return ex_lrw<XLEN_t, out>;
        case RISCV::MinorOpcode::SC: return ex_scw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOXOR: return ex_amoxorw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOOR: return ex_amoorw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOAND: return ex_amoandw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMIN: return ex_amominw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMAX: return ex_amomaxw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMINU: return ex_amominuw<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMAXU: return ex_amomaxuw<XLEN_t, out>;
        default: return ex_illegal;
        }
    case RISCV::AmoWidth::AMO_D:
        switch (swizzle<__uint32_t, FUNCT5>(inst)) {
        case RISCV::MinorOpcode::AMOADD: return ex_amoaddd<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOSWAP: return ex_amoswapd<XLEN_t, out>;
        case RISCV::MinorOpcode::LR: return ex_lrd<XLEN_t, out>;
        case RISCV::MinorOpcode::SC: return ex_scd<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOXOR: return ex_amoxord<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOOR: return ex_amoord<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOAND: return ex_amoandd<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMIN: return ex_amomind<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMAX: return ex_amomaxd<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMINU: return ex_amominud<XLEN_t, out>;
        case RISCV::MinorOpcode::AMOMAXU: return ex_amomaxud<XLEN_t, out>;
        default: return ex_illegal;
        }
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_op(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, OP_MINOR>(inst)) {
    case RISCV::MinorOpcode::ADD: return ex_add<XLEN_t, out>;
    case RISCV::MinorOpcode::SUB: return ex_sub<XLEN_t, out>;
    case RISCV::MinorOpcode::SLL: return ex_sll<XLEN_t, out>;
    case RISCV::MinorOpcode::SLT: return ex_slt<XLEN_t, out>;
    case RISCV::MinorOpcode::SLTU: return ex_sltu<XLEN_t, out>;
    case RISCV::MinorOpcode::XOR: return ex_xor<XLEN_t, out>;
    case RISCV::MinorOpcode::SRA: return ex_sra<XLEN_t, out>;
    case RISCV::MinorOpcode::SRL: return ex_srl<XLEN_t, out>;
    case RISCV::MinorOpcode::OR: return ex_or<XLEN_t, out>;
    case RISCV::MinorOpcode::AND: return ex_and<XLEN_t, out>;
    case RISCV::MinorOpcode::MUL: return ex_mul<XLEN_t, out>;
    case RISCV::MinorOpcode::MULH: return ex_mulh<XLEN_t, out>;
    case RISCV::MinorOpcode::MULHSU: return ex_mulhsu<XLEN_t, out>;
    case RISCV::MinorOpcode::MULHU: return ex_mulhu<XLEN_t, out>;
    case RISCV::MinorOpcode::DIV: return ex_div<XLEN_t, out>;
    case RISCV::MinorOpcode::DIVU: return ex_divu<XLEN_t, out>;
    case RISCV::MinorOpcode::REM: return ex_rem<XLEN_t, out>;
    case RISCV::MinorOpcode::REMU: return ex_remu<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_op_32(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_long_64b(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_madd(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_msub(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_nmsub(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_nmadd(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_op_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_reserved_0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}
template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_custom_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_long_48b_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_branch(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::BEQ: return ex_beq<XLEN_t, out>;
    case RISCV::MinorOpcode::BNE: return ex_bne<XLEN_t, out>;
    case RISCV::MinorOpcode::BLT: return ex_blt<XLEN_t, out>;
    case RISCV::MinorOpcode::BGE: return ex_bge<XLEN_t, out>;
    case RISCV::MinorOpcode::BLTU: return ex_bltu<XLEN_t, out>;
    case RISCV::MinorOpcode::BGEU: return ex_bgeu<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_reserved_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_system(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::PRIV:
        switch (swizzle<__uint32_t, FUNCT7>(inst)) {
        case RISCV::SubMinorOpcode::ECALL_EBREAK_URET:
            switch (swizzle<__uint32_t, RS2>(inst)) {
            case RISCV::SubSubMinorOpcode::ECALL: return ex_ecall<XLEN_t, out>;
            case RISCV::SubSubMinorOpcode::EBREAK: return ex_ebreak<XLEN_t, out>;
            case RISCV::SubSubMinorOpcode::URET: return ex_uret<XLEN_t, out>;
            default: return ex_illegal;
            }
        case RISCV::SubMinorOpcode::SRET_WFI:
            switch (swizzle<__uint32_t, RS2>(inst)) {
            case RISCV::SubSubMinorOpcode::WFI: return ex_wfi<XLEN_t, out>;
            case RISCV::SubSubMinorOpcode::SRET: return ex_sret<XLEN_t, out>;
            default: return ex_illegal;
            }
        case RISCV::SubMinorOpcode::MRET: return ex_mret<XLEN_t, out>;
        case RISCV::SFENCE_VMA: return ex_sfencevma<XLEN_t, out>;
        default: return ex_illegal;
        }
    case RISCV::MinorOpcode::CSRRW: return ex_csrrw<XLEN_t, out>;
    case RISCV::MinorOpcode::CSRRS: return ex_csrrs<XLEN_t, out>;
    case RISCV::MinorOpcode::CSRRC: return ex_csrrc<XLEN_t, out>;
    case RISCV::MinorOpcode::CSRRWI: return ex_csrrwi<XLEN_t, out>;
    case RISCV::MinorOpcode::CSRRSI: return ex_csrrsi<XLEN_t, out>;
    case RISCV::MinorOpcode::CSRRCI: return ex_csrrci<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_reserved_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_custom_3(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_long_80b(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return ex_unimplemented;
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_compressed_q0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0:
        if (swizzle<__uint32_t, ExtendBits::Zero, 10, 7, 12, 11, 5, 5, 6, 6, 2>(inst) == 0)
            return ex_illegal;
        return ex_caddi4spn<XLEN_t, out>;
    case 1:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return ex_unimplemented; // C.FLD TODO
        return ex_unimplemented; // C.LQ TODO
    case 2: return ex_clw<XLEN_t, out>;
    case 3:
        if (mxlen == RISCV::XlenMode::XL32)
            return ex_unimplemented; // C.FLW TODO
        return ex_unimplemented; // C.LD TODO
    case 4: return ex_reserved;
    case 5:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return ex_unimplemented; // C.FSD TODO
        return ex_unimplemented; // C.SQ TODO
    case 6: return ex_csw<XLEN_t, out>;
    case 7:
        if (mxlen == RISCV::XlenMode::XL32)
            return ex_unimplemented; // C.FSW TODO
        return ex_unimplemented; // C.SD TODO
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_compressed_q1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0: return ex_caddi<XLEN_t, out>; // Note that NOP is the same instruction
    case 1:
        if (mxlen == RISCV::XlenMode::XL32)
            return ex_cjal<XLEN_t, out>;
        return ex_unimplemented; // C.ADDIW TODO
    case 2: return ex_cli<XLEN_t, out>;
    case 3:
        if (swizzle<__uint32_t, CI_RD_RS1>(inst) != 2)
            return ex_clui<XLEN_t, out>;
        if (swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 4, 3, 5, 5, 2, 2, 6, 6, 4>(inst) != 0)
            return ex_caddi16sp<XLEN_t, out>;
        return ex_reserved;
    case 4:
        switch (swizzle<__uint32_t, ExtendBits::Zero, 11, 10>(inst)) {
        case 0:
            if (mxlen == RISCV::XlenMode::XL32)
                if (swizzle<__uint32_t, CB_SHAMT>(inst) & 1 << 5)
                    return ex_reserved;
            if (mxlen == RISCV::XlenMode::XL128)
                return ex_csrli_128<XLEN_t, out>;
            return ex_csrli<XLEN_t, out>;
        case 1:
            if (mxlen == RISCV::XlenMode::XL32)
                if (swizzle<__uint32_t, CB_SHAMT>(inst) & 1 << 5)
                    return ex_reserved;
            if (mxlen == RISCV::XlenMode::XL128)
                return ex_csrai_128<XLEN_t, out>;
            return ex_csrai<XLEN_t, out>;
        case 2: return ex_candi<XLEN_t, out>;
        case 3:
            switch(swizzle<__uint32_t, ExtendBits::Zero, 12, 12, 6, 5>(inst)) {
            case 0: return ex_csub<XLEN_t, out>;
            case 1: return ex_cxor<XLEN_t, out>;
            case 2: return ex_cor<XLEN_t, out>;
            case 3: return ex_cand<XLEN_t, out>;
            case 4: return ex_unimplemented; // C.SUBW TODO
            case 5: return ex_unimplemented; // C.ADDW TODO
            case 6: return ex_reserved;
            case 7: return ex_reserved;
            default: return ex_illegal;
            }
        default: return ex_illegal;
        }
    case 5: return ex_cj<XLEN_t, out>;
    case 6: return ex_cbeqz<XLEN_t, out>;
    case 7: return ex_cbnez<XLEN_t, out>;
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_compressed_q2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0:
        if (mxlen == RISCV::XlenMode::XL32)
            if (swizzle<__uint32_t, CI_SHAMT>(inst) & 1 << 5)
                return ex_reserved;
        if (mxlen == RISCV::XlenMode::XL128)
            return ex_cslli_128<XLEN_t, out>;
        return ex_cslli<XLEN_t, out>;
    case 1:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return ex_unimplemented; // C.FLDSP TODO
        return ex_unimplemented; // C.LQSP TODO
    case 2: return ex_clwsp<XLEN_t, out>;
    case 3: return ex_unimplemented; // C.FLWSP C.LDSP TODO
    case 4:
        if (inst & 1 << 12) {
            if (swizzle<__uint32_t, ExtendBits::Zero, 6, 2>(inst) == 0) {
                if (swizzle<__uint32_t, ExtendBits::Zero, 11, 7>(inst) == 0)
                    return ex_cebreak<XLEN_t, out>;
                return ex_cjalr<XLEN_t, out>;
            }
            return ex_cadd<XLEN_t, out>;
        } else {
            if (swizzle<__uint32_t, ExtendBits::Zero, 6, 2>(inst) == 0)
                return ex_cjr<XLEN_t, out>;
            return ex_cmv<XLEN_t, out>;
        }
    case 5: return ex_unimplemented; // C.FSDSP C.SQSP TODO
    case 6: return ex_cswsp<XLEN_t, out>;
    case 7: return ex_unimplemented; // C.FSWSP C.SDSP TODO
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_uncompressed(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, OPCODE>(inst)) {
    case RISCV::MajorOpcode::LOAD: return decode_load<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LOAD_FP: return decode_load_fp<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_0: return decode_custom_0<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MISC_MEM: return decode_misc_mem<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP_IMM: return decode_op_imm<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::AUIPC: return ex_auipc<XLEN_t, out>;
    case RISCV::MajorOpcode::OP_IMM_32: return decode_op_imm_32<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_48B_1: return decode_long_48b_1<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::STORE: return decode_store<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::STORE_FP: return decode_store_fp<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_1: return decode_custom_1<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::AMO: return decode_amo<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP: return decode_op<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LUI: return ex_lui<XLEN_t, out>;
    case RISCV::MajorOpcode::OP_32: return decode_op_32<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_64B: return decode_long_64b<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MADD: return decode_madd<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MSUB: return decode_msub<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::NMSUB: return decode_nmsub<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::NMADD: return decode_nmadd<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP_FP: return decode_op_fp<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::RESERVED_0: return decode_reserved_0<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_2: return decode_custom_2<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_48B_2: return decode_long_48b_2<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::BRANCH: return decode_branch<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::JALR: return ex_jalr<XLEN_t, out>;
    case RISCV::MajorOpcode::RESERVED_1: return decode_reserved_1<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::JAL: return ex_jal<XLEN_t, out>;
    case RISCV::MajorOpcode::SYSTEM: return decode_system<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::RESERVED_2: return decode_reserved_2<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_3: return decode_custom_3<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_80B: return decode_long_80b<XLEN_t, out>(inst, extensionsVector, mxlen);
    default: return ex_illegal;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
constexpr DecodedInstruction<XLEN_t> decode_instruction(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, QUADRANT>(inst)) {
    case RISCV::OpcodeQuadrant::UNCOMPRESSED: return decode_uncompressed<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q0: return decode_compressed_q0<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q1: return decode_compressed_q1<XLEN_t, out>(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q2: return decode_compressed_q2<XLEN_t, out>(inst, extensionsVector, mxlen);
    default: return ex_illegal;
    }
}
