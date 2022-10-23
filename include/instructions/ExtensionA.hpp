#pragma once

#include <RiscV.hpp>

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lrw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // Does rd have to contain 0? If nonzero, is it an illegal instruction?
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "lrw "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    __uint32_t tmp = 0;
    XLEN_t read_address = state->regs[rs1];
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&tmp);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? tmp : 0; // NOTE sign extend for RV64
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lrd(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: lrd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_scw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "scw "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t tmp = state->regs[rs2];
    XLEN_t write_address = state->regs[rs1];
    XLEN_t write_size = 4;
    Transaction<XLEN_t> transaction = mem->Write(write_address, write_size, (char*)&tmp);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != write_size) {
        state->RaiseException(transaction.trapCause, write_address);
        return;
    }
    state->regs[rd] = rd != 0 ? 0 : 0; // NOTE, zero means sc succeeded - for now it always does!
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_scd(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: scd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoaddw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "amoadd.w "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t tmp = 0;
    XLEN_t read_address = state->regs[rs1];
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&tmp);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? tmp : 0;
    tmp += state->regs[rs2];
    mem->Write(state->regs[rs1], 4, (char*)&tmp);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoaddd(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // TODO
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amoaddd"
             << std::endl;
        return;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoswapw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "amoswap.w "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    XLEN_t tmp = 0;
    XLEN_t read_address = state->regs[rs1];
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&tmp);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? tmp : 0;
    tmp = state->regs[rs2];
    mem->Write(state->regs[rs1], 4, (char*)&tmp);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoswapd(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amoswapd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoxorw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "amoxor.w "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    __uint32_t tmp = 0;
    XLEN_t read_address = state->regs[rs1];
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&tmp);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? tmp : 0;
    tmp ^= state->regs[rs2];
    mem->Write(state->regs[rs1], 4, (char*)&tmp);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoxord(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amoxord"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoorw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "amoor.w "
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    __uint32_t tmp = 0;
    XLEN_t read_address = state->regs[rs1];
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&tmp);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? tmp : 0;
    tmp |= state->regs[rs2];
    mem->Write(state->regs[rs1], 4, (char*)&tmp);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoord(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amoord"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoandw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "amoand.w x"
             << RISCV::regName(rd) << ", "
             << RISCV::regName(rs1) << ", "
             << RISCV::regName(rs2) << std::endl;
        return;
    }
    __uint32_t tmp = 0;
    XLEN_t read_address = state->regs[rs1];
    XLEN_t read_size = 4;
    Transaction<XLEN_t> transaction = mem->Read(read_address, read_size, (char*)&tmp);
    if (transaction.trapCause != RISCV::TrapCause::NONE || transaction.transferredSize != read_size) {
        state->RaiseException(transaction.trapCause, read_address);
        return;
    }
    state->regs[rd] = rd != 0 ? tmp : 0;
    tmp &= state->regs[rs2];
    mem->Write(state->regs[rs1], 4, (char*)&tmp);
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoandd(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amoandd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amominw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amominw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomind(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amomind"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amomaxw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxd(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amomaxd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amominuw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amominuw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amominud(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amominud"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxuw(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amomaxuw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxud(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: instruction not implemented: amomaxud"
             << std::endl;
        return;
    }
    // TODO
}
