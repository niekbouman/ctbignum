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
#ifndef CT_BIGINT_HPP
#define CT_BIGINT_HPP

#include <array>

namespace cbn {

template <size_t N, typename T = uint64_t,
          typename = std::enable_if_t<std::is_integral<T>::value>>
struct big_int {
  std::array<T, N> repr;

  constexpr T &operator[](size_t i) { return repr[i]; }
  constexpr const T &operator[](size_t i) const { return repr[i]; }
  constexpr size_t size() const noexcept { return repr.size(); }
};
}

#endif
