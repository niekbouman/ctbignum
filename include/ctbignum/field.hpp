#ifndef CT_FIELD_HPP
#define CT_FIELD_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>

namespace cbn {

template <size_t... Modulus> struct Zq {
  template <typename T, size_t N>
  constexpr auto add(big_int<N, T> a, big_int<N, T> b) {
    big_int<sizeof...(Modulus), T> modulus{{Modulus...}};
    return mod_add(a, b, modulus);
  }
};
}
#endif
