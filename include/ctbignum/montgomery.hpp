#ifndef CT_MONTGOMERY_HPP
#define CT_MONTGOMERY_HPP

#include <cstddef> // std::size_t
#include <ctbignum/addition.hpp>
#include <ctbignum/gcd.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>

namespace cbn {

template <template <typename, std::size_t> class Array, typename T,
          std::size_t N1, T... Modulus, std::size_t N2 = sizeof...(Modulus)>
constexpr auto montgomery_reduction(Array<T, N1> A,
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

  constexpr auto m = Array<T, N2>{Modulus...};
  constexpr auto inv =
      mod_inv(integer_sequence<T, Modulus...>{},
                     integer_sequence<T, 0, 1>{}); // m^{-1} mod 2^64
  constexpr T mprime = -inv[0];

  auto accum = pad<1>(A);

  for (auto i = 0; i < N2; ++i) {
    auto prod = short_mul(m, accum[i] * mprime);
    auto prod2 = limbwise_shift_left<N1 + 1>(prod, i);
    accum = mp_add_ignore_last_carry(accum, prod2);
  }

  auto result = skip<N2>(accum);
  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, result))
    result = mp_sub(result, padded_mod);

  return first<N2>(result);
}

template <typename DT = __uint128_t,
          template <typename, std::size_t> class Array, typename T,
          std::size_t N, T... Modulus>
constexpr auto montgomery_mul(Array<T, N> x, Array<T, N> y,
                              std::integer_sequence<T, Modulus...>) {
  // Montgomery multiplication with compile-time modulus

  using detail::skip;
  using detail::first;
  using detail::pad;
  using std::integer_sequence;

  constexpr auto m = Array<T, N>{Modulus...};
  constexpr auto inv =
      mod_inv(integer_sequence<T, Modulus...>{},
                     integer_sequence<T, 0, 1>{}); // m^{-1} mod 2^64
  constexpr T mprime = -inv[0];

  Array<T, N + 1> A{};
  for (auto i = 0; i < N; ++i) {
    T u_i = (A[0] + x[i] * y[0]) * mprime;

    // A += x[i] * y + u_i * m
    T k = 0;
    T k2 = 0;

    for (auto j = 0; j < N; ++j) {
      DT t = static_cast<DT>(y[j]) * static_cast<DT>(x[i]) + A[j] + k;
      DT t2 =
          static_cast<DT>(m[j]) * static_cast<DT>(u_i) + static_cast<T>(t) + k2;

      A[j] = t2;
      k = t >> std::numeric_limits<T>::digits;
      k2 = t2 >> std::numeric_limits<T>::digits;
    }
    A[N] += k + k2;

    // shift-right A by 1 limb while preserving A's limb-length
    A = skip<1, 1>(A);
  }

  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, A))
    A = mp_sub(A, padded_mod);
  return first<N>(A);
}

// Runtime-parameter variants

template <template <typename, std::size_t> class Array, typename T,
          std::size_t N1, std::size_t N2>
constexpr auto montgomery_reduction(Array<T, N1> A, Array<T, N2> m, T mprime) {
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
    accum = mp_add_ignore_last_carry(accum, prod2);
  }

  auto result = skip<N2>(accum);

  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, result))
    result = mp_sub(result, padded_mod);

  return first<N2>(result);
}

template <typename DT = __uint128_t,
          template <typename, std::size_t> class Array, typename T,
          std::size_t N>
constexpr auto montgomery_mul(Array<T, N> x, Array<T, N> y, Array<T, N> m,
                              T mprime) {
  // Montgomery multiplication with runtime parameters

  using detail::skip;
  using detail::first;
  using detail::pad;

  Array<T, N + 1> A{};

  for (auto i = 0; i < N; ++i) {
    T u_i = (A[0] + x[i] * y[0]) * mprime;

    // A += x[i] * y + u_i * m
    T k = 0;
    T k2 = 0;

    for (auto j = 0; j < N; ++j) {
      DT t = static_cast<DT>(y[j]) * static_cast<DT>(x[i]) + A[j] + k;
      DT t2 =
          static_cast<DT>(m[j]) * static_cast<DT>(u_i) + static_cast<T>(t) + k2;

      A[j] = t2;
      k = t >> std::numeric_limits<T>::digits;
      k2 = t2 >> std::numeric_limits<T>::digits;
    }
    A[N] += k + k2;

    // shift-right A by 1 limb while preserving A's limb-length
    A = skip<1, 1>(A);
  }

  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, A))
    A = mp_sub(A, padded_mod);
  return first<N>(A);
}
}
#endif

