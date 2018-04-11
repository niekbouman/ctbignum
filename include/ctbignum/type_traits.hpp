//
//   This file is part of:
//
//   CTBignum 
//
//   C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
//   
//   See the NOTICE file for the complete list of authors and contributors 
//
//   Copyright 2018 Niek J. Bouman
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
#ifndef CT_TYPE_TRAITS_HPP
#define CT_TYPE_TRAITS_HPP

#include <cstdint>

namespace cbn {

template <typename T> struct dbl_bitlen { using type = void; };
template <> struct dbl_bitlen<uint8_t> { using type = uint16_t; };
template <> struct dbl_bitlen<uint16_t> { using type = uint32_t; };
template <> struct dbl_bitlen<uint32_t> { using type = uint64_t; };
template <> struct dbl_bitlen<uint64_t> { using type = __uint128_t; };
}
#endif
