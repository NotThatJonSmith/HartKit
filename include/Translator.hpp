#pragma once

#include <IOVerb.hpp>
#include <Translation.hpp>

template<typename XLEN_t>
class Translator {
public:
    virtual inline Translation<XLEN_t> TranslateRead(XLEN_t address) = 0;
    virtual inline Translation<XLEN_t> TranslateWrite(XLEN_t address) = 0;
    virtual inline Translation<XLEN_t> TranslateFetch(XLEN_t address) = 0;
    template<IOVerb verb>
    inline Translation<XLEN_t> Translate(XLEN_t address) {
        if constexpr (verb == IOVerb::Read) {
            return TranslateRead(address);
        } else if constexpr (verb == IOVerb::Write) {
            return TranslateWrite(address);
        } else {
            return TranslateFetch(address);
        }
    }
};
