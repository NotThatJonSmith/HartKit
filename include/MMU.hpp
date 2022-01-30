#pragma once

#include <RiscV.hpp>
#include <IOTarget.hpp>
#include <TransactionBuffer.hpp>
#include <Translator.hpp>

class HartState;

template<typename XLEN_t>
struct TransactionResult {
    RISCV::TrapCause trapCause;
    XLEN_t transferredSize;
};

class MMU {

private:

    CASK::IOTarget* bus;
    Translator* translator;

public:

    MMU(CASK::IOTarget* targetBus, Translator* vmTranslator);

    template<typename T, CASK::AccessType accessType>
    inline TransactionResult<T> Transact(T startAddress, T size, char* buf) {
        if constexpr (accessType == CASK::AccessType::R) {
            return Read<T>(startAddress, size, buf);
        } else if constexpr (accessType == CASK::AccessType::W) {
            return Write<T>(startAddress, size, buf);
        } else {
            return Fetch<T>(startAddress, size, buf);
        }
    }

    virtual TransactionResult<__uint32_t> Read32(__uint32_t startAddress, __uint32_t size, char* dst);
    virtual TransactionResult<__uint64_t> Read64(__uint64_t startAddress, __uint64_t size, char* dst);
    virtual TransactionResult<__uint128_t> Read128(__uint128_t startAddress, __uint128_t size, char* dst);
    template<typename T>
    inline TransactionResult<T> Read(T startAddress, T size, char* dst) {
        if constexpr (std::is_same<T, __uint32_t>()) {
            return Read32(startAddress, size, dst);
        } else if constexpr (std::is_same<T, __uint64_t>()) {
            return Read64(startAddress, size, dst);
        } else if constexpr (std::is_same<T, __uint128_t>()) {
            return Read128(startAddress, size, dst);
        } else {
            // TODO fatal, bail out
        }
    }

    virtual TransactionResult<__uint32_t> Write32(__uint32_t startAddress, __uint32_t size, char* src);
    virtual TransactionResult<__uint64_t> Write64(__uint64_t startAddress, __uint64_t size, char* src);
    virtual TransactionResult<__uint128_t> Write128(__uint128_t startAddress, __uint128_t size, char* src);
    template<typename T>
    inline TransactionResult<T> Write(T startAddress, T size, char* src) {
        if constexpr (std::is_same<T, __uint32_t>()) {
            return Write32(startAddress, size, src);
        } else if constexpr (std::is_same<T, __uint64_t>()) {
            return Write64(startAddress, size, src);
        } else if constexpr (std::is_same<T, __uint128_t>()) {
           return  Write128(startAddress, size, src);
        } else {
            // TODO fatal, bail out
        }
    }

    virtual TransactionResult<__uint32_t> Fetch32(__uint32_t startAddress, __uint32_t size, char* dst);
    virtual TransactionResult<__uint64_t> Fetch64(__uint64_t startAddress, __uint64_t size, char* dst);
    virtual TransactionResult<__uint128_t> Fetch128(__uint128_t startAddress, __uint128_t size, char* dst);
    template<typename T>
    inline TransactionResult<T> Fetch(T startAddress, T size, char* dst) {
        if constexpr (std::is_same<T, __uint32_t>()) {
            return Fetch32(startAddress, size, dst);
        } else if constexpr (std::is_same<T, __uint64_t>()) {
            return Fetch64(startAddress, size, dst);
        } else if constexpr (std::is_same<T, __uint128_t>()) {
            return Fetch128(startAddress, size, dst);
        } else {
            // TODO fatal, bail out
        }
    }

private:

    template <typename XLEN_t, CASK::AccessType accessType>
    TransactionResult<XLEN_t> TransactInternal(XLEN_t startAddress, XLEN_t size, char* buf) {

        TransactionResult<XLEN_t> result;
        result.trapCause = RISCV::TrapCause::NONE;
        result.transferredSize = 0;

        XLEN_t endAddress = startAddress + size - 1;

        if (endAddress < startAddress) {
            return result;
        }

        static TransactionBuffer<XLEN_t, accessType> transaction(bus);

        XLEN_t chunkStartAddress = startAddress;
        while (chunkStartAddress <= endAddress) {
            
            Translation<XLEN_t> translation = translator->Translate<XLEN_t, accessType>(chunkStartAddress);
            
            XLEN_t chunkEndAddress = translation.validThrough;
            if (chunkEndAddress > endAddress) {
                chunkEndAddress = endAddress;
            }
            XLEN_t chunkSize = chunkEndAddress - chunkStartAddress + 1;

            result.trapCause = translation.generatedTrap;
            if (result.trapCause != RISCV::TrapCause::NONE) {
                transaction.Clear(); // TODO is clear even needed?
                return result;
            }

            char* chunkBuf = buf + (chunkStartAddress - startAddress);
            XLEN_t translatedChunkStart = translation.translated + chunkStartAddress - translation.untranslated;
            XLEN_t chunkSuccessSize = transaction.BufferedIO(translatedChunkStart, chunkSize, chunkBuf);
            chunkStartAddress += chunkSize;
            
            if (chunkSuccessSize != chunkSize) {
                transaction.Clear();
                return result;
            }

        }

        result.transferredSize = transaction.Commit();
        return result;
    }
};
