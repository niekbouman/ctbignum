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
#ifndef CT_INVAR_DIV_HPP
#define CT_INVAR_DIV_HPP

#include <ctbignum/addition.hpp>
#include <ctbignum/config.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/utility.hpp>
#include <ctbignum/decimal_literals.hpp>

#include <cstddef> // std::size_t
#include <limits>

namespace cbn {
namespace detail {

template <std::size_t N, typename T = uint64_t, T... Divisor, std::size_t... Is>
constexpr auto precompute_m_prime_nontight(std::integer_sequence<T, Divisor...>,
                                           std::index_sequence<Is...>) {
  constexpr auto D = sizeof...(Divisor);
  constexpr big_int<D> d{Divisor...};
  constexpr auto ell = bit_length(d);
  constexpr auto w = std::numeric_limits<T>::digits;
  constexpr auto limb_shifts = ell / w;
  constexpr auto bit_shifts = ell % w;
  constexpr auto pow2ell = place_at<D>(static_cast<T>(1) << bit_shifts, limb_shifts);
  constexpr auto pow2N = unary_encoding<N, N + 1>();
  constexpr auto divrem = div(mul(pow2N, subtract(pow2ell, d)), d);
  constexpr auto mp = to_length<N>(add(divrem.first, big_int<1>{static_cast<T>(1)}));
  return std::integer_sequence<T, mp[Is]...>{};
}

template <std::size_t N, typename T = uint64_t, T... Divisor>
constexpr auto precompute_m_prime(std::integer_sequence<T, Divisor...>) {
  auto m = precompute_m_prime_nontight<N>(std::integer_sequence<T, Divisor...>{},
                                       std::make_index_sequence<N>{});
  return take_first(m, std::make_index_sequence<tight_length(m)>{});
}

} // end of detail namespace

template <typename T, std::size_t N1, T... Divisor>
CBN_ALWAYS_INLINE constexpr auto div(big_int<N1, T> n,
                                     std::integer_sequence<T, Divisor...>) {
  // Integer division with compile-time divisor
  // as described in "Division by Invariant Integers using Multiplication",
  // by Granlund and Montgomery, 1994
  // https://gmplib.org/~tege/divcnst-pldi94.pdf
  //
  // inputs:
  //  n           divident
  //  Divisor...  compile-time divisor
  //

  using detail::skip;
  using detail::first;

  // Compile-time precomputation of m_prime
  constexpr big_int<sizeof...(Divisor), T> d{Divisor...};
  constexpr auto ell = detail::bit_length(d);
  static_assert(d > to_big_int(1_Z));

  constexpr auto w = std::numeric_limits<T>::digits;
  constexpr auto m_prime = to_big_int(detail::precompute_m_prime<N1>(std::integer_sequence<T, Divisor...>{}));
  // end of pre-computation

  // Perform the division
  auto t1 = skip<N1>(mul(m_prime, n));
  auto q = shift_right(
      skip<(ell - 1) / w>(add(t1, shift_right(subtract_ignore_carry(n, detail::to_length<N1>(t1)), 1))),
      (ell - 1) % w); // n >= t1
  return detail::to_length<N1>(q);
}

template <typename T, std::size_t N, T... Modulus>
CBN_ALWAYS_INLINE constexpr auto mod(big_int<N, T> n,
                                     std::integer_sequence<T, Modulus...>)
// Constant-time modulo operation with a fixed modulus
{
  auto d = div(n, std::integer_sequence<T, Modulus...>{});
  constexpr auto M = sizeof...(Modulus);
  return detail::first<M>(subtract_ignore_carry(n, partial_mul<N>(big_int<M, T>{Modulus...}, d)));
}

} // end of cbn namespace

#endif
