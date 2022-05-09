#pragma once

#include <cstdint>

#include <HartState.hpp>

struct CodePoint {
    Operands (*getOperands)(__uint32_t instruction);
    void (*execute32)(Operands operands, HartState<__uint32_t> *state, Transactor<__uint32_t> *memory);
    void (*execute64)(Operands operands, HartState<__uint64_t> *state, Transactor<__uint64_t> *memory);
    void (*execute128)(Operands operands, HartState<__uint128_t> *state, Transactor<__uint128_t> *memory);
    void (*disassemble)(Operands operands, std::ostream *out);
    unsigned int width;
};
