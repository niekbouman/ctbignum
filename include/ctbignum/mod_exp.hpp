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
#ifndef CT_MODULAR_EXP_HPP
#define CT_MODULAR_EXP_HPP

#include <ctbignum/bigint.hpp>
#include <ctbignum/montgomery.hpp>
#include <ctbignum/slicing.hpp>

#include <cstddef>
#include <limits>

namespace cbn {

template <std::size_t N1, std::size_t N2, typename T, T... Modulus>
constexpr auto mod_exp(big_int<N1, T> a, big_int<N2, T> exp,
                       std::integer_sequence<T, Modulus...> modulus) {
  // modular exponentiation using Montgomery multiplication

  constexpr auto N = modulus.size();
  constexpr big_int<N, T> m{Modulus...};
  constexpr auto R_mod_m = div(detail::unary_encoding<N, N + 1>(), m).second;
  constexpr auto Rsq_mod_m = div(detail::unary_encoding<2 * N, 2 * N + 1>(), m).second;

  auto result = R_mod_m;
  auto base = montgomery_mul(a, Rsq_mod_m, modulus);

  big_int<N2, T> zero{};
  if (exp == zero)
    return big_int<N, T>{1};
  if (m == big_int<N, T>{1})
    return big_int<N, T>{0};

  while (true) {
    auto lsb = exp[0] & 1;
    exp = shift_right(exp, 1);
    if (lsb) {
      result = montgomery_mul(base, result, modulus);
      if (exp == zero)
        break;
    }
    base = montgomery_mul(base, base, modulus);
  }

  return montgomery_mul(result, big_int<N, T>{1}, modulus);
}

}
#endif
