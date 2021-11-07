#include <RegisterLikeObject.hpp>

__uint32_t RegisterLikeObject::Read32() {
    __uint32_t value = Get32();
    CommitReadSideEffects();
    return value;
}

__uint64_t RegisterLikeObject::Read64 () {
    __uint64_t value = Get64();
    CommitReadSideEffects();
    return value;
}

__uint128_t RegisterLikeObject::Read128 () {
    __uint128_t value = Get128();
    CommitReadSideEffects();
    return value;
}

void RegisterLikeObject::Write32(__uint32_t value) {
    Set32(value);
    CommitWriteSideEffects();
}

void RegisterLikeObject::Write64(__uint64_t value) {
    Set64(value);
    CommitWriteSideEffects();
}

void RegisterLikeObject::Write128(__uint128_t value) {
    Set128(value);
    CommitWriteSideEffects();
}

void RegisterLikeObject::CommitWriteSideEffects() {

}

void RegisterLikeObject::CommitReadSideEffects() {

}
