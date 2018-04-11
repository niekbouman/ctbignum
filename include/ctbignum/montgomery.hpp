// This file is distributed under the Apache License, Version 2.0. See the LICENSE
// file for details.
#ifndef CT_MONTGOMERY_HPP
#define CT_MONTGOMERY_HPP

#include <cstddef> // std::size_t
#include <ctbignum/addition.hpp>
#include <ctbignum/gcd.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/type_traits.hpp>
#include <limits>

namespace cbn {

template <typename T, std::size_t N1, T... Modulus,
          std::size_t N2 = sizeof...(Modulus)>
constexpr auto montgomery_reduction(big_int<N1, T> A,
                                    std::integer_sequence<T, Modulus...>) {
  // Montgomery reduction with compile-time modulus
  //
  // inputs:
  //  A       (2n limbs)  number to be reduced
  //  m       ( n limbs)  modulus
  //
  // output:
  //  T R^-1 mod m,       where R = (2^64)^n
  //

  using detail::skip;
  using detail::first;
  using detail::unary_encoding;
  using detail::pad;
  using detail::limbwise_shift_left;
  using std::integer_sequence;

  constexpr auto m = big_int<N2, T>{Modulus...};
  constexpr auto inv = mod_inv(integer_sequence<T, Modulus...>{},
                               integer_sequence<T, 0, 1>{}); // m^{-1} mod 2^64
  constexpr T mprime = -inv[0];

  auto accum = pad<1>(A);

  for (auto i = 0; i < N2; ++i) {
    auto prod = short_mul(m, accum[i] * mprime);
    auto prod2 = limbwise_shift_left<N1 + 1>(prod, i);
    accum = add_ignore_carry(accum, prod2);
  }

  auto result = skip<N2>(accum);
  auto padded_mod = pad<1>(m);
  if (result >= padded_mod)
    result = subtract_ignore_carry(result, padded_mod);

  return first<N2>(result);
}


template <typename T, std::size_t N, T... Modulus>
[[gnu::always_inline]]
constexpr auto montgomery_mul(big_int<N, T> x, big_int<N, T> y,
                              std::integer_sequence<T, Modulus...>) {
  // Montgomery multiplication with compile-time modulus

  using detail::skip;
  using detail::first;
  using detail::pad;
  using std::integer_sequence;

  using TT = typename dbl_bitlen<T>::type;

  constexpr auto m = big_int<N, T>{Modulus...};
  constexpr auto inv = mod_inv(integer_sequence<T, Modulus...>{},
                               integer_sequence<T, 0, 1>{}); // m^{-1} mod 2^64
  constexpr T mprime = -inv[0];

  big_int<N + 1, T> A{};
  for (auto i = 0; i < N; ++i) {
    T u_i = (A[0] + x[i] * y[0]) * mprime;

    // A += x[i] * y + u_i * m
    T k = 0;
    T k2 = 0;

    for (auto j = 0; j < N; ++j) {
      TT t = static_cast<TT>(y[j]) * static_cast<TT>(x[i]) + A[j] + k;
      TT t2 =
          static_cast<TT>(m[j]) * static_cast<TT>(u_i) + static_cast<T>(t) + k2;

      A[j] = t2;
      k = t >> std::numeric_limits<T>::digits;
      k2 = t2 >> std::numeric_limits<T>::digits;
    }
    A[N] += k + k2;

    // shift-right A by 1 limb while preserving A's limb-length
    A = skip<1, 1>(A);
  }

  auto padded_mod = pad<1>(m);
  if (A >= padded_mod)
    A = subtract_ignore_carry(A, padded_mod);
  return first<N>(A);
}

namespace {
// Define a template that can be used to prevent type deduction of a parameter.
template <typename T> struct Identity { typedef T type; };
template <typename T> using Identity_t = typename Identity<T>::type;
} // anonymous namespace

// Runtime-parameter variants

/// Note: the type of the last parameter is not deduced from itself, but from
/// the other parameters instead.
template <typename T, std::size_t N1, std::size_t N2>
constexpr auto montgomery_reduction(big_int<N1, T> A, big_int<N2, T> m,
                                    Identity_t<T> mprime) {
  // Montgomery reduction with runtime parameters
  //
  // inputs:
  //  A       (2n limbs)  number to be reduced
  //  m       ( n limbs)  modulus
  //  mprime  (uint64_t)  mprime = - m^{-1} mod 2^64
  //
  // output:
  //  T R^-1 mod m,       where R = (2^64)^n
  //

  using detail::skip;
  using detail::first;
  using detail::unary_encoding;
  using detail::pad;
  using detail::limbwise_shift_left;

  auto accum = pad<1>(A);

  for (auto i = 0; i < N2; ++i) {
    auto prod = short_mul(m, accum[i] * mprime);
    auto prod2 = limbwise_shift_left<N1 + 1>(prod, i);
    accum = add_ignore_carry(accum, prod2);
  }

  auto result = skip<N2>(accum);

  auto padded_mod = pad<1>(m);
  if (result >= padded_mod)
    result = subtract_ignore_carry(result, padded_mod);

  return first<N2>(result);
}

/// Note: the type of the last parameter is not deduced from itself, but from
/// the other parameters instead.
template <typename T, std::size_t N>
constexpr auto montgomery_mul(big_int<N, T> x, big_int<N, T> y, big_int<N, T> m,
                              Identity_t<T> mprime) {
  // Montgomery multiplication with runtime parameters

  using detail::skip;
  using detail::first;
  using detail::pad;

  using TT = typename dbl_bitlen<T>::type;
  big_int<N + 1, T> A{};

  for (auto i = 0; i < N; ++i) {
    T u_i = (A[0] + x[i] * y[0]) * mprime;

    // A += x[i] * y + u_i * m
    T k = 0;
    T k2 = 0;

    for (auto j = 0; j < N; ++j) {
      TT t = static_cast<TT>(y[j]) * static_cast<TT>(x[i]) + A[j] + k;
      TT t2 =
          static_cast<TT>(m[j]) * static_cast<TT>(u_i) + static_cast<T>(t) + k2;

      A[j] = t2;
      k = t >> std::numeric_limits<T>::digits;
      k2 = t2 >> std::numeric_limits<T>::digits;
    }
    A[N] += k + k2;

    // shift-right A by 1 limb while preserving A's limb-length
    A = skip<1, 1>(A);
  }

  auto padded_mod = pad<1>(m);
  if (A >= padded_mod)
    A = subtract_ignore_carry(A, padded_mod);
  return first<N>(A);
}
}
#endif

