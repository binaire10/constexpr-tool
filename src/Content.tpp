#ifndef CONTENT_TPP
#define CONTENT_TPP
#pragma once

#include "Content.hpp"

namespace meta {
    template<typename type_t, type_t... str>
    constexpr Content<type_t, str...>::Content() noexcept
    {}

    template<typename type_t, type_t... str>
    constexpr typename Content<type_t, str...>::array_type Content<type_t, str...>::s_data;

    template<typename type_t, type_t... str>
    constexpr size_t Content<type_t, str...>::Size;

    template<typename type_t, type_t... str>
        template<type_t... arg2>
    constexpr Content<type_t, str..., arg2...> Content<type_t, str...>::operator +(Content<type_t, arg2...>)
    {return {};}

    template<typename type_t, type_t... str>
    constexpr const typename Content<type_t, str...>::array_type &Content<type_t, str...>::get()
    { return s_data; }

    template<typename type_t, type_t... str>
    constexpr Content<type_t, str...>::operator const type_t *() const
    {
        return s_data;
    }


    template<char... str>
    constexpr Content<char, str...>::Content() noexcept
    {}

    template<char... str>
    constexpr typename Content<char, str...>::array_type Content<char, str...>::s_data;

    template<char... str>
    constexpr size_t Content<char, str...>::Size;

    template<char... str>
        template<char... arg2>
    constexpr Content<char, str..., arg2...> Content<char, str...>::operator +(Content<char, arg2...>)
    {return {};}

    template<char... str>
    constexpr const typename Content<char, str...>::array_type &Content<char, str...>::get()
    { return s_data; }

    template<char... str>
    constexpr Content<char, str...>::operator const char *() const
    {
        return s_data;
    }
}

#endif // CONTENT_TPP
