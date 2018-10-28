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
#ifndef CT_MULT_HPP
#define CT_MULT_HPP

#include <ctbignum/bigint.hpp>
#include <ctbignum/config.hpp>
#include <ctbignum/type_traits.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>

namespace cbn {

template <typename T, std::size_t N>
CBN_ALWAYS_INLINE 
constexpr auto short_mul(big_int<N, T> a, T b) {

  using TT = typename dbl_bitlen<T>::type;
  big_int<N + 1, T> p{};
  T k = 0;
  for (auto j = 0; j < N; ++j) {
    TT t = static_cast<TT>(a[j]) * static_cast<TT>(b) + k;
    p[j] = t;
    k = t >> std::numeric_limits<T>::digits;
  }
  p[N] = k;
  return p;
}

template <size_t padding_limbs = 0, size_t M, size_t N, typename T>
CBN_ALWAYS_INLINE 
constexpr auto mul(big_int<M, T> u, big_int<N, T> v) {

  using TT = typename dbl_bitlen<T>::type;
  big_int<M + N + padding_limbs, T> w{};
  for (auto j = 0; j < N; ++j) {
    // if (v[j] == 0)
    //  w[j + M] = static_cast<uint64_t>(0);
    // else {
    T k = 0;
    for (auto i = 0; i < M; ++i) {
      TT t = static_cast<TT>(u[i]) * static_cast<TT>(v[j]) + w[i + j] + k;
      w[i + j] = static_cast<T>(t);
      k = t >> std::numeric_limits<T>::digits;
    }
    w[j + M] = k;
    //}
  }
  return w;
}

template <size_t ResultLength, size_t M, size_t N, typename T>
constexpr auto partial_mul(big_int<M, T> u, big_int<N, T> v) {

  using TT = typename dbl_bitlen<T>::type;
  big_int<ResultLength, T> w{};
  for (auto j = 0; j < N; ++j) {
    // if (v[j] == 0) {
    //  if (j + M < ResultLength)
    //    w[j + M] = static_cast<T>(0);
    //} else {
    T k = 0;
    const auto m = std::min(M, ResultLength - j);
    for (auto i = 0; i < m; ++i) {
      TT t = static_cast<TT>(u[i]) * static_cast<TT>(v[j]) + w[i + j] + k;
      w[i + j] = static_cast<T>(t);
      k = t >> std::numeric_limits<T>::digits;
    }
    if (j + M < ResultLength)
      w[j + M] = k;
    //}
  }
  return w;
}

template <typename T, size_t N1, size_t N2>
constexpr auto operator*(big_int<N1, T> a, big_int<N2, T> b) {
  return mul(a, b);
}

}
#endif
