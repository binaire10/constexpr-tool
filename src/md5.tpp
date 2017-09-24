#ifndef MD5_TPP
#define MD5_TPP

#include "md5.h"
#include "index_generator.hpp"

namespace binaire_constexpr
{
    namespace tool_md5
    {
        namespace
        {

            template<std::size_t n, std::size_t pos>
            struct md5_buffer_helper
            {
                static constexpr char get(const char *data)
                {
                    return (pos <= n) ? data[pos] : 0;
                }
            };

            template<std::size_t n>
            struct md5_buffer_helper<n, n>
            {
                static constexpr char get(const char *)
                {
                    return 0x80;
                }
            };

            constexpr char toByte_md5(uint32_t len, std::size_t i)
            {
                return len==0?0:static_cast<char>(len >> (8*i));
            }

            template<typename Function, std::size_t n, std::size_t... value>
            constexpr auto make_md5_buffer_helper(Function &&f, const char (&data)[n], meta::Content<std::size_t, value...>)
            {
                return f(md5_buffer_helper<n, value>::get(data)..., toByte_md5(n<<3, 0), toByte_md5(n<<3, 1), toByte_md5(n<<3, 2), toByte_md5(n<<3, 3)
                         , toByte_md5(n>>29, 4), toByte_md5(n>>29, 5), toByte_md5(n>>29, 6), toByte_md5(n>>29, 7));
            }
            // for (new_len = initial_len + 1; new_len % (512/8) != 448/8; ++new_len)

            constexpr std::size_t md5_new_size(std::size_t initial_len)
            {
                std::size_t new_len = initial_len + 1;
                for (; new_len % (512/8) != 448/8; ++new_len);
                return new_len;
            }

            template<typename Function, std::size_t n>
            constexpr auto make_md5_buffer(Function &&f, const char (&data)[n])
            {
                return make_md5_buffer_helper(f, data, meta::index_generator<md5_new_size(n)>());
            }

            struct MakeMd5Pad
            {
                template<std::size_t step, typename... arg>
                struct ToUInt32
                {
                    static_assert((sizeof...(arg)%4) == 0, "impair argument");

                    template<typename... argsq>
                    constexpr static uint32_t toUInt32(char, char, char, char, argsq... args)
                    {
                        return ToUInt32<step-1, argsq...>::toUInt32(std::forward<argsq>(args)...);
                    }
                };

                template<std::size_t... size, typename... arg>
                static constexpr constexpr_array<uint32_t, sizeof...(arg)/4> make_uintBuffer(meta::Content<std::size_t, size...>, arg... args)
                {
                    return constexpr_array<uint32_t, sizeof...(arg)/4>{ToUInt32<size, arg...>::toUInt32(std::forward<arg>(args)...)...};
                }

                template<typename... arg>
                static constexpr constexpr_array<uint32_t, sizeof...(arg)/4> make_uintBuffer(arg... args)
                {
                    return make_uintBuffer(meta::index_generator<sizeof...(arg)/4>(), std::forward<arg>(args)...);
                }

                template<typename... arg>
                constexpr auto operator()(arg...s) const
                {
                    return make_uintBuffer<arg...>(std::forward<arg>(s)...);
                }
            };

            template<typename... arg>
            struct MakeMd5Pad::ToUInt32<0, char, char, char, char, arg...>
            {
                // convert each 4 char into uint32_t value
                static constexpr uint32_t toUInt32(char a, char b, char c, char d, arg...)
                {
                    return static_cast<uint32_t>(a&0xff) | static_cast<uint32_t>((b&0xff)) << 8ull | static_cast<uint32_t>((c&0xff)) << 16ull | static_cast<uint32_t>((d&0xff)) << 24ull;
                }
            };

            constexpr uint32_t LeftRotate(uint32_t x, uint32_t c)
            {
                return (((x&0xffffffff) << (c)) | ((x&0xffffffff) >> (32 - (c))));
            }
        }
        template<std::size_t n>
        constexpr constexpr_array<uint32_t, 4> md5_hard(const uint32_t (&w)[n], uint32_t h0, uint32_t h1, uint32_t h2, uint32_t h3)
        {
            // origin code of this function
            // https://web.njit.edu/~hz278/md5/md5_orig.c

            static_assert(n%16==0, "marche pas");
            constexpr const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                                            5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                                            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                                            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
            constexpr const uint32_t k[64] = {
                0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
                0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
                0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
                0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
                0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
                0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
                0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
                0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
                0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
                0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
                0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
                0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
                0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
                0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
                0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
                0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

            uint32_t f = 0;
            uint32_t g = 0;
            uint32_t temp = 0;

            uint32_t i = 0;
            uint32_t a = h0;
            uint32_t b = h1;
            uint32_t c = h2;
            uint32_t d = h3;
            uint32_t offset=0;

            for(offset=0; offset<n; offset += (512/8)) {
                // Initialize hash value for this chunk:
                // Main loop:
                for(i = 0; i<64; i++) {

                    if (i < 16) {
                        f = (b & c) | ((~b) & d);
                        g = i;
                    } else if (i < 32) {
                        f = (d & b) | ((~d) & c);
                        g = (5*i + 1) % 16;
                    } else if (i < 48) {
                        f = b ^ c ^ d;
                        g = (3*i + 5) % 16;
                    } else {
                        f = c ^ (b | (~d));
                        g = (7*i) % 16;
                    }

                    temp = d;
                    d = c;
                    c = b;
                    b = b + LeftRotate((a + f + k[i] + w[g+offset]), r[i]);
                    a = temp;

                }

                // Add this chunk's hash to result so far:
                h0 += a;
                h1 += b;
                h2 += c;
                h3 += d;

            }
            return constexpr_array<uint32_t, 4>{h0, h1, h2, h3};
        }
    }

    template<std::size_t n>
    constexpr constexpr_array<uint32_t, 4> md5(const char (&data)[n])
    {
        return tool_md5::md5_hard(tool_md5::make_md5_buffer(tool_md5::MakeMd5Pad(), data).data());
    }

}

#endif // MD5_TPP
