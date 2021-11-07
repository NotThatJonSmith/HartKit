#pragma once

#include <RiscV.hpp>

#include <CodePoint.hpp>
#include <Operands.hpp>

#pragma mark -- lrw --

template<typename XLEN_t>
inline void ex_lrw(Operands operands, HartState *state) {
    // Does rd have to contain 0? If nonzero, is it an illegal instruction?
    __uint32_t tmp = 0;
    XLEN_t read_address = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t read_size = 4;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(read_address, read_size, (char*)&tmp);
    if (!success) {
        return;
    }
    state->regs[operands.R.rd].Write<XLEN_t>(tmp); // NOTE sign extend for RV64
}

inline void print_lrw(Operands operands, std::ostream *out) {
    *out << "lrw "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_lrw = {
    r_operands_from,
    ex_lrw<__uint32_t>,
    ex_lrw<__uint64_t>,
    ex_lrw<__uint128_t>,
    print_lrw,
    4
};

#pragma mark -- lrd --

template<typename XLEN_t>
inline void ex_lrd(Operands operands, HartState *state) {
    // TODO
}

inline void print_lrd(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: lrd"
         << std::endl;
}

constexpr CodePoint inst_lrd = {
    r_operands_from,
    ex_lrd<__uint32_t>,
    ex_lrd<__uint64_t>,
    ex_lrd<__uint128_t>,
    print_lrd,
    4
};

#pragma mark -- scw --

template<typename XLEN_t>
inline void ex_scw(Operands operands, HartState *state) {
    XLEN_t tmp = state->regs[operands.R.rs2].Read<XLEN_t>();
    XLEN_t write_address = state->regs[operands.R.rs1].Read<XLEN_t>();
    XLEN_t write_size = 4;
    bool success = state->Transact<XLEN_t, CASK::AccessType::W>(write_address, write_size, (char*)&tmp);
    if (!success) {
        return; // TODO or else what??
    }
    state->regs[operands.R.rd].Write<XLEN_t>(0); // NOTE, zero means sc succeeded - for now it always does!
}

inline void print_scw(Operands operands, std::ostream *out) {
    *out << "scw "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_scw = {
    r_operands_from,
    ex_scw<__uint32_t>,
    ex_scw<__uint64_t>,
    ex_scw<__uint128_t>,
    print_scw,
    4
};

#pragma mark -- scd --

template<typename XLEN_t>
inline void ex_scd(Operands operands, HartState *state) {
    // TODO
}

inline void print_scd(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: scd"
         << std::endl;
}

constexpr CodePoint inst_scd = {
    r_operands_from,
    ex_scd<__uint32_t>,
    ex_scd<__uint64_t>,
    ex_scd<__uint128_t>,
    print_scd,
    4
};

#pragma mark -- amoaddw --

template<typename XLEN_t>
inline void ex_amoaddw(Operands operands, HartState *state) {
    XLEN_t tmp = 0;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
    if (!success) {
        return;
    }
    state->regs[operands.R.rd].Write<XLEN_t>(tmp);
    tmp += state->regs[operands.R.rs2].Read<XLEN_t>();
    state->Transact<XLEN_t, CASK::AccessType::W>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
}

inline void print_amoaddw(Operands operands, std::ostream *out) {
    *out << "amoadd.w "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_amoaddw = {
    r_operands_from,
    ex_amoaddw<__uint32_t>,
    ex_amoaddw<__uint64_t>,
    ex_amoaddw<__uint128_t>,
    print_amoaddw,
    4
};

#pragma mark -- amoaddd --

template<typename XLEN_t>
inline void ex_amoaddd(Operands operands, HartState *state) {
    // TODO
}

inline void print_amoaddd(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amoaddd"
         << std::endl;
}

constexpr CodePoint inst_amoaddd = {
    r_operands_from,
    ex_amoaddd<__uint32_t>,
    ex_amoaddd<__uint64_t>,
    ex_amoaddd<__uint128_t>,
    print_amoaddd,
    4
};

#pragma mark -- amoswapw --

template<typename XLEN_t>
inline void ex_amoswapw(Operands operands, HartState *state) {
    XLEN_t tmp = 0;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
    if (!success) {
        return;
    }
    state->regs[operands.R.rd].Write<XLEN_t>(tmp);
    tmp = state->regs[operands.R.rs2].Read<XLEN_t>();
    state->Transact<XLEN_t, CASK::AccessType::W>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
}

inline void print_amoswapw(Operands operands, std::ostream *out) {
    *out << "amoswap.w "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_amoswapw = {
    r_operands_from,
    ex_amoswapw<__uint32_t>,
    ex_amoswapw<__uint64_t>,
    ex_amoswapw<__uint128_t>,
    print_amoswapw,
    4
};

#pragma mark -- amoswapd --

template<typename XLEN_t>
inline void ex_amoswapd(Operands operands, HartState *state) {
    // TODO
}

inline void print_amoswapd(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amoswapd"
         << std::endl;
}

constexpr CodePoint inst_amoswapd = {
    r_operands_from,
    ex_amoswapd<__uint32_t>,
    ex_amoswapd<__uint64_t>,
    ex_amoswapd<__uint128_t>,
    print_amoswapd,
    4
};

#pragma mark -- amoxorw --

template<typename XLEN_t>
inline void ex_amoxorw(Operands operands, HartState *state) {
    __uint32_t tmp = 0;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
    if (!success) {
        return;
    }
    state->regs[operands.R.rd].Write<XLEN_t>(tmp);
    tmp ^= state->regs[operands.R.rs2].Read<XLEN_t>();
    state->Transact<XLEN_t, CASK::AccessType::W>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
}

inline void print_amoxorw(Operands operands, std::ostream *out) {
    *out << "amoxor.w "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_amoxorw = {
    r_operands_from,
    ex_amoxorw<__uint32_t>,
    ex_amoxorw<__uint64_t>,
    ex_amoxorw<__uint128_t>,
    print_amoxorw,
    4
};

#pragma mark -- amoxord --

template<typename XLEN_t>
inline void ex_amoxord(Operands operands, HartState *state) {
    // TODO
}

inline void print_amoxord(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amoxord"
         << std::endl;
}

constexpr CodePoint inst_amoxord = {
    r_operands_from,
    ex_amoxord<__uint32_t>,
    ex_amoxord<__uint64_t>,
    ex_amoxord<__uint128_t>,
    print_amoxord,
    4
};

#pragma mark -- amoorw --

template<typename XLEN_t>
inline void ex_amoorw(Operands operands, HartState *state) {
    __uint32_t tmp = 0;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
    if (!success) {
        return;
    }
    state->regs[operands.R.rd].Write<XLEN_t>(tmp);
    tmp |= state->regs[operands.R.rs2].Read<XLEN_t>();
    state->Transact<XLEN_t, CASK::AccessType::W>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
}

inline void print_amoorw(Operands operands, std::ostream *out) {
    *out << "amoor.w "
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_amoorw = {
    r_operands_from,
    ex_amoorw<__uint32_t>,
    ex_amoorw<__uint64_t>,
    ex_amoorw<__uint128_t>,
    print_amoorw,
    4
};

#pragma mark -- amoord --

template<typename XLEN_t>
inline void ex_amoord(Operands operands, HartState *state) {
    // TODO
}

inline void print_amoord(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amoord"
         << std::endl;
}

constexpr CodePoint inst_amoord = {
    r_operands_from,
    ex_amoord<__uint32_t>,
    ex_amoord<__uint64_t>,
    ex_amoord<__uint128_t>,
    print_amoord,
    4
};

#pragma mark -- amoandw --

template<typename XLEN_t>
inline void ex_amoandw(Operands operands, HartState *state) {
    __uint32_t tmp = 0;
    bool success = state->Transact<XLEN_t, CASK::AccessType::R>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
    if (!success) {
        return;
    }
    state->regs[operands.R.rd].Write<XLEN_t>(tmp);
    tmp &= state->regs[operands.R.rs2].Read<XLEN_t>();
    state->Transact<XLEN_t, CASK::AccessType::W>(state->regs[operands.R.rs1].Read<XLEN_t>(), 4, (char*)&tmp);
}

inline void print_amoandw(Operands operands, std::ostream *out) {
    *out << "amoand.w x"
         << RISCV::regName(operands.R.rd) << ", "
         << RISCV::regName(operands.R.rs1) << ", "
         << RISCV::regName(operands.R.rs2) << std::endl;
}

constexpr CodePoint inst_amoandw = {
    r_operands_from,
    ex_amoandw<__uint32_t>,
    ex_amoandw<__uint64_t>,
    ex_amoandw<__uint128_t>,
    print_amoandw,
    4
};

#pragma mark -- amoandd --

template<typename XLEN_t>
inline void ex_amoandd(Operands operands, HartState *state) {
    // TODO
}

inline void print_amoandd(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amoandd"
         << std::endl;
}

constexpr CodePoint inst_amoandd = {
    r_operands_from,
    ex_amoandd<__uint32_t>,
    ex_amoandd<__uint64_t>,
    ex_amoandd<__uint128_t>,
    print_amoandd,
    4
};

#pragma mark -- amominw --

template<typename XLEN_t>
inline void ex_amominw(Operands operands, HartState *state) {
    // TODO
}

inline void print_amominw(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amominw"
         << std::endl;
}

constexpr CodePoint inst_amominw = {
    r_operands_from,
    ex_amominw<__uint32_t>,
    ex_amominw<__uint64_t>,
    ex_amominw<__uint128_t>,
    print_amominw,
    4
};

#pragma mark -- amomind --

template<typename XLEN_t>
inline void ex_amomind(Operands operands, HartState *state) {
    // TODO
}

inline void print_amomind(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amomind"
         << std::endl;
}

constexpr CodePoint inst_amomind = {
    r_operands_from,
    ex_amomind<__uint32_t>,
    ex_amomind<__uint64_t>,
    ex_amomind<__uint128_t>,
    print_amomind,
    4
};

#pragma mark -- amomaxw --

template<typename XLEN_t>
inline void ex_amomaxw(Operands operands, HartState *state) {
    // TODO
}

inline void print_amomaxw(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amomaxw"
         << std::endl;
}

constexpr CodePoint inst_amomaxw = {
    r_operands_from,
    ex_amomaxw<__uint32_t>,
    ex_amomaxw<__uint64_t>,
    ex_amomaxw<__uint128_t>,
    print_amomaxw,
    4
};

#pragma mark -- amomaxd --

template<typename XLEN_t>
inline void ex_amomaxd(Operands operands, HartState *state) {
    // TODO
}

inline void print_amomaxd(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amomaxd"
         << std::endl;
}

constexpr CodePoint inst_amomaxd = {
    r_operands_from,
    ex_amomaxd<__uint32_t>,
    ex_amomaxd<__uint64_t>,
    ex_amomaxd<__uint128_t>,
    print_amomaxd,
    4
};

#pragma mark -- amominuw --

template<typename XLEN_t>
inline void ex_amominuw(Operands operands, HartState *state) {
    // TODO
}

inline void print_amominuw(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amominuw"
         << std::endl;
}

constexpr CodePoint inst_amominuw = {
    r_operands_from,
    ex_amominuw<__uint32_t>,
    ex_amominuw<__uint64_t>,
    ex_amominuw<__uint128_t>,
    print_amominuw,
    4
};

#pragma mark -- amominud --

template<typename XLEN_t>
inline void ex_amominud(Operands operands, HartState *state) {
    // TODO
}

inline void print_amominud(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amominud"
         << std::endl;
}

constexpr CodePoint inst_amominud = {
    r_operands_from,
    ex_amominud<__uint32_t>,
    ex_amominud<__uint64_t>,
    ex_amominud<__uint128_t>,
    print_amominud,
    4
};

#pragma mark -- amomaxuw --

template<typename XLEN_t>
inline void ex_amomaxuw(Operands operands, HartState *state) {
    // TODO
}

inline void print_amomaxuw(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amomaxuw"
         << std::endl;
}

constexpr CodePoint inst_amomaxuw = {
    r_operands_from,
    ex_amomaxuw<__uint32_t>,
    ex_amomaxuw<__uint64_t>,
    ex_amomaxuw<__uint128_t>,
    print_amomaxuw,
    4
};

#pragma mark -- amomaxud --

template<typename XLEN_t>
inline void ex_amomaxud(Operands operands, HartState *state) {
    // TODO
}

inline void print_amomaxud(Operands operands, std::ostream *out) {
    *out << "WARNING: instruction not implemented: amomaxud"
         << std::endl;
}

constexpr CodePoint inst_amomaxud = {
    r_operands_from,
    ex_amomaxud<__uint32_t>,
    ex_amomaxud<__uint64_t>,
    ex_amomaxud<__uint128_t>,
    print_amomaxud,
    4
};
