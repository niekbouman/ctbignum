#ifndef CT_FIELD_HPP
#define CT_FIELD_HPP

#include "addition.hpp"
#include <cstddef>

template <size_t... Modulus> struct Zq {
  template <template <typename, size_t> class Array, typename T, size_t N>
  constexpr auto add(Array<T, N> a, Array<T, N> b) {
    Array<T, sizeof...(Modulus)> modulus{{Modulus...}};
    return mod_add(a, b, modulus);
  }
};

#endif
