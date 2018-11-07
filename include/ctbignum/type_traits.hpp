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
#ifndef CT_TYPE_TRAITS_HPP
#define CT_TYPE_TRAITS_HPP

#include <cstdint>

namespace cbn {

template <typename T> struct dbl_bitlen { using type = void; };
template <> struct dbl_bitlen<uint8_t> { using type = uint16_t; };
template <> struct dbl_bitlen<uint16_t> { using type = uint32_t; };
template <> struct dbl_bitlen<uint32_t> { using type = uint64_t; };
template <> struct dbl_bitlen<uint64_t> { using type = __uint128_t; };
//template <> struct dbl_bitlen<unsigned long> { using type = __uint128_t; };
}
#endif
