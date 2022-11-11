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

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_add(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
            *out << "add "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value + rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sub(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
            *out << "sub "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value - rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sll(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
            *out << "sll "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
             return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value << rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_slt(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
            *out << "slt "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    XLEN_t rd_value = rs1_value < rs2_value ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sltu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
            *out << "sltu "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value < rs2_value ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_xor(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
            *out << "xor "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value ^ rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sra(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
            *out << "sra "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value >> rs2_value;
    constexpr XLEN_t xlen_bits = sizeof(XLEN_t) * 8;
    if (rs1_value & ((XLEN_t)1 << (xlen_bits - 1))) {
        rd_value |= ((1 << rs2_value)-1) << (xlen_bits-rs2_value);
    }
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_srl(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "srl "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value >> rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_or(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "or "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value | rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_and(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "and "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value & rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_addi(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "addi "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value + imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_slli(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(state->inst);
    if constexpr (out != nullptr) {
        *out << "slli "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value << imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_srli(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(state->inst);
    if constexpr (out != nullptr) {
        *out << "srli "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value >> imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_srai(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 24, 20>(state->inst);
    if constexpr (out != nullptr) {
        *out << "srai "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    __uint16_t imm_value = imm;
    XLEN_t rd_value = rs1_value >> imm_value;
    // Spec says: the original sign bit is copied into the vacated upper bits
    if (rs1_value & ((XLEN_t)1 << ((sizeof(XLEN_t)*8)-1)))
        rd_value |= (XLEN_t)((1 << imm_value)-1) << ((sizeof(XLEN_t)*8)-imm_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_slti(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "slti "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t rd_value = rs1_value < imm ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sltiu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "sltiu "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t imm_value = imm;
    XLEN_t rd_value = rs1_value < imm_value ? 1 : 0;
    state->regs[rd] = rd != 0 ? rd_value : 0; 
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_xori(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "xori "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value_signed = imm;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value ^ imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_ori(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "ori "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value_signed = imm;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value | imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_andi(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "andi "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value_signed = imm;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value & imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lui(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, U_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "lui "
             << RISCV::regName(rd) << ", "
             << (imm >> 12) << std::endl;
        return;
    }
    XLEN_t imm_value = imm;
    XLEN_t rd_value = imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_auipc(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Zero, U_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "auipc "
             << RISCV::regName(rd) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t pc_value = state->pc;
    XLEN_t imm_value = imm;
    XLEN_t rd_value = pc_value + imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_jal(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, J_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "jal "
             << RISCV::regName(rd) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t next_pc_value = state->nextPC;
    state->regs[rd] = rd != 0 ? next_pc_value : 0;
    XLEN_t pc_value = state->pc;
    XLEN_t new_pc_value = pc_value + imm;
    state->nextPC = new_pc_value;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_jalr(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "jalr "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rd_value = state->nextPC;
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    imm_value &= ~(XLEN_t)1;
    XLEN_t new_pc_value = rs1_value + imm_value;
    state->nextPC = new_pc_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_beq(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "beq "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    if (rs1_value == rs2_value) {
        XLEN_t new_pc_value = state->pc + imm;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_bne(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "bne "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    if (rs1_value != rs2_value) {
        XLEN_t new_pc_value = state->pc + imm;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_blt(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "blt "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    if (rs1_value < rs2_value) {
        XLEN_t new_pc_value = state->pc + imm;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_bge(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "bge "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    SXLEN_t rs1_value = *((SXLEN_t*)&unsigned_rs1_value);
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    SXLEN_t rs2_value = *((SXLEN_t*)&unsigned_rs2_value);
    if (rs1_value >= rs2_value) {
        XLEN_t new_pc_value = state->pc + imm;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_bltu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "bltu "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    if (unsigned_rs1_value < unsigned_rs2_value) {
        XLEN_t new_pc_value = state->pc + imm;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_bgeu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, B_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "bgeu "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t unsigned_rs1_value = state->regs[rs1];
    XLEN_t unsigned_rs2_value = state->regs[rs2];
    if (unsigned_rs1_value >= unsigned_rs2_value) {
        XLEN_t new_pc_value = state->pc + imm;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lb(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "lb "
             << RISCV::regName(rd) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    __uint8_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 1;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t sign_extended_word = (SXLEN_t)word;
    state->regs[rd] = rd != 0 ? sign_extended_word : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lh(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "lh "
             << RISCV::regName(rd) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    __uint16_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 2;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t sign_extended_word = (SXLEN_t)word;
    state->regs[rd] = rd != 0 ? sign_extended_word : 0;
}

// TODO endianness-agnostic impl; for now x86 and RV being both LE save us
template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "lw "
             << RISCV::regName(rd) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    __uint32_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? word : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lbu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "lbu "
             << RISCV::regName(rd) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    __uint8_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 1;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    SXLEN_t signed_word = word;
    XLEN_t unsigned_word = *((XLEN_t*)(&signed_word));
    state->regs[rd] = rd != 0 ? unsigned_word : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lhu(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "lhu "
             << RISCV::regName(rd) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    __uint16_t word;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t read_address = rs1_value + imm;
    XLEN_t read_size = 2;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    SXLEN_t signed_word = word;
    XLEN_t unsigned_word = *((XLEN_t*)(&signed_word));
    state->regs[rd] = rd != 0 ? unsigned_word : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sb(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "sb "
             << RISCV::regName(rs2) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t write_addr = rs1_value + imm;
    __uint8_t write_value = rs2_value & (XLEN_t)0xff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sh(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "sh "
             << RISCV::regName(rs2) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t write_addr = rs1_value + imm;
    __uint16_t write_value = rs2_value & (XLEN_t)0xffff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_sw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, S_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "sw "
             << RISCV::regName(rs2) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t write_addr = rs1_value + imm;
    __uint32_t write_value = rs2_value & (XLEN_t)0xffffffff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_fence(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "fence"
             << std::endl;
        return;
    }
    // NOP for now.
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_fencei(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "fence.i"
             << std::endl;
        return;
    }
    state->implCallback(HartCallbackArgument::RequestedIfence);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_ecall(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "ecall"
             << std::endl;
        return;
    }
    RISCV::TrapCause cause =
        state->privilegeMode == RISCV::PrivilegeMode::Machine ? RISCV::TrapCause::ECALL_FROM_M_MODE :
        state->privilegeMode == RISCV::PrivilegeMode::Supervisor ? RISCV::TrapCause::ECALL_FROM_S_MODE :
        RISCV::TrapCause::ECALL_FROM_U_MODE;
    state->RaiseException(cause, state->inst);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_ebreak(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "ebreak" << std::endl;
        return;
    }
    state->RaiseException(RISCV::TrapCause::BREAKPOINT, state->inst);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrrw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "csrrw "
             << RISCV::regName(rd) << ", "
             << RISCV::csrName(imm) << ", "
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t regVal = state->regs[rs1];
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        // TODO zero is one option, current encoding is another, spec struct controls. Implement throughout ISA...
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    if (rd != 0) {
        XLEN_t csrValue = state->ReadCSR(csr);
        state->regs[rd] = rd != 0 ? csrValue : 0;
    }
    state->WriteCSR(csr, regVal);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrrs(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "csrrs "
             << RISCV::regName(rd) << ", "
             << RISCV::csrName(imm) << ", "
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t regVal = state->regs[rs1];
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if ((rs1 != 0 && readOnly) || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    if (rs1 != 0) {
        state->WriteCSR(csr, csrValue | regVal);
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrrc(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "csrrc "
             << RISCV::regName(rd) << ", "
             << RISCV::csrName(imm) << ", "
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t regVal = state->regs[rs1];
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if ((rs1 != 0 && readOnly) || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    if (rs1 != 0) {
        state->WriteCSR(csr, ~csrValue & regVal);
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrrwi(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "csrrwi "
             << RISCV::regName(rd) << ", "
             << RISCV::csrName(imm) << ", "
             << rs1 << std::endl;
        return;
    }
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    XLEN_t imm_value = rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    if (rd != 0) {
        XLEN_t csrValue = state->ReadCSR(csr);
        state->regs[rd] = rd != 0 ? csrValue : 0;
    }
    state->WriteCSR(csr, imm_value);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrrsi(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "csrrsi "
             << RISCV::regName(rd) << ", "
             << RISCV::csrName(imm) << ", "
             << rs1 << std::endl;
        return;
    }
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    XLEN_t imm_value = rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    state->WriteCSR(csr, csrValue | imm_value);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrrci(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t imm = (int32_t)swizzle<__uint32_t, ExtendBits::Zero, I_IMM>(state->inst);
    if constexpr (out != nullptr) {
        *out << "csrrci "
             << RISCV::regName(rd) << ", "
             << RISCV::csrName(imm) << ", "
             << rs1 << std::endl;
        return;
    }
    RISCV::CSRAddress csr = (RISCV::CSRAddress)imm;
    XLEN_t imm_value = rs1;
    bool readOnly = RISCV::csrIsReadOnly(csr);
    RISCV::PrivilegeMode requiredPrivilege = RISCV::csrRequiredPrivilege(csr);
    if (readOnly || state->privilegeMode < requiredPrivilege) {
        state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, 0);
        return;
    }
    XLEN_t csrValue = state->ReadCSR(csr);
    state->regs[rd] = rd != 0 ? csrValue : 0;
    state->WriteCSR(csr, ~csrValue & imm_value);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_illegal(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "Illegal Instruction" << std::endl;
        return;
    }
    state->RaiseException(RISCV::TrapCause::ILLEGAL_INSTRUCTION, state->inst);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_reserved(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "Reserved Instruction" << std::endl;
        return;
    }
    std::cout << "RI" << std::endl;
    exit(1);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_unimplemented(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "Unimplemented Instruction" << std::endl;
        return;
    }
    std::cout << "UI" << std::endl;
    exit(1);
}
