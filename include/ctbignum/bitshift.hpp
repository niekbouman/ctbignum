//
//   This file is part of:
//
//   CTBignum 
//
//   C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
//   
//   Copyright 2018 Authors
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
#ifndef CT_BITSHIFTS_HPP
#define CT_BITSHIFTS_HPP

#include <ctbignum/bigint.hpp>

#include <cstddef>
#include <limits>

namespace cbn {

template <size_t N, typename T>
constexpr auto shift_right(big_int<N, T> a, size_t k) {
  // shift-right the big integer a by k bits
  big_int<N, T> res{};

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
