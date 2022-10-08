#pragma once

#include <cstdint>

#include <Transactor.hpp>
#include <Operands.hpp>

template<typename XLEN_t>
class HartState;

// A DecodedInstruction is the same object across all encodings that decode to
// the same instruction, including all those with different immediates. This is
// a necessary abstraction so we can build pre-decoded lookup tables.
// Another layer should maybe have the pre-baked operands for a completely
// resolved decode of a whole up-to-4-byte encoded instruction, which can then
// be cached. But such a structure would have to be in addition to this one if
// we want to keep the possibility of a precomputed decoder.
template<typename XLEN_t>
struct DecodedInstruction {
    Operands (*getOperands)(__uint32_t instruction);
    void (*execute)(Operands operands, HartState<XLEN_t> *state, Transactor<XLEN_t> *transactor);
    unsigned int width;
};
