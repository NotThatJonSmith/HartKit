#pragma once

#include <cstdint>

#include <Transactor.hpp>
#include <Operands.hpp>

template<typename XLEN_t>
class HartState;

template<typename XLEN_t>
struct DecodedInstruction {
    Operands (*getOperands)(__uint32_t instruction);
    void (*execute)(Operands operands, HartState<XLEN_t> *state, Transactor<XLEN_t> *transactor);
    void (*disassemble)(Operands operands, std::ostream *out);
    unsigned int width;
};
