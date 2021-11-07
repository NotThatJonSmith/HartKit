#include <HartSpec.hpp>

void HartSpec::SetPrivWidthSupport(RISCV::PrivilegeMode priv, RISCV::XlenMode xlen, bool enabled) {
    supportWidthPrivilege[xlen][priv] = enabled;
}

void HartSpec::SetWidthSupport(RISCV::XlenMode xlen, bool enabled) {
    supportWidthPrivilege[xlen][RISCV::PrivilegeMode::Machine] = enabled;
    supportWidthPrivilege[xlen][RISCV::PrivilegeMode::Supervisor] = enabled;
    supportWidthPrivilege[xlen][RISCV::PrivilegeMode::User] = enabled;
}

void HartSpec::SetExtensionSupport(const char* extensionString) {
    maximalExtensions = RISCV::stringToExtensions(extensionString);
}

RISCV::XlenMode HartSpec::WidestXlen(RISCV::PrivilegeMode privilege, RISCV::XlenMode targetXlen) {

    if (targetXlen == RISCV::XlenMode::XL128 && supportWidthPrivilege[RISCV::XlenMode::XL128][privilege]) {
        return RISCV::XlenMode::XL128;
    }

    if (targetXlen != RISCV::XlenMode::XL32 && supportWidthPrivilege[RISCV::XlenMode::XL64][privilege]) {
        return RISCV::XlenMode::XL64;
    }

    return RISCV::XlenMode::XL32;
}