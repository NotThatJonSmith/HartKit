#pragma once

#include <cstdint>

#include <RiscV.hpp>

#include <Register.hpp>

class HartSpec {

public:

    bool supportWidthPrivilege[4][4] = {
        /*U*/  /*S*/  /*H*/  /*M*/ // TODO H isn't real
        { false, false, false, false }, /*None*/
        { true,  true,  false, true  }, /*32*/
        { true,  true,  false, true  }, /*64*/
        { true,  true,  false, true  }  /*128*/
    };
    __uint32_t maximalExtensions;
    Register resetVector;
    // TODO some bits hardwired to other bits in status regs...

    #pragma mark -- Queries --

    RISCV::XlenMode WidestXlen(RISCV::PrivilegeMode privilege, RISCV::XlenMode targetXlen);

    #pragma mark -- Muatations --

    void SetPrivWidthSupport(RISCV::PrivilegeMode priv, RISCV::XlenMode xlen, bool enabled);
    void SetWidthSupport(RISCV::XlenMode xlen, bool enabled);
    void SetExtensionSupport(const char* extensionString);

};
