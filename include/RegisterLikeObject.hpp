#pragma once

#include <cstdint>
#include <type_traits>

class RegisterLikeObject {

public:

    // Read the raw value without triggering side-effects
    virtual __uint32_t Get32() = 0;
    virtual __uint64_t Get64() = 0;
    virtual __uint128_t Get128() = 0;
    template <typename XLEN_t>
    XLEN_t Get() {
        if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
            return Get32();
        } else if constexpr (std::is_same<XLEN_t, __uint64_t>()) {
            return Get64();
        } else if constexpr (std::is_same<XLEN_t, __uint128_t>()) {
            return Get128();
        } else throw 0;
    }

    // Write the raw value without triggering side-effects
    virtual void Set32(__uint32_t value) = 0;
    virtual void Set64(__uint64_t value) = 0;
    virtual void Set128(__uint128_t value) = 0;
    template <typename XLEN_t>
    void Set(XLEN_t value) {
        if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
            Set32(value);
        } else if constexpr (std::is_same<XLEN_t, __uint64_t>()) {
            Set64(value);
        } else if constexpr (std::is_same<XLEN_t, __uint128_t>()) {
            Set128(value);
        } else throw 0;
    }

    // Read the raw value, and trigger any read side-effects
    virtual __uint32_t Read32();
    virtual __uint64_t Read64();
    virtual __uint128_t Read128();
    template <typename XLEN_t>
    XLEN_t Read() {
        if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
            return Read32();
        } else if constexpr (std::is_same<XLEN_t, __uint64_t>()) {
            return Read64();
        } else if constexpr (std::is_same<XLEN_t, __uint128_t>()) {
            return Read128();
        } else throw 0;
    }

    // Write the raw value, and trigger any write side-effects
    virtual void Write32(__uint32_t value);
    virtual void Write64(__uint64_t value);
    virtual void Write128(__uint128_t value);
    template <typename XLEN_t>
    void Write(XLEN_t value) {
        if constexpr (std::is_same<XLEN_t, __uint32_t>()) {
            Write32(value);
        } else if constexpr (std::is_same<XLEN_t, __uint64_t>()) {
            Write64(value);
        } else if constexpr (std::is_same<XLEN_t, __uint128_t>()) {
            Write128(value);
        } else throw 0;
    }

    virtual void CommitWriteSideEffects();
    virtual void CommitReadSideEffects();

};
