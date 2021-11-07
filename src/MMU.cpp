#include <MMU.hpp>

MMU::MMU(CASK::IOTarget* targetBus, Translator* vmTranslator)
    : bus(targetBus), translator(vmTranslator) {
}

TransactionResult<__uint32_t> MMU::Read32(__uint32_t startAddress, __uint32_t size, char* dst) {
    return TransactInternal<__uint32_t, CASK::AccessType::R>(startAddress, size, dst);
}

TransactionResult<__uint64_t> MMU::Read64(__uint64_t startAddress, __uint64_t size, char* dst) {
    return TransactInternal<__uint64_t, CASK::AccessType::R>(startAddress, size, dst);
}

TransactionResult<__uint128_t> MMU::Read128(__uint128_t startAddress, __uint128_t size, char* dst) {
    return TransactInternal<__uint128_t, CASK::AccessType::R>(startAddress, size, dst);
}

TransactionResult<__uint32_t> MMU::Write32(__uint32_t startAddress, __uint32_t size, char* src) {
    return TransactInternal<__uint32_t, CASK::AccessType::W>(startAddress, size, src);
}

TransactionResult<__uint64_t> MMU::Write64(__uint64_t startAddress, __uint64_t size, char* src) {
    return TransactInternal<__uint64_t, CASK::AccessType::W>(startAddress, size, src);
}

TransactionResult<__uint128_t> MMU::Write128(__uint128_t startAddress, __uint128_t size, char* src) {
    return TransactInternal<__uint128_t, CASK::AccessType::W>(startAddress, size, src);
}

TransactionResult<__uint32_t> MMU::Fetch32(__uint32_t startAddress, __uint32_t size, char* dst) {
    return TransactInternal<__uint32_t, CASK::AccessType::X>(startAddress, size, dst);
}

TransactionResult<__uint64_t> MMU::Fetch64(__uint64_t startAddress, __uint64_t size, char* dst) {
    return TransactInternal<__uint64_t, CASK::AccessType::X>(startAddress, size, dst);
}

TransactionResult<__uint128_t> MMU::Fetch128(__uint128_t startAddress, __uint128_t size, char* dst) {
    return TransactInternal<__uint128_t, CASK::AccessType::X>(startAddress, size, dst);
}
