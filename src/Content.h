#ifndef CONTENT_H
#define CONTENT_H
#pragma once

#include <type_traits>
#include <unistd.h>

namespace meta
{
    template<typename type_t, type_t... str>
    struct Content
    {
        typedef type_t (array_type)[sizeof... (str)];

        constexpr Content() noexcept;

        static constexpr const array_type &get();

        constexpr operator const type_t *() const;

        template<type_t... arg2>
        constexpr Content<type_t, str..., arg2...> operator +(Content<type_t, arg2...>);

        static constexpr size_t Size = sizeof... (str);
        static constexpr array_type s_data = {str...};
    };

    template<char... str>
    struct Content<char, str...>
    {
        typedef char (array_type)[sizeof... (str)+1];

        constexpr Content() noexcept;

        static constexpr const array_type &get();

        constexpr operator const char *() const;

        template<char... arg2>
        constexpr Content<char, str..., arg2...> operator +(Content<char, arg2...>);

        static constexpr size_t Size = sizeof... (str);
        static constexpr array_type s_data = {str..., char(0)};
    };
}

#endif // CONTENT_H
