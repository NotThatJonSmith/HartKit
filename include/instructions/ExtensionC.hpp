#pragma once

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

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_caddi4spn(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CIW_RDX>(state->inst)+8;
    __uint32_t rs1 = 2;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 10, 7, 12, 11, 5, 5, 6, 6, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.ADDI4SPN) addi "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    XLEN_t rd_value = rs1_value + imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

// TODO endianness-agnostic impl; for now x86 and RV being both LE save us
template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_clw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CL_RDX>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CL_RS1X>(state->inst)+8;
    // Yes, this is correct. It's zero extended into the signed immediate.
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.LW) lw "
             << RISCV::regName(rd) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    __uint32_t word;
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    XLEN_t read_address = rs1_value + imm_value;
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? word : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = swizzle<__uint32_t, CS_RS1X>(state->inst)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CS_RS2X>(state->inst)+8;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.SW) sw "
             << RISCV::regName(rs2) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    SXLEN_t imm_value = imm;
    XLEN_t write_addr = rs1_value + imm_value;
    __uint32_t write_value = rs2_value & (XLEN_t)0xffffffff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_caddi(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.ADDI) addi "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    XLEN_t rd_value = rs1_value + imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cjal(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = 1;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.JAL) jal "
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
inline void ex_cli(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __uint32_t rs1 = 0;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.LI) addi "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    XLEN_t rd_value = rs1_value + imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_clui(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2, 12>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.LUI) lui "
             << RISCV::regName(rd) << ", "
             << (imm >> 12) << std::endl;
        return;
    }
    XLEN_t imm_value = imm;
    XLEN_t rd_value = imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_caddi16sp(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = 2;
    __uint32_t rs1 = 2;
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 4, 3, 5, 5, 2, 2, 6, 6, 4>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.ADDI16SP) addi "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    XLEN_t rd_value = rs1_value + imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csub(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(state->inst)+8;
    if constexpr (out != nullptr) {
        *out << "(C.SUB) sub "
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
inline void ex_cxor(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(state->inst)+8;
    if constexpr (out != nullptr) {
        *out << "(C.XOR) xor "
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
inline void ex_cor(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(state->inst)+8;
    if constexpr (out != nullptr) {
        *out << "(C.OR) or "
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
inline void ex_cand(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(state->inst)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(state->inst)+8;
    if constexpr (out != nullptr) {
        *out << "(C.AND) and "
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
inline void ex_cj(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(state->inst);
    __uint32_t rd = 0;
    if constexpr (out != nullptr) {
        *out << "(C.J) jal "
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
inline void ex_cbeqz(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t rs2 = 0;
    if constexpr (out != nullptr) {
        *out << "(C.BEQZ) beq "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    SXLEN_t imm_value = imm;
    if (rs1_value == rs2_value) {
        XLEN_t new_pc_value = state->pc + imm_value;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cbnez(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t rs2 = 0;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.BNEZ) bne "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << ", "
             << imm << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    SXLEN_t imm_value = imm;
    if (rs1_value != rs2_value) {
        XLEN_t new_pc_value = state->pc + imm_value;
        state->nextPC = new_pc_value;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_candi(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __int32_t imm = swizzle<__int32_t, ExtendBits::Sign, 12, 12, 6, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.ANDI) andi "
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
inline void ex_cslli(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t imm = swizzle<__uint32_t, CI_SHAMT>(state->inst);
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __uint32_t rs1 = rd;
    if constexpr (out != nullptr) {
        *out << "(C.SLLI) slli "
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
inline void ex_cslli_128(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t imm = swizzle<__uint32_t, CI_SHAMT>(state->inst);
    imm = imm == 0 ? 64 : imm;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __uint32_t rs1 = rd;
    if constexpr (out != nullptr) {
        *out << "(C.SLLI) slli"
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
inline void ex_clwsp(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __uint32_t rs1 = 2;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 3, 2, 12, 12, 6, 4, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.LWSP) lw "
             << RISCV::regName(rd) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    __uint32_t word;
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    XLEN_t read_address = rs1_value + imm_value;
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&word);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? word : 0;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cswsp(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = 2;
    __uint32_t rs2 = swizzle<__uint32_t, CSS_RS2>(state->inst);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 8, 7, 12, 9, 2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.SWSP) sw "
             << RISCV::regName(rs2) << ",("
             << imm << ")"
             << RISCV::regName(rs1) << std::endl;
        return;
    }
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    SXLEN_t imm_value = imm;
    XLEN_t write_addr = rs1_value + imm_value;
    __uint32_t write_value = rs2_value & (XLEN_t)0xffffffff;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cjalr(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __int32_t imm = 0;
    __uint32_t rd = 1;
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.JALR) jalr "
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
inline void ex_cadd(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CR_RD_RS1>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, CR_RD_RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, CR_RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.ADD) add "
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
inline void ex_cjr(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(state->inst);
    __int32_t imm = 0;
    __uint32_t rd = 0;
    if constexpr (out != nullptr) {
        *out << "(C.JR) jalr "
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
inline void ex_cmv(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CR_RD_RS1>(state->inst);
    __uint32_t rs1 = 0;
    __uint32_t rs2 = swizzle<__uint32_t, CR_RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.MV) add "
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
inline void ex_cebreak(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    if constexpr (out != nullptr) {
        *out << "(C.EBREAK) ebreak";
        return;
    }
    state->RaiseException(RISCV::TrapCause::BREAKPOINT, state->inst);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrli(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.SRLI) srli "
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
inline void ex_csrli_128(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(state->inst);
    imm = imm == 0 ? 64 : imm;
    if constexpr (out != nullptr) {
        *out << "(C.SRLI) srli "
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
inline void ex_csrai(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(state->inst);
    if constexpr (out != nullptr) {
        *out << "(C.SRAI) srai "
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
inline void ex_csrai_128(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(state->inst)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(state->inst);
    imm = imm == 0 ? 64 : imm;
    if constexpr (out != nullptr) {
        *out << "(C.SRAI) srai "
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
