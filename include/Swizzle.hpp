#pragma once

enum class ExtendBits { Zero, Sign };

template <typename XLEN_t, unsigned int applied, unsigned int... slices>
static inline constexpr XLEN_t swizzleHelper(XLEN_t inst, XLEN_t imm) {

    // Unpack the template parameter
    constexpr unsigned int numSliceInts = sizeof...(slices);
    constexpr XLEN_t values[numSliceInts] = {slices...};

    // Base case 1: No half-slice to signify leading zeroes
    if constexpr (applied == numSliceInts)
        return imm;

    // Base case 2: A final integer not part of a slice denotes leading zeroes
    else if constexpr (applied == numSliceInts-1)
        return imm << values[applied];

    // Recursive case
    else {
        // Get the first slice
        XLEN_t sliceHi = values[applied];
        XLEN_t sliceLo = values[applied+1];
        XLEN_t slice_len = sliceHi-sliceLo+1;

        // Make room for the slice and apply it
        imm <<= slice_len;
        imm |= (inst >> sliceLo) & ((1 << slice_len) - 1);
        return swizzleHelper<XLEN_t, applied+2, slices...>(inst, imm);
    }
}

template <typename XLEN_t, ExtendBits extend, unsigned int... slices>
inline constexpr XLEN_t swizzle(XLEN_t inst) {
    if constexpr (extend == ExtendBits::Sign) {
        constexpr XLEN_t values[sizeof...(slices)] = {slices...};
        XLEN_t starting_imm = inst & (1 << values[0]) ? ~0 : 0;
        return swizzleHelper<XLEN_t, 0, slices...>(inst, starting_imm);
    }
    return swizzleHelper<XLEN_t, 0, slices...>(inst, 0);
}
