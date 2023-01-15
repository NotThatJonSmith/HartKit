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

CREATE_INSTRUCTION(add)
CREATE_INSTRUCTION(sub)
CREATE_INSTRUCTION(sll)
CREATE_INSTRUCTION(slt)
CREATE_INSTRUCTION(sltu)
CREATE_INSTRUCTION(xor)
CREATE_INSTRUCTION(sra)
CREATE_INSTRUCTION(srl)
CREATE_INSTRUCTION(or)
CREATE_INSTRUCTION(and)
CREATE_INSTRUCTION(addi)
CREATE_INSTRUCTION(slli)
CREATE_INSTRUCTION(srli)
CREATE_INSTRUCTION(srai)
CREATE_INSTRUCTION(slti)
CREATE_INSTRUCTION(sltiu)
CREATE_INSTRUCTION(xori)
CREATE_INSTRUCTION(ori)
CREATE_INSTRUCTION(andi)
CREATE_INSTRUCTION(lui)
CREATE_INSTRUCTION(auipc)
CREATE_INSTRUCTION(jal)
CREATE_INSTRUCTION(jalr)
CREATE_INSTRUCTION(beq)
CREATE_INSTRUCTION(bne)
CREATE_INSTRUCTION(blt)
CREATE_INSTRUCTION(bge)
CREATE_INSTRUCTION(bltu)
CREATE_INSTRUCTION(bgeu)
CREATE_INSTRUCTION(lb)
CREATE_INSTRUCTION(lh)
CREATE_INSTRUCTION(lw)
CREATE_INSTRUCTION(lbu)
CREATE_INSTRUCTION(lhu)
CREATE_INSTRUCTION(sb)
CREATE_INSTRUCTION(sh)
CREATE_INSTRUCTION(sw)
CREATE_INSTRUCTION(fence)
CREATE_INSTRUCTION(fencei)
CREATE_INSTRUCTION(ecall)
CREATE_INSTRUCTION(ebreak)
CREATE_INSTRUCTION(csrrw)
CREATE_INSTRUCTION(csrrs)
CREATE_INSTRUCTION(csrrc)
CREATE_INSTRUCTION(csrrwi)
CREATE_INSTRUCTION(csrrsi)
CREATE_INSTRUCTION(csrrci)
CREATE_INSTRUCTION(illegal)
CREATE_INSTRUCTION(reserved)
CREATE_INSTRUCTION(unimplemented)
CREATE_INSTRUCTION(lrw)
CREATE_INSTRUCTION(lrd)
CREATE_INSTRUCTION(scw)
CREATE_INSTRUCTION(scd)
CREATE_INSTRUCTION(amoaddw)
CREATE_INSTRUCTION(amoaddd)
CREATE_INSTRUCTION(amoswapw)
CREATE_INSTRUCTION(amoswapd)
CREATE_INSTRUCTION(amoxorw)
CREATE_INSTRUCTION(amoxord)
CREATE_INSTRUCTION(amoorw)
CREATE_INSTRUCTION(amoord)
CREATE_INSTRUCTION(amoandw)
CREATE_INSTRUCTION(amoandd)
CREATE_INSTRUCTION(amominw)
CREATE_INSTRUCTION(amomind)
CREATE_INSTRUCTION(amomaxw)
CREATE_INSTRUCTION(amomaxd)
CREATE_INSTRUCTION(amominuw)
CREATE_INSTRUCTION(amominud)
CREATE_INSTRUCTION(amomaxuw)
CREATE_INSTRUCTION(amomaxud)
CREATE_INSTRUCTION(caddi4spn)
CREATE_INSTRUCTION(clw)
CREATE_INSTRUCTION(csw)
CREATE_INSTRUCTION(caddi)
CREATE_INSTRUCTION(cjal)
CREATE_INSTRUCTION(cli)
CREATE_INSTRUCTION(clui)
CREATE_INSTRUCTION(caddi16sp)
CREATE_INSTRUCTION(csub)
CREATE_INSTRUCTION(cxor)
CREATE_INSTRUCTION(cor)
CREATE_INSTRUCTION(cand)
CREATE_INSTRUCTION(cj)
CREATE_INSTRUCTION(cbeqz)
CREATE_INSTRUCTION(cbnez)
CREATE_INSTRUCTION(candi)
CREATE_INSTRUCTION(cslli)
CREATE_INSTRUCTION(cslli_128)
CREATE_INSTRUCTION(clwsp)
CREATE_INSTRUCTION(cswsp)
CREATE_INSTRUCTION(cjalr)
CREATE_INSTRUCTION(cadd)
CREATE_INSTRUCTION(cjr)
CREATE_INSTRUCTION(cmv)
CREATE_INSTRUCTION(cebreak)
CREATE_INSTRUCTION(csrli)
CREATE_INSTRUCTION(csrli_128)
CREATE_INSTRUCTION(csrai)
CREATE_INSTRUCTION(csrai_128)
CREATE_INSTRUCTION(mul)
CREATE_INSTRUCTION(mulh)
CREATE_INSTRUCTION(mulhsu)
CREATE_INSTRUCTION(mulhu)
CREATE_INSTRUCTION(div)
CREATE_INSTRUCTION(divu)
CREATE_INSTRUCTION(rem)
CREATE_INSTRUCTION(remu)
CREATE_INSTRUCTION(wfi)
CREATE_INSTRUCTION(uret)
CREATE_INSTRUCTION(sret)
CREATE_INSTRUCTION(mret)
CREATE_INSTRUCTION(sfencevma)

#define QUADRANT    ExtendBits::Zero, 1, 0
#define OPCODE      ExtendBits::Zero, 6, 2
#define OP_MINOR    ExtendBits::Zero, 31, 25, 14, 12
#define FUNCT3      ExtendBits::Zero, 14, 12
#define FUNCT7      ExtendBits::Zero, 31, 25
#define SHAMT       ExtendBits::Zero, 24, 20
#define FUNCT5      ExtendBits::Zero, 31, 27

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_load(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::LB: return inst_lb<XLEN_t>;
    case RISCV::MinorOpcode::LH: return inst_lh<XLEN_t>;
    case RISCV::MinorOpcode::LW: return inst_lw<XLEN_t>;
    case RISCV::MinorOpcode::LBU: return inst_lbu<XLEN_t>;
    case RISCV::MinorOpcode::LHU: return inst_lhu<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_load_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_custom_0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_misc_mem(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::FENCE: return inst_fence<XLEN_t>;
    case RISCV::MinorOpcode::FENCE_I: return inst_fencei<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_op_imm(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::ADDI: return inst_addi<XLEN_t>;
    case RISCV::MinorOpcode::SLLI: return inst_slli<XLEN_t>;
    case RISCV::MinorOpcode::SLTI: return inst_slti<XLEN_t>;
    case RISCV::MinorOpcode::SLTIU: return inst_sltiu<XLEN_t>;
    case RISCV::MinorOpcode::XORI: return inst_xori<XLEN_t>;
    case RISCV::MinorOpcode::SRI:
        switch(swizzle<__uint32_t, FUNCT7>(inst)) {
        case RISCV::SubMinorOpcode::SRAI: return inst_srai<XLEN_t>;
        case RISCV::SubMinorOpcode::SRLI: return inst_srli<XLEN_t>;
        default: return inst_illegal<XLEN_t>;
        }
    case RISCV::MinorOpcode::ORI: return inst_ori<XLEN_t>;
    case RISCV::MinorOpcode::ANDI: return inst_andi<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_op_imm_32(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) { // TODO op_imm, __uint32_t extensionsVector_32
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_long_48b_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_store(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::SB: return inst_sb<XLEN_t>;
    case RISCV::MinorOpcode::SH: return inst_sh<XLEN_t>;
    case RISCV::MinorOpcode::SW: return inst_sw<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_store_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) { // TODO store, __uint32_t extensionsVector_fp
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_custom_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_amo(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::AmoWidth::AMO_W:
        switch (swizzle<__uint32_t, FUNCT5>(inst)) {
        case RISCV::MinorOpcode::AMOADD: return inst_amoaddw<XLEN_t>;
        case RISCV::MinorOpcode::AMOSWAP: return inst_amoswapw<XLEN_t>;
        case RISCV::MinorOpcode::LR: return inst_lrw<XLEN_t>;
        case RISCV::MinorOpcode::SC: return inst_scw<XLEN_t>;
        case RISCV::MinorOpcode::AMOXOR: return inst_amoxorw<XLEN_t>;
        case RISCV::MinorOpcode::AMOOR: return inst_amoorw<XLEN_t>;
        case RISCV::MinorOpcode::AMOAND: return inst_amoandw<XLEN_t>;
        case RISCV::MinorOpcode::AMOMIN: return inst_amominw<XLEN_t>;
        case RISCV::MinorOpcode::AMOMAX: return inst_amomaxw<XLEN_t>;
        case RISCV::MinorOpcode::AMOMINU: return inst_amominuw<XLEN_t>;
        case RISCV::MinorOpcode::AMOMAXU: return inst_amomaxuw<XLEN_t>;
        default: return inst_illegal<XLEN_t>;
        }
    case RISCV::AmoWidth::AMO_D:
        switch (swizzle<__uint32_t, FUNCT5>(inst)) {
        case RISCV::MinorOpcode::AMOADD: return inst_amoaddd<XLEN_t>;
        case RISCV::MinorOpcode::AMOSWAP: return inst_amoswapd<XLEN_t>;
        case RISCV::MinorOpcode::LR: return inst_lrd<XLEN_t>;
        case RISCV::MinorOpcode::SC: return inst_scd<XLEN_t>;
        case RISCV::MinorOpcode::AMOXOR: return inst_amoxord<XLEN_t>;
        case RISCV::MinorOpcode::AMOOR: return inst_amoord<XLEN_t>;
        case RISCV::MinorOpcode::AMOAND: return inst_amoandd<XLEN_t>;
        case RISCV::MinorOpcode::AMOMIN: return inst_amomind<XLEN_t>;
        case RISCV::MinorOpcode::AMOMAX: return inst_amomaxd<XLEN_t>;
        case RISCV::MinorOpcode::AMOMINU: return inst_amominud<XLEN_t>;
        case RISCV::MinorOpcode::AMOMAXU: return inst_amomaxud<XLEN_t>;
        default: return inst_illegal<XLEN_t>;
        }
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_op(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, OP_MINOR>(inst)) {
    case RISCV::MinorOpcode::ADD: return inst_add<XLEN_t>;
    case RISCV::MinorOpcode::SUB: return inst_sub<XLEN_t>;
    case RISCV::MinorOpcode::SLL: return inst_sll<XLEN_t>;
    case RISCV::MinorOpcode::SLT: return inst_slt<XLEN_t>;
    case RISCV::MinorOpcode::SLTU: return inst_sltu<XLEN_t>;
    case RISCV::MinorOpcode::XOR: return inst_xor<XLEN_t>;
    case RISCV::MinorOpcode::SRA: return inst_sra<XLEN_t>;
    case RISCV::MinorOpcode::SRL: return inst_srl<XLEN_t>;
    case RISCV::MinorOpcode::OR: return inst_or<XLEN_t>;
    case RISCV::MinorOpcode::AND: return inst_and<XLEN_t>;
    case RISCV::MinorOpcode::MUL: return inst_mul<XLEN_t>;
    case RISCV::MinorOpcode::MULH: return inst_mulh<XLEN_t>;
    case RISCV::MinorOpcode::MULHSU: return inst_mulhsu<XLEN_t>;
    case RISCV::MinorOpcode::MULHU: return inst_mulhu<XLEN_t>;
    case RISCV::MinorOpcode::DIV: return inst_div<XLEN_t>;
    case RISCV::MinorOpcode::DIVU: return inst_divu<XLEN_t>;
    case RISCV::MinorOpcode::REM: return inst_rem<XLEN_t>;
    case RISCV::MinorOpcode::REMU: return inst_remu<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_op_32(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_long_64b(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_madd(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_msub(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_nmsub(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_nmadd(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_op_fp(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_reserved_0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}
template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_custom_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_long_48b_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_branch(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::BEQ: return inst_beq<XLEN_t>;
    case RISCV::MinorOpcode::BNE: return inst_bne<XLEN_t>;
    case RISCV::MinorOpcode::BLT: return inst_blt<XLEN_t>;
    case RISCV::MinorOpcode::BGE: return inst_bge<XLEN_t>;
    case RISCV::MinorOpcode::BLTU: return inst_bltu<XLEN_t>;
    case RISCV::MinorOpcode::BGEU: return inst_bgeu<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_reserved_1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_system(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, FUNCT3>(inst)) {
    case RISCV::MinorOpcode::PRIV:
        switch (swizzle<__uint32_t, FUNCT7>(inst)) {
        case RISCV::SubMinorOpcode::ECALL_EBREAK_URET:
            switch (swizzle<__uint32_t, RS2>(inst)) {
            case RISCV::SubSubMinorOpcode::ECALL: return inst_ecall<XLEN_t>;
            case RISCV::SubSubMinorOpcode::EBREAK: return inst_ebreak<XLEN_t>;
            case RISCV::SubSubMinorOpcode::URET: return inst_uret<XLEN_t>;
            default: return inst_illegal<XLEN_t>;
            }
        case RISCV::SubMinorOpcode::SRET_WFI:
            switch (swizzle<__uint32_t, RS2>(inst)) {
            case RISCV::SubSubMinorOpcode::WFI: return inst_wfi<XLEN_t>;
            case RISCV::SubSubMinorOpcode::SRET: return inst_sret<XLEN_t>;
            default: return inst_illegal<XLEN_t>;
            }
        case RISCV::SubMinorOpcode::MRET: return inst_mret<XLEN_t>;
        case RISCV::SFENCE_VMA: return inst_sfencevma<XLEN_t>;
        default: return inst_illegal<XLEN_t>;
        }
    case RISCV::MinorOpcode::CSRRW: return inst_csrrw<XLEN_t>;
    case RISCV::MinorOpcode::CSRRS: return inst_csrrs<XLEN_t>;
    case RISCV::MinorOpcode::CSRRC: return inst_csrrc<XLEN_t>;
    case RISCV::MinorOpcode::CSRRWI: return inst_csrrwi<XLEN_t>;
    case RISCV::MinorOpcode::CSRRSI: return inst_csrrsi<XLEN_t>;
    case RISCV::MinorOpcode::CSRRCI: return inst_csrrci<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_reserved_2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_custom_3(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_long_80b(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    return inst_unimplemented<XLEN_t>;
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_compressed_q0(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0:
        if (swizzle<__uint32_t, ExtendBits::Zero, 10, 7, 12, 11, 5, 5, 6, 6, 2>(inst) == 0)
            return inst_illegal<XLEN_t>;
        return inst_caddi4spn<XLEN_t>;
    case 1:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return inst_unimplemented<XLEN_t>; // C.FLD TODO
        return inst_unimplemented<XLEN_t>; // C.LQ TODO
    case 2: return inst_clw<XLEN_t>;
    case 3:
        if (mxlen == RISCV::XlenMode::XL32)
            return inst_unimplemented<XLEN_t>; // C.FLW TODO
        return inst_unimplemented<XLEN_t>; // C.LD TODO
    case 4: return inst_reserved<XLEN_t>;
    case 5:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return inst_unimplemented<XLEN_t>; // C.FSD TODO
        return inst_unimplemented<XLEN_t>; // C.SQ TODO
    case 6: return inst_csw<XLEN_t>;
    case 7:
        if (mxlen == RISCV::XlenMode::XL32)
            return inst_unimplemented<XLEN_t>; // C.FSW TODO
        return inst_unimplemented<XLEN_t>; // C.SD TODO
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_compressed_q1(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0: return inst_caddi<XLEN_t>; // Note that NOP is the same instruction
    case 1:
        if (mxlen == RISCV::XlenMode::XL32)
            return inst_cjal<XLEN_t>;
        return inst_unimplemented<XLEN_t>; // C.ADDIW TODO
    case 2: return inst_cli<XLEN_t>;
    case 3:
        if (swizzle<__uint32_t, CI_RD_RS1>(inst) != 2)
            return inst_clui<XLEN_t>;
        if (swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 4, 3, 5, 5, 2, 2, 6, 6, 4>(inst) != 0)
            return inst_caddi16sp<XLEN_t>;
        return inst_reserved<XLEN_t>;
    case 4:
        switch (swizzle<__uint32_t, ExtendBits::Zero, 11, 10>(inst)) {
        case 0:
            if (mxlen == RISCV::XlenMode::XL32)
                if (swizzle<__uint32_t, CB_SHAMT>(inst) & 1 << 5)
                    return inst_reserved<XLEN_t>;
            if (mxlen == RISCV::XlenMode::XL128)
                return inst_csrli_128<XLEN_t>;
            return inst_csrli<XLEN_t>;
        case 1:
            if (mxlen == RISCV::XlenMode::XL32)
                if (swizzle<__uint32_t, CB_SHAMT>(inst) & 1 << 5)
                    return inst_reserved<XLEN_t>;
            if (mxlen == RISCV::XlenMode::XL128)
                return inst_csrai_128<XLEN_t>;
            return inst_csrai<XLEN_t>;
        case 2: return inst_candi<XLEN_t>;
        case 3:
            switch(swizzle<__uint32_t, ExtendBits::Zero, 12, 12, 6, 5>(inst)) {
            case 0: return inst_csub<XLEN_t>;
            case 1: return inst_cxor<XLEN_t>;
            case 2: return inst_cor<XLEN_t>;
            case 3: return inst_cand<XLEN_t>;
            case 4: return inst_unimplemented<XLEN_t>; // C.SUBW TODO
            case 5: return inst_unimplemented<XLEN_t>; // C.ADDW TODO
            case 6: return inst_reserved<XLEN_t>;
            case 7: return inst_reserved<XLEN_t>;
            default: return inst_illegal<XLEN_t>;
            }
        default: return inst_illegal<XLEN_t>;
        }
    case 5: return inst_cj<XLEN_t>;
    case 6: return inst_cbeqz<XLEN_t>;
    case 7: return inst_cbnez<XLEN_t>;
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_compressed_q2(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch(swizzle<__uint32_t, C_FUNCT3>(inst)) {
    case 0:
        if (mxlen == RISCV::XlenMode::XL32)
            if (swizzle<__uint32_t, CI_SHAMT>(inst) & 1 << 5)
                return inst_reserved<XLEN_t>;
        if (mxlen == RISCV::XlenMode::XL128)
            return inst_cslli_128<XLEN_t>;
        return inst_cslli<XLEN_t>;
    case 1:
        if (mxlen == RISCV::XlenMode::XL32 || mxlen == RISCV::XlenMode::XL64)
            return inst_unimplemented<XLEN_t>; // C.FLDSP TODO
        return inst_unimplemented<XLEN_t>; // C.LQSP TODO
    case 2: return inst_clwsp<XLEN_t>;
    case 3: return inst_unimplemented<XLEN_t>; // C.FLWSP C.LDSP TODO
    case 4:
        if (inst & 1 << 12) {
            if (swizzle<__uint32_t, ExtendBits::Zero, 6, 2>(inst) == 0) {
                if (swizzle<__uint32_t, ExtendBits::Zero, 11, 7>(inst) == 0)
                    return inst_cebreak<XLEN_t>;
                return inst_cjalr<XLEN_t>;
            }
            return inst_cadd<XLEN_t>;
        } else {
            if (swizzle<__uint32_t, ExtendBits::Zero, 6, 2>(inst) == 0)
                return inst_cjr<XLEN_t>;
            return inst_cmv<XLEN_t>;
        }
    case 5: return inst_unimplemented<XLEN_t>; // C.FSDSP C.SQSP TODO
    case 6: return inst_cswsp<XLEN_t>;
    case 7: return inst_unimplemented<XLEN_t>; // C.FSWSP C.SDSP TODO
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_uncompressed(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, OPCODE>(inst)) {
    case RISCV::MajorOpcode::LOAD: return decode_load<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LOAD_FP: return decode_load_fp<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_0: return decode_custom_0<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MISC_MEM: return decode_misc_mem<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP_IMM: return decode_op_imm<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::AUIPC: return inst_auipc<XLEN_t>;
    case RISCV::MajorOpcode::OP_IMM_32: return decode_op_imm_32<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_48B_1: return decode_long_48b_1<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::STORE: return decode_store<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::STORE_FP: return decode_store_fp<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_1: return decode_custom_1<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::AMO: return decode_amo<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP: return decode_op<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LUI: return inst_lui<XLEN_t>;
    case RISCV::MajorOpcode::OP_32: return decode_op_32<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_64B: return decode_long_64b<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MADD: return decode_madd<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::MSUB: return decode_msub<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::NMSUB: return decode_nmsub<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::NMADD: return decode_nmadd<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::OP_FP: return decode_op_fp<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::RESERVED_0: return decode_reserved_0<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_2: return decode_custom_2<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_48B_2: return decode_long_48b_2<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::BRANCH: return decode_branch<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::JALR: return inst_jalr<XLEN_t>;
    case RISCV::MajorOpcode::RESERVED_1: return decode_reserved_1<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::JAL: return inst_jal<XLEN_t>;
    case RISCV::MajorOpcode::SYSTEM: return decode_system<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::RESERVED_2: return decode_reserved_2<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::CUSTOM_3: return decode_custom_3<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::MajorOpcode::LONG_80B: return decode_long_80b<XLEN_t>(inst, extensionsVector, mxlen);
    default: return inst_illegal<XLEN_t>;
    }
}

template<typename XLEN_t>
constexpr Instruction<XLEN_t> decode_instruction(__uint32_t inst, __uint32_t extensionsVector, RISCV::XlenMode mxlen) {
    switch (swizzle<__uint32_t, QUADRANT>(inst)) {
    case RISCV::OpcodeQuadrant::UNCOMPRESSED: return decode_uncompressed<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q0: return decode_compressed_q0<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q1: return decode_compressed_q1<XLEN_t>(inst, extensionsVector, mxlen);
    case RISCV::OpcodeQuadrant::Q2: return decode_compressed_q2<XLEN_t>(inst, extensionsVector, mxlen);
    default: return inst_illegal<XLEN_t>;
    }
}
