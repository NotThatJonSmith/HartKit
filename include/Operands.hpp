#pragma once

#include <cstdint>

union Operands {
    struct {
        __uint8_t rd, rs1, rs2;
    } R;
    struct {
        __uint8_t rd, rs1;
        union {__int16_t s; __uint16_t u; } imm;
    } I;
    struct {
        __uint8_t rs1, rs2;
        union {__int16_t s; __uint16_t u; } imm;
    } S;
    struct {
        __uint8_t rd;
        union {__int32_t s; __uint32_t u; } imm;
    } U;
    struct {
        __uint32_t word;
    } status;
};
