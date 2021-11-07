#pragma once

#include <cstdint>
#include <iostream>

#include <Operands.hpp>

class HartState;

struct Instruction {
    Operands (*getOperands)(__uint32_t instruction);
    void (*execute)(Operands operands, HartState *state);
    void (*disassemble)(Operands operands, std::ostream *out);
    unsigned int width;
};
