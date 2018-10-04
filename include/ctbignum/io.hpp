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

template <typename T> struct Radix10 {
  using character_t = char;
  static constexpr T radix = 10;
  static constexpr size_t
  representation_length_upper_bound(size_t bit_length_upper_bound) {
    return (25 * bit_length_upper_bound + 82) / 83; // 25/83 =approx= log(2)/log(10)
  }
  static character_t represent(T x) { return 48 + static_cast<character_t>(x); }
  static constexpr const char *prefix = "";
};

template <typename T> struct Radix16 {
  using character_t = char;
  static constexpr T radix = 16;
  static constexpr size_t
  representation_length_upper_bound(size_t bit_length_upper_bound) {
    return (bit_length_upper_bound + 3) / 4;
  }
  static character_t represent(T x) {
    if (x <= 9)
      return 48 + static_cast<character_t>(x); // ascii: 0..9
    else
      return 87 + static_cast<character_t>(x); // ascii: a..f
  }
  static constexpr const char *prefix = "0x";
};

template <class Radix, size_t N, typename T>
auto convert_radix(cbn::big_int<N, T> obj) {
  // Return a representation of the big-integer in a user-specified radix
  //
  // this should be constant time (not verified yet)

  using namespace cbn;

  constexpr size_t bit_length_upper_bound = N * std::numeric_limits<T>::digits;
  constexpr size_t max_digits =
      Radix::representation_length_upper_bound(bit_length_upper_bound);
  std::array<typename Radix::character_t, max_digits> radix_repr;

  for (auto i = 0; i < max_digits; ++i) {
    auto qr = div(obj, std::integer_sequence<T, Radix::radix>{});
    detail::assign(obj, qr.quotient);
    radix_repr[max_digits - 1 - i] = Radix::represent(qr.remainder[0]);
  }

  return radix_repr;
}

template <size_t N, typename T>
std::ostream &operator<<(std::ostream &strm, cbn::big_int<N, T> num) {

  // Write a base-10 representation of the big-integer to the stream
  using Radix = Radix10<T>;
  auto buf = convert_radix<Radix>(num);

  // remove leading zeros, except the last zero if obj == 0
  int offset = 0;
  auto one_before_end = buf.cend() - 1;
  for (auto it = buf.cbegin(); it != one_before_end; ++it) {
    if (*it != Radix::represent(static_cast<T>(0)))
      break;
    ++offset;
  }
  strm << Radix::prefix;
  strm.write(buf.cbegin() + offset, buf.size() - offset);
  return strm;
}
}

#endif
