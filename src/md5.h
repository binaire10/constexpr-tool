#ifndef MD5_H
#define MD5_H

#include <cstdint>
#include "constexpr_array.h"

namespace binaire_constexpr
{
    namespace tool_md5
    {
        template<std::size_t n>
        constexpr constexpr_array<uint32_t, 4> md5_hard(const uint32_t (&w)[n], uint32_t h0 = 0x67452301, uint32_t h1 = 0xefcdab89, uint32_t h2 = 0x98badcfe, uint32_t h3 = 0x10325476);
    }

    template<std::size_t n>
    constexpr constexpr_array<uint32_t, 4> md5(const char (&data)[n]);

}

#endif // MD5_H
