#pragma once
#include <Translation.hpp>

class HartState;

class Translator {

public:

    virtual void Configure(HartState* hartState) = 0;

    template<typename T, CASK::AccessType accessType>
    inline Translation<T> Translate(T address) {
        if constexpr (accessType == CASK::AccessType::R) {
            return TranslateRead<T>(address);
        } else if constexpr (accessType == CASK::AccessType::W) {
            return TranslateWrite<T>(address);
        } else {
            return TranslateFetch<T>(address);
        }
    }

    virtual Translation<__uint32_t> TranslateRead32(__uint32_t address) = 0;
    virtual Translation<__uint64_t> TranslateRead64(__uint64_t address) = 0;
    virtual Translation<__uint128_t> TranslateRead128(__uint128_t address) = 0;
    template<typename T>
    inline Translation<T> TranslateRead(T address) {
        if constexpr (std::is_same<T, __uint32_t>()) {
            return TranslateRead32(address);
        } else if constexpr (std::is_same<T, __uint64_t>()) {
            return TranslateRead64(address);
        } else if constexpr (std::is_same<T, __uint128_t>()) {
            return TranslateRead128(address);
        } else {
            // TODO fatal, bail out
        }
    }

    virtual Translation<__uint32_t> TranslateWrite32(__uint32_t address) = 0;
    virtual Translation<__uint64_t> TranslateWrite64(__uint64_t address) = 0;
    virtual Translation<__uint128_t> TranslateWrite128(__uint128_t address) = 0;
    template<typename T>
    inline Translation<T> TranslateWrite(T address) {
        if constexpr (std::is_same<T, __uint32_t>()) {
            return TranslateWrite32(address);
        } else if constexpr (std::is_same<T, __uint64_t>()) {
            return TranslateWrite64(address);
        } else if constexpr (std::is_same<T, __uint128_t>()) {
           return  TranslateWrite128(address);
        } else {
            // TODO fatal, bail out
        }
    }

    virtual Translation<__uint32_t> TranslateFetch32(__uint32_t address) = 0;
    virtual Translation<__uint64_t> TranslateFetch64(__uint64_t address) = 0;
    virtual Translation<__uint128_t> TranslateFetch128(__uint128_t address) = 0;
    template<typename T>
    inline Translation<T> TranslateFetch(T address) {
        if constexpr (std::is_same<T, __uint32_t>()) {
            return TranslateFetch32(address);
        } else if constexpr (std::is_same<T, __uint64_t>()) {
            return TranslateFetch64(address);
        } else if constexpr (std::is_same<T, __uint128_t>()) {
            return TranslateFetch128(address);
        } else {
            // TODO fatal, bail out
        }
    }

};