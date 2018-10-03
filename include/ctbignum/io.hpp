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
#ifndef CT_INPUTOUTPUT_HPP
#define CT_INPUTOUTPUT_HPP

#include <ctbignum/bigint.hpp>
#include <ctbignum/config.hpp>
#include <ctbignum/invariant_div.hpp>
#include <ctbignum/utility.hpp>

#include <ostream>
#include <limits>

namespace cbn {

template <size_t N, typename T>
CBN_ALWAYS_INLINE
auto to_decimal(cbn::big_int<N, T> obj) {
  // constant time?

  // Write a base-10 representation of the big-integer to the stream
  //
  // Idea of this algorithm: we "peel off" enough digits by repeatedly dividing by ten,
  // so that we are left with some digits that the built-in <<-operator can handle 
  using namespace cbn;

  constexpr size_t bit_length_upper_bound = N * std::numeric_limits<T>::digits;
  constexpr size_t max_digits = (25 * bit_length_upper_bound + 82) / 83;
  std::array<char, max_digits> decimal_chars;

  for (auto i = 0; i < max_digits; ++i) {
    auto qr = div(obj, std::integer_sequence<T,10>{});
    detail::assign(obj, qr.quotient);
    decimal_chars[max_digits - 1 - i] = 48 + static_cast<unsigned char>(qr.remainder[0]);
  }

  return decimal_chars;
}

template <size_t N, typename T>
std::ostream &operator<<(std::ostream &strm, cbn::big_int<N, T> obj) {

  // Write a base-10 representation of the big-integer to the stream
  auto buf = to_decimal(obj);

  // remove leading zeros, except the last zero if obj == 0
  int offset = 0;
  auto one_before_end = buf.cend() - 1;
  for (auto it = buf.cbegin(); it != one_before_end; ++it) {
    if (*it != 48)
      break;
    ++offset;
  }
  strm.write(buf.cbegin() + offset, buf.size() - offset);
  return strm;
}
}


#endif
