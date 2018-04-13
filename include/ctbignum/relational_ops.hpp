//
// This file is part of
//
// CTBignum 	
//
// C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
// 
//
// This file is distributed under the Apache License, Version 2.0. See the LICENSE
// file for details.
#ifndef CT_RELATIONAL_HPP
#define CT_RELATIONAL_HPP

#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/slicing.hpp>

#include <algorithm>
#include <cstddef>

namespace cbn {
namespace detail {

template <size_t N, typename T>
constexpr bool equal(big_int<N, T> a, big_int<N, T> b) {
  size_t x = 0;
  for (auto i = 0; i < N; ++i)
    x += (a[i] != b[i]);
  return (x == 0);
}

template <size_t N, typename T>
constexpr bool less_than(big_int<N, T> a, big_int<N, T> b) {

  return subtract(a, b)[N];
}
} // end of detail namespace

template <typename T, size_t N1, size_t N2>
constexpr bool operator==(big_int<N1, T> a, big_int<N2, T> b) {
  constexpr auto L = std::max(N1, N2);
  return detail::equal(detail::pad<L - N1>(a), detail::pad<L - N2>(b));
}

template <typename T, size_t N1, size_t N2>
constexpr bool operator!=(big_int<N1, T> a, big_int<N2, T> b) {
  return !(a==b);
}

template <typename T, size_t N1, size_t N2>
constexpr bool operator<(big_int<N1, T> a, big_int<N2, T> b) {
  constexpr auto L = std::max(N1, N2);
  return detail::less_than(detail::pad<L - N1>(a), detail::pad<L - N2>(b));
}

template <typename T, size_t N1, size_t N2>
constexpr bool operator>(big_int<N1, T> a, big_int<N2, T> b) {
  return (b < a);
}

template <typename T, size_t N1, size_t N2>
constexpr bool operator<=(big_int<N1, T> a, big_int<N2, T> b) {
  return !(b < a);
}

template <typename T, size_t N1, size_t N2>
constexpr bool operator>=(big_int<N1, T> a, big_int<N2, T> b) {
  return  !(a < b);
}

}
#endif
