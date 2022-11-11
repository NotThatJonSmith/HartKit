#pragma once

#include <cstdint>

#include <Transactor.hpp>

template<typename XLEN_t>
class HartState;

template<typename XLEN_t>
using DecodedInstruction = void (*)(HartState<XLEN_t> *state, Transactor<XLEN_t> *mem);
