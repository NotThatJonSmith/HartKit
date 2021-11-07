#include <RegisterMSTATUS.hpp>

RegisterMISA::RegisterMISA(HartState *owner) : state(owner) {
}

__uint32_t RegisterMISA::Get32() {
    return state->GetMISA<__uint32_t>();
}

__uint64_t RegisterMISA::Get64() {
    return state->GetMISA<__uint64_t>();
}

__uint128_t RegisterMISA::Get128() {
    return state->GetMISA<__uint128_t>();
}

void RegisterMISA::Set32(__uint32_t value) {
    state->SetMISA<__uint32_t>(value);
}

void RegisterMISA::Set64(__uint64_t value) {
    state->SetMISA<__uint64_t>(value);
}

void RegisterMISA::Set128(__uint128_t value) {
    state->SetMISA<__uint128_t>(value);
}

RegisterSATP::RegisterSATP(HartState *owner) : state(owner) {
}

__uint32_t RegisterSATP::Get32() {
    return state->GetSATP<__uint32_t>();
}

__uint64_t RegisterSATP::Get64() {
    return state->GetSATP<__uint64_t>();
}

__uint128_t RegisterSATP::Get128() {
    return state->GetSATP<__uint128_t>();
}

void RegisterSATP::Set32(__uint32_t value) {
    state->SetSATP<__uint32_t>(value);
}

void RegisterSATP::Set64(__uint64_t value) {
    state->SetSATP<__uint64_t>(value);
}

void RegisterSATP::Set128(__uint128_t value) {
    state->SetSATP<__uint128_t>(value);
}

RegisterMSTATUS::RegisterMSTATUS(HartState *owner) : state(owner) {
}

__uint32_t RegisterMSTATUS::Get32() {
    return state->GetMSTATUS<__uint32_t, RISCV::PrivilegeMode::Machine>();
}

__uint64_t RegisterMSTATUS::Get64() {
    return state->GetMSTATUS<__uint64_t, RISCV::PrivilegeMode::Machine>();
}

__uint128_t RegisterMSTATUS::Get128() {
    return state->GetMSTATUS<__uint128_t, RISCV::PrivilegeMode::Machine>();
}

void RegisterMSTATUS::Set32(__uint32_t value) {
    state->SetMSTATUS<__uint32_t, RISCV::PrivilegeMode::Machine>(value);
}

void RegisterMSTATUS::Set64(__uint64_t value) {
    state->SetMSTATUS<__uint64_t, RISCV::PrivilegeMode::Machine>(value);
}

void RegisterMSTATUS::Set128(__uint128_t value) {
    state->SetMSTATUS<__uint128_t, RISCV::PrivilegeMode::Machine>(value);
}

RegisterSSTATUS::RegisterSSTATUS(HartState *owner) : state(owner) {
}

__uint32_t RegisterSSTATUS::Get32() {
    return state->GetMSTATUS<__uint32_t, RISCV::PrivilegeMode::Supervisor>();
}

__uint64_t RegisterSSTATUS::Get64() {
    return state->GetMSTATUS<__uint64_t, RISCV::PrivilegeMode::Supervisor>();
}

__uint128_t RegisterSSTATUS::Get128() {
    return state->GetMSTATUS<__uint128_t, RISCV::PrivilegeMode::Supervisor>();
}

void RegisterSSTATUS::Set32(__uint32_t value) {
    state->SetMSTATUS<__uint32_t, RISCV::PrivilegeMode::Supervisor>(value);
}

void RegisterSSTATUS::Set64(__uint64_t value) {
    state->SetMSTATUS<__uint64_t, RISCV::PrivilegeMode::Supervisor>(value);
}

void RegisterSSTATUS::Set128(__uint128_t value) {
    state->SetMSTATUS<__uint128_t, RISCV::PrivilegeMode::Supervisor>(value);
}

RegisterUSTATUS::RegisterUSTATUS(HartState *owner) : state(owner) {
}

__uint32_t RegisterUSTATUS::Get32() {
    return state->GetMSTATUS<__uint32_t, RISCV::PrivilegeMode::User>();
}

__uint64_t RegisterUSTATUS::Get64() {
    return state->GetMSTATUS<__uint64_t, RISCV::PrivilegeMode::User>();
}

__uint128_t RegisterUSTATUS::Get128() {
    return state->GetMSTATUS<__uint128_t, RISCV::PrivilegeMode::User>();
}

void RegisterUSTATUS::Set32(__uint32_t value) {
    state->SetMSTATUS<__uint32_t, RISCV::PrivilegeMode::User>(value);
}

void RegisterUSTATUS::Set64(__uint64_t value) {
    state->SetMSTATUS<__uint64_t, RISCV::PrivilegeMode::User>(value);
}

void RegisterUSTATUS::Set128(__uint128_t value) {
    state->SetMSTATUS<__uint128_t, RISCV::PrivilegeMode::User>(value);
}
