#pragma once

#include <cstdint>

template<typename XLEN_t>
struct SignedXLEN {
    using type = XLEN_t;
};

template<>
struct SignedXLEN<__uint32_t> {
    using type = __int32_t;
};

template<>
struct SignedXLEN<__uint64_t> {
    using type = __int64_t;
};

template<>
struct SignedXLEN<__uint128_t> {
    using type = __int128_t;
};
