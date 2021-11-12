#pragma once

#include <IOTarget.hpp> // AccessType
#include <RiscV.hpp>

template<typename XLEN_t>
struct Translation {
    XLEN_t untranslated;
    XLEN_t translated;
    XLEN_t validThrough;
    RISCV::TrapCause generatedTrap;
};

template<typename XLEN_t, CASK::AccessType accessType>
inline Translation<XLEN_t> PageFault(XLEN_t virt_addr) {
    if constexpr (accessType == CASK::AccessType::R) {
        return { virt_addr, 0, 0, RISCV::TrapCause::LOAD_PAGE_FAULT };
    } else if constexpr (accessType == CASK::AccessType::W) {
        return { virt_addr, 0, 0, RISCV::TrapCause::STORE_AMO_PAGE_FAULT };
    } else {
        return { virt_addr, 0, 0, RISCV::TrapCause::INSTRUCTION_PAGE_FAULT };
    }
}