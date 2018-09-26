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
#ifndef CT_BITSHIFTS_HPP
#define CT_BITSHIFTS_HPP

#include <ctbignum/bigint.hpp>
#include <ctbignum/slicing.hpp>

#include <cstddef>
#include <limits>

namespace cbn {

template <size_t N, typename T>
constexpr auto shift_right(big_int<N, T> a, size_t k) {
  // shift-right the big integer a by k bits
  big_int<N, T> res{};

  if (k == 0) return a;
  
  for (auto i = 0; i < N - 1; ++i) {
    res[i] = (a[i] >> k) | (a[i + 1] << (std::numeric_limits<T>::digits - k));
  }
  res[N - 1] = (a[N - 1] >> k);
  return res;
}

template <size_t N, typename T>
constexpr auto shift_left(big_int<N, T> a, size_t k) {
  // shift-left the big integer a by k bits
  // answer has 1 limb more
  //
  
  if (k == 0) return detail::pad<1>(a);

  big_int<N + 1, T> res{};

  res[0] = (a[0] << k);

  for (auto i = 1; i < N; ++i) {
    res[i] = (a[i] << k) | (a[i - 1] >> (std::numeric_limits<T>::digits - k));
  }

  res[N] = a[N - 1] >> (std::numeric_limits<T>::digits - k);
  return res;
}
}
#endif
