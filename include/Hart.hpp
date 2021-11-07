#pragma once

#include <HartState.hpp>
#include <HartSpec.hpp>

#include <Tickable.hpp>

class Hart : public CASK::Tickable {

public:

    HartState state;
    HartSpec spec;

};
