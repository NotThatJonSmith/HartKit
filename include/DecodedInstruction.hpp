#pragma once

#include <cstdint>

#include <Transactor.hpp>

template<typename XLEN_t>
class HartState;

template<typename XLEN_t>
using DecodedInstruction = void (*)(__uint32_t inst, HartState<XLEN_t> *state, Transactor<XLEN_t> *mem);
