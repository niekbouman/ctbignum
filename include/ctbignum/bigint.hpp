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
#ifndef CT_BIGINT_HPP
#define CT_BIGINT_HPP

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>

namespace cbn {

template <size_t N, std::unsigned_integral T = uint64_t>
struct big_int : std::array<T, N> {};

}

#endif
