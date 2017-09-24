#ifndef INDEX_GENERATOR_H
#define INDEX_GENERATOR_H

#include <utility>
#include "Content.hpp"

namespace meta
{
    template<std::size_t count, typename = void, std::size_t... value>
    struct index_generator_meta : index_generator_meta<count-1, void, count-1, value...>
    {};

    template<std::size_t... value>
    struct index_generator_meta<0, void, value...>
    {
        using type = meta::Content<std::size_t, value...>;
    };

    // index_generator generate number from 0 to count into meta::Content
    template<std::size_t count>
    using index_generator = typename index_generator_meta<count>::type;
}

#endif // INDEX_GENERATOR_H
