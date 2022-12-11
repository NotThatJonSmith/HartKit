#pragma once

template<typename XLEN_t>
inline void print_wfi(__uint32_t encoding, std::ostream* out) {
    *out << "wfi" << std::endl;
}

template<typename XLEN_t>
inline void print_uret(__uint32_t encoding, std::ostream* out) {
    *out << "uret" << std::endl;
}

template<typename XLEN_t>
inline void print_sret(__uint32_t encoding, std::ostream* out) {
    *out << "sret" << std::endl;
}

template<typename XLEN_t>
inline void print_mret(__uint32_t encoding, std::ostream* out) {
    *out << "mret" << std::endl;
}

template<typename XLEN_t>
inline void print_sfencevma(__uint32_t encoding, std::ostream* out) {
    *out << "sfence.vma" << std::endl;
}
