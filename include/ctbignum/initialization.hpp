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

template <typename T = uint64_t, char... Chars>
constexpr auto chars_to_big_int(std::integer_sequence<char, Chars...>) {
  // might return a 'non-tight' representation, meaning that there could be
  // leading zero-limbs
  constexpr size_t len = sizeof...(Chars);
  constexpr size_t N = 1 + (10 * len) / (3 * std::numeric_limits<T>::digits);
  std::array<char, len> digits{Chars...};
  big_int<N, T> num{0};
  big_int<N, T> power_of_ten{1};

  for (int i = len - 1; i >= 0; --i) {
    num = accumulate(num, partial_mul<N>(big_int<1, T>{static_cast<T>(digits[i]) - 48}, power_of_ten));
    power_of_ten = partial_mul<N>(big_int<1, T>{static_cast<T>(10)}, power_of_ten);
  }
  return num;
}

template <typename T = uint64_t, char... Chars, std::size_t... Is>
constexpr auto chars_to_integer_seq(std::integer_sequence<char, Chars...>,
                 std::index_sequence<Is...>) {
  constexpr auto num = detail::chars_to_big_int(std::integer_sequence<char, Chars...>{});
  return std::integer_sequence<T,num[Is]...>{};
}

} //end of detail namespace

template <char... Chars> constexpr auto operator"" _Z() {
  //this function performs a conversion from base-10 to base-2^W twice:
  // first, to compute the tight length L
  // second, to compute the integer sequence
  
  using T = uint64_t; // TODO: how to expose this type to the user?

  constexpr auto num = detail::chars_to_big_int(std::integer_sequence<char, Chars...>{});
  constexpr auto L = detail::tight_length(num);

  return detail::chars_to_integer_seq<T>(std::integer_sequence<char, Chars...>{}, std::make_index_sequence<L>{});
}

template <size_t ExplicitLength = 0, typename T, T... Limbs>
constexpr auto to_big_int(std::integer_sequence<T, Limbs...>) {
  return big_int<ExplicitLength ? ExplicitLength : sizeof...(Limbs),T>{ Limbs... };
}

} // end of cbn namespace

#endif
