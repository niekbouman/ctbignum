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

template <size_t M, typename T> constexpr auto short_div(big_int<M, T> u, T v) {
  using TT = typename dbl_bitlen<T>::type;
  TT r{0};
  big_int<M, T> q{};
  for (int i = M - 1; i >= 0; --i) {
    TT w = (r << std::numeric_limits<T>::digits) + u[i];
    q[i] = w / v;
    r = w % v;
  }
  return std::make_pair(q, big_int<1, T>{{static_cast<T>(r)}});
}

template <size_t NN, size_t NplusM, typename T>
constexpr auto div(big_int<NplusM, T> u, big_int<NN, T> v) {
  // Knuth's "Algorithm D" for multiprecision division as described in TAOCP
  // Volume 2: Seminumerical Algorithms
  // combined with short division

  //
  // input:
  // u  big_int<N+M>
  // v  big_int<N>
  //
  // computes:
  // quotient = floor[ u/v ]
  // rem = u % v
  //
  // returns:
  // std::pair<big_int<N+M>, big_int<N>>(quotient, rem)

  using TT = typename dbl_bitlen<T>::type;
  size_t N = NN;
  while (N > 0 && v[N - 1] == 0)
    --N;

  big_int<NplusM, T> q{};

  if (N == 1) { // short division
    TT r {};
    for (int i = NplusM - 1; i >= 0; --i) {
      TT w = (r << std::numeric_limits<T>::digits) + u[i];
      q[i] = w / v[0];
      r = w % v[0];
    }
    return std::make_pair(q, big_int<NN, T>{{static_cast<T>(r)}});
  }

  uint8_t k = 0;
  while (v[N - 1] <
         (static_cast<T>(1) << (std::numeric_limits<T>::digits - 1))) {
    ++k;
    v = detail::first<NN>(shift_left(v, 1));
  }
  auto us = shift_left(u, k);

  size_t M = NplusM - N;
  for (int j = M; j >= 0; --j) {
    TT tmp = us[j + N - 1];
    TT tmp2 = us[j + N];
    tmp += (tmp2 << std::numeric_limits<T>::digits);
    TT qhat = tmp / v[N - 1];
    TT rhat = tmp % v[N - 1];

    auto b = static_cast<TT>(1) << std::numeric_limits<T>::digits;
    while (qhat == b ||
           (qhat * v[N - 2] >
            (rhat << std::numeric_limits<T>::digits) + us[j + N - 2])) {
      qhat -= 1;
      rhat += v[N - 1];
      if (rhat >= b)
        break;
    }
    auto true_value = subtract(detail::take<NN + 1>(us, j, j + N + 1),
                               mul(v, big_int<1, T>{{static_cast<T>(qhat)}}));
    if (true_value[N]) {
      auto corrected =
          add_ignore_carry(true_value, detail::unary_encoding<NN + 2>(N + 1));
      auto new_us_part = add_ignore_carry(corrected, detail::pad<2>(v));
      for (auto i = 0; i <= N; ++i)
        us[j + i] = new_us_part[i];
      --qhat;
    } else {
      for (auto i = 0; i <= N; ++i)
        us[j + i] = true_value[i];
    }
    q[j] = qhat;
  }
  return std::make_pair(q, shift_right(detail::first<NN>(us), k));
}
}
#endif
