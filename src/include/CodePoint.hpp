#pragma once

#include <Operands.hpp>

class HartState;

struct CodePoint {
    Operands (*getOperands)(__uint32_t instruction);
    void (*execute32)(Operands operands, HartState *state);
    void (*execute64)(Operands operands, HartState *state);
    void (*execute128)(Operands operands, HartState *state);
    void (*disassemble)(Operands operands, std::ostream *out);
    unsigned int width;
};
