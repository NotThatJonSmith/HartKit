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
inline void ex_caddi4spn(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CIW_RDX>(encoding)+8;
    __uint32_t rs1 = 2;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 10, 7, 12, 11, 5, 5, 6, 6, 2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    XLEN_t rd_value = rs1_value + imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_caddi4spn(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CIW_RDX>(encoding)+8;
    __uint32_t rs1 = 2;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 10, 7, 12, 11, 5, 5, 6, 6, 2>(encoding);
    *out << "(C.ADDI4SPN) addi "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

// TODO endianness-agnostic impl; for now x86 and RV being both LE save us
template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_clw(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CL_RDX>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CL_RS1X>(encoding)+8;
    // Yes, this is correct. It's zero extended into the signed immediate.
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(encoding);
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
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_clw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CL_RDX>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CL_RS1X>(encoding)+8;
    // Yes, this is correct. It's zero extended into the signed immediate.
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(encoding);
    *out << "(C.LW) lw "
         << RISCV::regName(rd) << ",("
         << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csw(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, CS_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CS_RS2X>(encoding)+8;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(encoding);
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
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csw(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, CS_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CS_RS2X>(encoding)+8;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(encoding);
    *out << "(C.SW) sw "
         << RISCV::regName(rs2) << ",("
         << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

// TODO, when bringing up 128-bit XLEN, need to reintroduce masking off the low 64 of a 128.
template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csd(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rs1 = swizzle<__uint32_t, CS_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CS_RS2X>(encoding)+8;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t write_addr = rs1_value + imm;
    __uint64_t write_value = rs2_value;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csd(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, CS_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CS_RS2X>(encoding)+8;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 5, 5, 12, 10, 6, 6, 2>(encoding);
    *out << "(C.SD) sd "
         << RISCV::regName(rs2) << ",("
         << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_caddi(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value + imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_caddi(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(encoding);
    *out << "(C.ADDI) addi "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cjal(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(encoding);
    state->regs[1] = state->pc + 2;
    state->pc += imm;
}

template<typename XLEN_t>
inline void print_cjal(__uint32_t encoding, std::ostream* out) {
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(encoding);
    *out << "(C.JAL) jal "
         << RISCV::regName(1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cli(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(encoding);
    SXLEN_t imm_value = imm;
    state->regs[rd] = rd != 0 ? imm_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cli(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2>(encoding);
    *out << "(C.LI) addi "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(0) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_clui(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2, 12>(encoding);
    XLEN_t imm_value = imm;
    state->regs[rd] = rd != 0 ? imm_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_clui(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 2, 12>(encoding);
    *out << "(C.LUI) lui "
         << RISCV::regName(rd) << ", "
         << (imm >> 12) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_caddi16sp(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 4, 3, 5, 5, 2, 2, 6, 6, 4>(encoding);
    state->regs[2] += imm;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_caddi16sp(__uint32_t encoding, std::ostream* out) {
    __int32_t imm = (__int32_t)swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 4, 3, 5, 5, 2, 2, 6, 6, 4>(encoding);
    *out << "(C.ADDI16SP) addi "
         << RISCV::regName(2) << ", "
         << RISCV::regName(2) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csub(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    state->regs[rd] = rd != 0 ? state->regs[rs1] - state->regs[rs2] : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csub(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    *out << "(C.SUB) sub "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cxor(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    state->regs[rd] = rd != 0 ? state->regs[rs1] ^ state->regs[rs2] : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cxor(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    *out << "(C.XOR) xor "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cor(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    state->regs[rd] = rd != 0 ? state->regs[rs1] | state->regs[rs2] : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cor(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    *out << "(C.OR) or "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cand(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    state->regs[rd] = rd != 0 ? state->regs[rs1] & state->regs[rs2] : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cand(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CA_RDX_RS1X>(encoding)+8;
    __uint32_t rs2 = swizzle<__uint32_t, CA_RS2X>(encoding)+8;
    *out << "(C.AND) and "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cj(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(encoding);
    __uint32_t rd = 0;
    state->regs[rd] = rd != 0 ? (state->pc + 2) : 0;
    state->pc += imm;
}

template<typename XLEN_t>
inline void print_cj(__uint32_t encoding, std::ostream* out) {
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 8, 8, 10, 9, 6, 6, 7, 7, 2, 2, 11, 11, 5, 3, 1>(encoding);
    __uint32_t rd = 0;
    *out << "(C.J) jal "
         << RISCV::regName(rd) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cbeqz(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    SXLEN_t imm_value = imm;
    if (state->regs[rs1] == 0) {
        state->pc += imm_value;
    } else {
        state->pc += 2;
    }
}

template<typename XLEN_t>
inline void print_cbeqz(__uint32_t encoding, std::ostream* out) {
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    *out << "(C.BEQZ) beq "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(0) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cbnez(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(encoding);
    SXLEN_t imm_value = imm;
    if (state->regs[rs1] != 0) {
        state->pc += imm_value;
    } else {
        state->pc += 2;
    }
}

template<typename XLEN_t>
inline void print_cbnez(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Sign, 12, 12, 6, 5, 2, 2, 11, 10, 4, 3, 1>(encoding);
    *out << "(C.BNEZ) bne "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(0) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_candi(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __int32_t imm = swizzle<__int32_t, ExtendBits::Sign, 12, 12, 6, 2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value_signed = imm;
    XLEN_t imm_value = *(XLEN_t*)&imm_value_signed;
    XLEN_t rd_value = rs1_value & imm_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_candi(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __int32_t imm = swizzle<__int32_t, ExtendBits::Sign, 12, 12, 6, 2>(encoding);
    *out << "(C.ANDI) andi "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cslli(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t imm = swizzle<__uint32_t, CI_SHAMT>(encoding);
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = rd;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value << imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cslli(__uint32_t encoding, std::ostream* out) {
    __uint32_t imm = swizzle<__uint32_t, CI_SHAMT>(encoding);
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = rd;
    *out << "(C.SLLI) slli "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cslli_128(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t imm = swizzle<__uint32_t, CI_SHAMT>(encoding);
    imm = imm == 0 ? 64 : imm;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = rd;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value << imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cslli_128(__uint32_t encoding, std::ostream* out) {
    __uint32_t imm = swizzle<__uint32_t, CI_SHAMT>(encoding);
    imm = imm == 0 ? 64 : imm;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = rd;
    *out << "(C.SLLI) slli"
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_clwsp(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = 2;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 3, 2, 12, 12, 6, 4, 2>(encoding);
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
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_clwsp(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __uint32_t rs1 = 2;
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 3, 2, 12, 12, 6, 4, 2>(encoding);
    *out << "(C.LWSP) lw "
         << RISCV::regName(rd) << ",("
         << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cswsp(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = 2;
    __uint32_t rs2 = swizzle<__uint32_t, CSS_RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 8, 7, 12, 9, 2>(encoding);
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
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cswsp(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = 2;
    __uint32_t rs2 = swizzle<__uint32_t, CSS_RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 8, 7, 12, 9, 2>(encoding);
    *out << "(C.SWSP) sw "
         << RISCV::regName(rs2) << ",("
         << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csdsp(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = 2;
    __uint32_t rs2 = swizzle<__uint32_t, CSS_RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 8, 7, 12, 9, 2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    SXLEN_t imm_value = imm;
    XLEN_t write_addr = rs1_value + imm_value;
    __uint64_t write_value = rs2_value;
    XLEN_t write_size = sizeof(write_value);
    Transaction<XLEN_t> transaction = mem->Write(write_addr, write_size, (char*)&write_value);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_addr);
        return;
    }
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csdsp(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = 2;
    __uint32_t rs2 = swizzle<__uint32_t, CSS_RS2>(encoding);
    __int32_t imm = swizzle<__uint32_t, ExtendBits::Zero, 8, 7, 12, 9, 2>(encoding);
    *out << "(C.SDSP) sw "
         << RISCV::regName(rs2) << ",("
         << imm << ")"
         << RISCV::regName(rs1) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cjalr(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __int32_t imm = 0;
    __uint32_t rd = 1;
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    imm_value &= ~(XLEN_t)1;
    state->regs[rd] = rd != 0 ? (state->pc + 2) : 0;
    state->pc = rs1_value + imm_value;
}

template<typename XLEN_t>
inline void print_cjalr(__uint32_t encoding, std::ostream* out) {
    __int32_t imm = 0;
    __uint32_t rd = 1;
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    *out << "(C.JALR) jalr "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cadd(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CR_RD_RS1>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, CR_RD_RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, CR_RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value + rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cadd(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CR_RD_RS1>(encoding);
    __uint32_t rs1 = swizzle<__uint32_t, CR_RD_RS1>(encoding);
    __uint32_t rs2 = swizzle<__uint32_t, CR_RS2>(encoding);
    *out << "(C.ADD) add "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cjr(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    typedef typename SignedXLEN<XLEN_t>::type SXLEN_t;
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __int32_t imm = 0;
    __uint32_t rd = 0;
    XLEN_t rs1_value = state->regs[rs1];
    SXLEN_t imm_value = imm;
    imm_value &= ~(XLEN_t)1;
    state->regs[rd] = rd != 0 ? (state->pc + 2) : 0;
    state->pc = rs1_value + imm_value;
}

template<typename XLEN_t>
inline void print_cjr(__uint32_t encoding, std::ostream* out) {
    __uint32_t rs1 = swizzle<__uint32_t, CI_RD_RS1>(encoding);
    __int32_t imm = 0;
    __uint32_t rd = 0;
    *out << "(C.JR) jalr "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cmv(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CR_RD_RS1>(encoding);
    __uint32_t rs1 = 0;
    __uint32_t rs2 = swizzle<__uint32_t, CR_RS2>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rs2_value = state->regs[rs2];
    XLEN_t rd_value = rs1_value + rs2_value;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_cmv(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CR_RD_RS1>(encoding);
    __uint32_t rs1 = 0;
    __uint32_t rs2 = swizzle<__uint32_t, CR_RS2>(encoding);
    *out << "(C.MV) add "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << RISCV::regName(rs2) << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_cebreak(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    state->RaiseException(RISCV::TrapCause::BREAKPOINT, encoding);
}

template<typename XLEN_t>
inline void print_cebreak(__uint32_t encoding, std::ostream* out) {
    *out << "(C.EBREAK) ebreak";
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrli(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value >> imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csrli(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    *out << "(C.SRLI) srli "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrli_128(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    imm = imm == 0 ? 64 : imm;
    XLEN_t rs1_value = state->regs[rs1];
    XLEN_t rd_value = rs1_value >> imm;
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csrli_128(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    imm = imm == 0 ? 64 : imm;
    *out << "(C.SRLI) srli "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrai(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    XLEN_t rs1_value = state->regs[rs1];
    __uint16_t imm_value = imm;
    XLEN_t rd_value = rs1_value >> imm_value;
    // Spec says: the original sign bit is copied into the vacated upper bits
    if (rs1_value & ((XLEN_t)1 << ((sizeof(XLEN_t)*8)-1)))
        rd_value |= (XLEN_t)((1 << imm_value)-1) << ((sizeof(XLEN_t)*8)-imm_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csrai(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    *out << "(C.SRAI) srai "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_csrai_128(__uint32_t encoding, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    imm = imm == 0 ? 64 : imm;
    XLEN_t rs1_value = state->regs[rs1];
    __uint16_t imm_value = imm;
    XLEN_t rd_value = rs1_value >> imm_value;
    // Spec says: the original sign bit is copied into the vacated upper bits
    if (rs1_value & ((XLEN_t)1 << ((sizeof(XLEN_t)*8)-1)))
        rd_value |= (XLEN_t)((1 << imm_value)-1) << ((sizeof(XLEN_t)*8)-imm_value);
    state->regs[rd] = rd != 0 ? rd_value : 0;
    state->pc += 2;
}

template<typename XLEN_t>
inline void print_csrai_128(__uint32_t encoding, std::ostream* out) {
    __uint32_t rd = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t rs1 = swizzle<__uint32_t, CB_RDX_RS1X>(encoding)+8;
    __uint32_t imm = swizzle<__uint32_t, CB_SHAMT>(encoding);
    imm = imm == 0 ? 64 : imm;
    *out << "(C.SRAI) srai "
         << RISCV::regName(rd) << ", "
         << RISCV::regName(rs1) << ", "
         << imm << std::endl;
}
