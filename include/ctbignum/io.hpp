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
#ifndef CT_INPUTOUTPUT_HPP
#define CT_INPUTOUTPUT_HPP

#include <ctbignum/bigint.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/pow.hpp>
#include <ctbignum/utility.hpp>

#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>

namespace cbn {
template <std::size_t N, typename T>
std::ostream &operator<<(std::ostream &strm, cbn::big_int<N, T> obj) {
  using namespace cbn;
  size_t bitlen = detail::bit_length(obj);
  auto power_of_ten = pow(big_int<N, T>{10}, static_cast<T>(25 * bitlen / 83));
  auto zero = big_int<N, T>{};
  bool skip_zeros = true;
  while (power_of_ten != zero) {
    auto qr = div(obj, power_of_ten);
    detail::assign(obj, qr.second);
    if (qr.first[0] > 9)
      throw std::runtime_error("division error");
    char digit = 48 + qr.first[0];
    if (digit != '0' || (digit == '0' && !skip_zeros)) {
      strm << digit;
      skip_zeros = false;
    }
    detail::assign(power_of_ten,
                   short_div(power_of_ten, static_cast<T>(10)).first);
  }
  return strm;
}

}


#endif
