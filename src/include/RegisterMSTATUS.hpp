#pragma once

#include <HartState.hpp>


class RegisterMISA : public RegisterLikeObject {

private:

    HartState *state;

public:

    RegisterMISA(HartState *owner);
    virtual __uint32_t Get32() override;
    virtual __uint64_t Get64() override;
    virtual __uint128_t Get128() override;
    virtual void Set32(__uint32_t value) override;
    virtual void Set64(__uint64_t value) override;
    virtual void Set128(__uint128_t value) override;

};


class RegisterSATP : public RegisterLikeObject {

private:

    HartState* state;

public:

    RegisterSATP(HartState *owner);
    virtual __uint32_t Get32() override;
    virtual __uint64_t Get64() override;
    virtual __uint128_t Get128() override;
    virtual void Set32(__uint32_t value) override;
    virtual void Set64(__uint64_t value) override;
    virtual void Set128(__uint128_t value) override;

};


class RegisterMSTATUS : public RegisterLikeObject {

private:

    HartState *state;

public:

    RegisterMSTATUS(HartState *owner);
    virtual __uint32_t Get32() override;
    virtual __uint64_t Get64() override;
    virtual __uint128_t Get128() override;
    virtual void Set32(__uint32_t value) override;
    virtual void Set64(__uint64_t value) override;
    virtual void Set128(__uint128_t value) override;

};


class RegisterSSTATUS : public RegisterLikeObject {

private:

    HartState *state;

public:

    RegisterSSTATUS(HartState *owner);
    virtual __uint32_t Get32() override;
    virtual __uint64_t Get64() override;
    virtual __uint128_t Get128() override;
    virtual void Set32(__uint32_t value) override;
    virtual void Set64(__uint64_t value) override;
    virtual void Set128(__uint128_t value) override;

};


class RegisterUSTATUS : public RegisterLikeObject {

private:

    HartState *state;

public:

    RegisterUSTATUS(HartState *owner);
    virtual __uint32_t Get32() override;
    virtual __uint64_t Get64() override;
    virtual __uint128_t Get128() override;
    virtual void Set32(__uint32_t value) override;
    virtual void Set64(__uint64_t value) override;
    virtual void Set128(__uint128_t value) override;

};
