#include <Register.hpp>

Register::~Register() {
}

__uint32_t Register::Get32() {
    if (zero) {
        return 0;
    }
    return bits.w;
}

__uint64_t Register::Get64() {
    if (zero) {
        return 0;
    }
    return bits.dw;
}

__uint128_t Register::Get128() {
    if (zero) {
        return 0;
    }
    return bits.qw;
}

void Register::Set32(__uint32_t value) {
    bits.w = value;
}

void Register::Set64(__uint64_t value) {
    bits.dw = value;
}

void Register::Set128(__uint128_t value) {
    bits.qw = value;
}
