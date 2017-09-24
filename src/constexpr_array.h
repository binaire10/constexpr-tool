#ifndef CONSTEXPR_ARRAY_H
#define CONSTEXPR_ARRAY_H

#include <utility>

template<typename T, std::size_t n>
struct constexpr_array {
    typedef const T(&array_const_reference)[n];
    const T array[n];

    constexpr std::size_t size() const {
        return n;
    }

    constexpr const array_const_reference data() const {
        return array;
    }
};

#endif // CONSTEXPR_ARRAY_H
