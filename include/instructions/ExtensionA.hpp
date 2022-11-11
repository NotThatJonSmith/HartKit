#pragma once

#include <RiscV.hpp>

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_lrw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // Does rd have to contain 0? If nonzero, is it an illegal state->instruction?
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
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
inline void ex_lrd(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: lrd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_scw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
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
inline void ex_scd(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: scd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoaddw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
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
inline void ex_amoaddd(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // TODO
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amoaddd"
             << std::endl;
        return;
    }
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoswapw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
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
inline void ex_amoswapd(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amoswapd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoxorw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
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
inline void ex_amoxord(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amoxord"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoorw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
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
inline void ex_amoord(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amoord"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amoandw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
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
inline void ex_amoandd(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amoandd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amominw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amominw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomind(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amomind"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amomaxw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxd(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amomaxd"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amominuw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amominuw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amominud(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amominud"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxuw(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amomaxuw"
             << std::endl;
        return;
    }
    // TODO
}

template<typename XLEN_t, std::ostream* out = nullptr>
inline void ex_amomaxud(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem) {
    // __uint32_t rd = swizzle<__uint32_t, RD>(state->inst);
    // __uint32_t rs1 = swizzle<__uint32_t, RS1>(state->inst);
    // __uint32_t rs2 = swizzle<__uint32_t, RS2>(state->inst);
    if constexpr (out != nullptr) {
        *out << "WARNING: state->instruction not implemented: amomaxud"
             << std::endl;
        return;
    }
    // TODO
}
