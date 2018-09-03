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

namespace cbn {

template <size_t N, typename T = uint64_t,
          typename = std::enable_if_t<std::is_integral<T>::value>>
struct big_int : std::array<T, N> {};

}

#endif
