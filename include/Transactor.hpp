#pragma once

#include <IOVerb.hpp>
#include <Transaction.hpp>

template<typename XLEN_t>
class Transactor {
public:
    virtual inline Transaction<XLEN_t> Read(XLEN_t startAddress, XLEN_t size, char* buf) = 0;
    virtual inline Transaction<XLEN_t> Write(XLEN_t startAddress, XLEN_t size, char* buf) = 0;
    virtual inline Transaction<XLEN_t> Fetch(XLEN_t startAddress, XLEN_t size, char* buf) = 0;
    template<IOVerb verb>
    inline Transaction<XLEN_t> Transact(XLEN_t startAddress, XLEN_t size, char* buf) {
        if constexpr (verb == IOVerb::Read) {
            return Read(startAddress, size, buf);
        } else if constexpr (verb == IOVerb::Write) {
            return Write(startAddress, size, buf);
        } else {
            return Fetch(startAddress, size, buf);
        }
    }
};
