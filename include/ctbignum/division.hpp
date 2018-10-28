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
#ifndef CT_DIVISION_HPP
#define CT_DIVISION_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/bitshift.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/type_traits.hpp>
#include <ctbignum/utility.hpp>
#include <limits>

namespace cbn {

template <typename Q, typename R> struct DivisionResult {
  Q quotient;
  R remainder;
};

template <size_t M, typename T> constexpr  
DivisionResult<big_int<M, T>,big_int<1, T>>
short_div(big_int<M, T> u, T v) {
  using TT = typename dbl_bitlen<T>::type;
  TT r{0};
  big_int<M, T> q{};
  for (int i = M - 1; i >= 0; --i) {
    TT w = (r << std::numeric_limits<T>::digits) + u[i];
    q[i] = w / v;
    r = w % v;
  }
  return {q, {static_cast<T>(r)}};
}

template <size_t M, size_t N, typename T>
constexpr DivisionResult<big_int<M, T>, big_int<N, T>> div(big_int<M, T> u,
                                                           big_int<N, T> v) {
  // Knuth's "Algorithm D" for multiprecision division as described in TAOCP
  // Volume 2: Seminumerical Algorithms
  // combined with short division

  //
  // input:
  // u  big_int<M>,      M>=N
  // v  big_int<N>
  //
  // computes:
  // quotient = floor[ u/v ]
  // rem = u % v
  //
  // returns:
  // std::pair<big_int<N+M>, big_int<N>>(quotient, rem)

  using TT = typename dbl_bitlen<T>::type;
  size_t tight_N = N;
  while (tight_N > 0 && v[tight_N - 1] == 0)
    --tight_N;

  big_int<M, T> q{};

  if (tight_N == 1) { // short division
    TT r {};
    for (int i = M - 1; i >= 0; --i) {
      TT w = (r << std::numeric_limits<T>::digits) + u[i];
      q[i] = w / v[0];
      r = w % v[0];
    }
    return {q, {static_cast<T>(r)}};
  }

  uint8_t k = 0;
  while (v[tight_N - 1] <
         (static_cast<T>(1) << (std::numeric_limits<T>::digits - 1))) {
    ++k;
    v = detail::first<N>(shift_left(v, 1));
  }
  auto us = shift_left(u, k);

  for (int j = M - tight_N; j >= 0; --j) {
    TT tmp = us[j + tight_N - 1];
    TT tmp2 = us[j + tight_N];
    tmp += (tmp2 << std::numeric_limits<T>::digits);
    TT qhat = tmp / v[tight_N - 1];
    TT rhat = tmp % v[tight_N - 1];

    auto b = static_cast<TT>(1) << std::numeric_limits<T>::digits;
    while (qhat == b ||
           (qhat * v[tight_N - 2] >
            (rhat << std::numeric_limits<T>::digits) + us[j + tight_N - 2])) {
      qhat -= 1;
      rhat += v[tight_N - 1];
      if (rhat >= b)
        break;
    }
    auto true_value = subtract(detail::take<N + 1>(us, j, j + tight_N + 1),
                               mul(v, big_int<1, T>{{static_cast<T>(qhat)}}));
    if (true_value[tight_N]) {
      auto corrected =
          add_ignore_carry(true_value, detail::unary_encoding<N + 2, T>(tight_N + 1));
      auto new_us_part = add_ignore_carry(corrected, detail::pad<2>(v));
      for (auto i = 0; i <= tight_N; ++i)
        us[j + i] = new_us_part[i];
      --qhat;
    } else {
      for (auto i = 0; i <= tight_N; ++i)
        us[j + i] = true_value[i];
    }
    q[j] = qhat;
  }
  return {q, shift_right(detail::first<N>(us), k) };
}

template <typename T, size_t N1, size_t N2>
constexpr auto operator/(big_int<N1, T> a, big_int<N2, T> b) {
  return div(a, b).quotient;
}

template <typename T, size_t N1, size_t N2>
constexpr auto operator%(big_int<N1, T> a, big_int<N2, T> b) {
  return div(a, b).remainder;
}

}
#endif
