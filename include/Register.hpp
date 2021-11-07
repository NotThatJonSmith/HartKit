#pragma once


#include <RegisterLikeObject.hpp>


class Register : public RegisterLikeObject {

private:

    union { __uint32_t w; __uint64_t dw; __uint128_t qw; } bits;

public:

    bool zero = false;

    virtual ~Register();
    virtual __uint32_t Get32() override;
    virtual __uint64_t Get64() override;
    virtual __uint128_t Get128() override;
    virtual void Set32(__uint32_t value) override;
    virtual void Set64(__uint64_t value) override;
    virtual void Set128(__uint128_t value) override;

};
