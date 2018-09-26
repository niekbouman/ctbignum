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
#ifndef CT_BARRETT_HPP
#define CT_BARRETT_HPP

#include <algorithm>
#include <cstddef> // std::size_t
#include <cmath>

#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>

namespace cbn {

namespace detail {
template <typename T, T... Modulus> constexpr auto precompute_mu() {
  big_int<sizeof...(Modulus), T> modulus = {Modulus...};
  const std::size_t twoN = 2 * sizeof...(Modulus);
  auto quot_rem = div(detail::unary_encoding<twoN, twoN + 1>(), modulus);
  return quot_rem.quotient;
}
} // end namespace detail

template <typename T, std::size_t N1, T... Modulus>
constexpr auto barrett_reduction(big_int<N1, T> x,
                                 std::integer_sequence<T, Modulus...>) {

  // Barrett reduction, modulus as a template parameter.

  using detail::skip;
  using detail::first;
  using detail::unary_encoding;
  using detail::pad;

  auto mu = detail::precompute_mu<T, Modulus...>();
  const std::size_t N2 = sizeof...(Modulus);

  constexpr auto L = std::max(static_cast<int>(0), static_cast<int>(N2)+1-static_cast<int>(N1)); 
  auto y = pad<L>(x);

  big_int<N2, T> modulus = {Modulus...};

  auto q2approx = mul(skip<N2 - 1>(y),   //
                      skip<N2 - 2>(mu)); // approximation as suggested in Ch.14
                                         // of "Handbook of Applied Cryptography",
                                         // by Menezes and van Oorschot
  auto q3 = skip<3>(q2approx);           //

  //auto q3 = skip<N2+1>(mul(skip<N2 - 1>(x),mu)); // this would be the exact version

  auto r1 = first<N2 + 1>(y);
  auto r2 = partial_mul<N2 + 1>(q3, modulus);
  auto r_with_carry = subtract(r1, r2);
  auto r = first<N2 + 1>(r_with_carry);

  if (r_with_carry[N2 + 1])
    r = add_ignore_carry(r, unary_encoding<N2, N2 + 1>());

  auto padded_mod = pad<1>(modulus);
  if (r >= padded_mod)
    r = subtract_ignore_carry(r, padded_mod);
  if (r >= padded_mod)
    r = subtract_ignore_carry(r, padded_mod);

  return first<N2>(r);
}

// specialization for length one
template <typename T, T Modulus>
constexpr auto barrett_reduction(big_int<1, T> x, std::integer_sequence<T, Modulus>) 
{
  return big_int<1,T>{ x[0] % Modulus }; 
}  

  
template <typename T, std::size_t N1, std::size_t N2, std::size_t N3>
constexpr auto barrett_reduction(big_int<N1, T> x, big_int<N2, T> modulus,
                                 big_int<N3, T> mu) {

  // Barrett reduction, when given modulus and precomputed value mu that depends
  // on modulus as ordinary parameters.

  using detail::skip;
  using detail::first;
  using detail::unary_encoding;
  using detail::pad;

  auto q2approx = mul(skip<N2 - 1>(x), skip<N2 - 2>(mu));
  auto q3 = skip<3>(q2approx);

  auto r1 = first<N2 + 1>(x);
  auto r2 = partial_mul<N2 + 1>(q3, modulus);
  auto r_with_carry = subtract(r1, r2);
  auto r = first<N2 + 1>(r_with_carry);

  if (r_with_carry[N2 + 1])
    r = add_ignore_carry(r, unary_encoding<N2, N2 + 1>());

  auto padded_mod = pad<1>(modulus);
  if (r >= padded_mod)
    r = subtract_ignore_carry(r, padded_mod);
  if (r >= padded_mod)
    r = subtract_ignore_carry(r, padded_mod);

  return first<N2>(r);
}
}
#endif
