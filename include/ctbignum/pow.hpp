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
#ifndef CT_POW_HPP
#define CT_POW_HPP

#include <ctbignum/bigint.hpp>
#include <ctbignum/mult.hpp>

#include <cstddef>

namespace cbn {

template <std::size_t N1, typename T>
constexpr auto pow(big_int<N1, T> base, T exp) {

  big_int<N1, T> result{1};

  if (exp == 0)
    return result;

  while (true) {
    auto lsb = exp & 1;
    exp >>= 1;
    if (lsb) {
      result = partial_mul<N1>(base, result);
      if (exp == 0)
        break;
    }
    base = partial_mul<N1>(base, base);
  }

  return result;
}
}
#endif
