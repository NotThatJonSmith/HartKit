#pragma once

#include <Swizzle.hpp>

#include <RiscV.hpp>

#include <HartState.hpp>
#include <Transactor.hpp>

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
inline void ex_add(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value + rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sub(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value - rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sll(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value << rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_slt(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    XLEN_t rd_value = rs1_value < rs2_value ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sltu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value < rs2_value ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_xor(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value ^ rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sra(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value >> rs2_value;
    constexpr XLEN_t xlen_bits = sizeof(XLEN_t) * 8;
    if (rs1_value & ((XLEN_t)1 << (xlen_bits - 1))) {
        rd_value |= ((1 << rs2_value)-1) << (xlen_bits-rs2_value);
    }
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_srl(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value >> rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_or(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value | rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_and(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value & rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_addi(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value + imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_slli(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value << imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_srli(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value >> imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_srai(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    __uint16_t imm_value = imm;
    XLEN_t rd_value = rs1_value >> imm_value;
    // Spec says: the original sign bit is copied into the vacated upper bits
    if (rs1_value & ((XLEN_t)1 << ((sizeof(XLEN_t)*8)-1)))
        rd_value |= (XLEN_t)((1 << imm_value)-1) << ((sizeof(XLEN_t)*8)-imm_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_slti(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t rd_value = rs1_value < imm ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sltiu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t imm_value = imm;
    XLEN_t rd_value = rs1_value < imm_value ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0; 
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_xori(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value_signed = imm;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value ^ imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_ori(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value_signed = imm;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value | imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_andi(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value_signed = imm;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value & imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_lui(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, U_IMM>(encoding);
    XLEN_t imm_value = imm;
    XLEN_t rd_value = imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_auipc(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, U_IMM>(encoding);
    XLEN_t pc_value = state->pc;
    XLEN_t imm_value = imm;
    XLEN_t rd_value = pc_value + imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_jal(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, J_IMM>(encoding);
    state->regs[rd] = rd != 0 ? (state->pc + 4) : 0;
    state->pc = state->pc + imm;
}

template<typename XLEN_t>
inline void ex_jalr(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    imm_value &= ~(XLEN_t)1;
    state->regs[rd] = rd != 0 ? (state->pc + 4) : 0;
    state->pc = rs1_value + imm_value;
}

template<typename XLEN_t>
inline void ex_beq(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    if (rs1_value == rs2_value) {
        state->pc = state->pc + imm;
    } else {
        state->pc += 4;
    }
}

template<typename XLEN_t>
inline void ex_bne(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    if (rs1_value != rs2_value) {
        state->pc = state->pc + imm;
    } else {
        state->pc += 4;
    }
}

template<typename XLEN_t>
inline void ex_blt(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    if (rs1_value < rs2_value) {
        state->pc = state->pc + imm;
    } else {
        state->pc += 4;
    }
}

template<typename XLEN_t>
inline void ex_bge(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    if (rs1_value >= rs2_value) {
        state->pc = state->pc + imm;
    } else {
        state->pc += 4;
    }
}

template<typename XLEN_t>
inline void ex_bltu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    if (unsigned_rs1_value < unsigned_rs2_value) {
        state->pc = state->pc + imm;
    } else {
        state->pc += 4;
    }
}

template<typename XLEN_t>
inline void ex_bgeu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(encoding);
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    if (unsigned_rs1_value >= unsigned_rs2_value) {
        state->pc = state->pc + imm;
    } else {
        state->pc += 4;
    }
}

template<typename XLEN_t>
inline void ex_lb(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    __uint8_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 1;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t sign_extended_word = (SXLEN_t)word;
    state->regs[rd] = rd != 0 ? sign_extended_word : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_lh(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    __uint16_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 2;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t sign_extended_word = (SXLEN_t)word;
    state->regs[rd] = rd != 0 ? sign_extended_word : 0;
    state->pc += 4;
}

// TODO endianness-agnostic impl; for now x86 and RV being both LE save us
template<typename XLEN_t>
inline void ex_lw(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    __uint32_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? word : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_lbu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    __uint8_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 1;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t signed_word = word;
    XLEN_t unsigned_word = *((XLEN_t*)(&signed_word));
    state->regs[rd] = rd != 0 ? unsigned_word : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_lhu(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(encoding);
    __uint16_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 2;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    SXLEN_t signed_word = word;
    XLEN_t unsigned_word = *((XLEN_t*)(&signed_word));
    state->regs[rd] = rd != 0 ? unsigned_word : 0;
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sb(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t write_addr = rs1_value + imm;
    __uint8_t write_value = rs2_value & (XLEN_t)0xff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sh(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t write_addr = rs1_value + imm;
    __uint16_t write_value = rs2_value & (XLEN_t)0xffff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_sw(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t write_addr = rs1_value + imm;
    __uint32_t write_value = rs2_value & (XLEN_t)0xffffffff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->pc += 4;
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_fence(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    state->pc += 4;
    // NOP for now.
}

template<typename XLEN_t>
inline void ex_fencei(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    state->implCallback(HartCallbackArgument::RequestedIfence);
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_ecall(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    RISCV::TrapCause cause =
        state->privilegeMode == RISCV::PrivilegeMode::Machine ? RISCV::TrapCause::ECALL_FROM_M_MODE :
        state->privilegeMode == RISCV::PrivilegeMode::Supervisor ? RISCV::TrapCause::ECALL_FROM_S_MODE :
        RISCV::TrapCause::ECALL_FROM_U_MODE;
    state->pc += 4;
    state->RaiseException(cause, encoding);
}

template<typename XLEN_t>
inline void ex_ebreak(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    state->pc += 4;
    state->RaiseException(RISCV::TrapCause::BREAKPOINT, encoding);
}

template<typename XLEN_t>
inline void ex_csrrw(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    XLEN_t regVal = state->regs[rs1];
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        // TODO zero is one option, current encoding is another, spec struct controls. Implement throughout ISA...
        state->pc += 4;
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    if (rd != 0) {
        XLEN_t csrValue = state->ReadCSR(csr);
        state->regs[rd] = rd != 0 ? csrValue : 0;
    }
    state->WriteCSR(csr, regVal);
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_csrrs(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    XLEN_t regVal = state->regs[rs1];
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if ((rs1 != 0 && readOnly) || state->privilegeMode < requiredPrivilege) {
        state->pc += 4;
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    if (rs1 != 0) {
        state->WriteCSR(csr, csrValue | regVal);
    }
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_csrrc(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    XLEN_t regVal = state->regs[rs1];
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if ((rs1 != 0 && readOnly) || state->privilegeMode < requiredPrivilege) {
        state->pc += 4;
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    if (rs1 != 0) {
        state->WriteCSR(csr, ~csrValue & regVal);
    }
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_csrrwi(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    XLEN_t imm_value = rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->pc += 4;
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    if (rd != 0) {
        XLEN_t csrValue = state->ReadCSR(csr);
        state->regs[rd] = rd != 0 ? csrValue : 0;
    }
    state->WriteCSR(csr, imm_value);
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_csrrsi(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    XLEN_t imm_value = rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->pc += 4;
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    state->WriteCSR(csr, csrValue | imm_value);
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_csrrci(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(encoding);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(encoding);
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    XLEN_t imm_value = rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->pc += 4;
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    state->WriteCSR(csr, ~csrValue & imm_value);
    state->pc += 4;
}

template<typename XLEN_t>
inline void ex_illegal(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    state->pc += 4;
    state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, encoding);
}

template<typename XLEN_t>
inline void ex_reserved(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    exit(1);
}

template<typename XLEN_t>
inline void ex_unimplemented(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    exit(1);
}
