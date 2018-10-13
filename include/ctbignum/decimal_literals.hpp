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
#ifndef CT_STRINGINIT_HPP
#define CT_STRINGINIT_HPP

#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/utility.hpp>

#include <cstddef>
#include <limits>

namespace cbn {
namespace detail {

template <typename T = uint64_t, size_t L = 0, char... Chars> //, std::size_t... Is>
constexpr auto 
chars_to_big_int(std::integer_sequence<char, Chars...>) {
  // might return a 'non-tight' representation, meaning that there could be
  // leading zero-limbs
  constexpr size_t len = sizeof...(Chars);
  constexpr size_t N = std::max(L, 1 + (10 * len) / (3 * std::numeric_limits<T>::digits));
  std::array<char, len> digits{Chars...};
  big_int<N, T> num{0};
  big_int<N, T> power_of_ten{1};

  for (int i = len - 1; i >= 0; --i) {
    num = add_ignore_carry(num, partial_mul<N>(big_int<1, T>{static_cast<T>(digits[i]) - 48}, power_of_ten));
    power_of_ten = partial_mul<N>(big_int<1, T>{static_cast<T>(10)}, power_of_ten);
  }
  return num;
}

template <typename T = uint64_t, char... Chars, std::size_t... Is>
constexpr auto chars_to_integer_seq(std::integer_sequence<char, Chars...>,
                                    std::index_sequence<Is...>) {
  constexpr auto num = detail::chars_to_big_int<T, sizeof...(Chars)>(std::integer_sequence<char, Chars...>{});
  return std::integer_sequence<T, num[Is]...>{};
}

} //end of detail namespace

namespace literals {
template <char... Chars> constexpr auto operator"" _Z() {

  using T = uint64_t; // Question: How to elegantly expose the choice of this
                      // type to the user?

  constexpr size_t len = sizeof...(Chars);
  constexpr size_t N = 1 + (10 * len) / (3 * std::numeric_limits<T>::digits);

  auto num = detail::chars_to_integer_seq(std::integer_sequence<char, Chars...>{}, std::make_index_sequence<N>{});
  constexpr auto L = detail::tight_length(num) + (to_big_int(num) == big_int<1, T>{});
  return detail::take_first(num, std::make_index_sequence<L>{});
}
}

} // end of cbn namespace

#endif
