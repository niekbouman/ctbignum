#ifndef CT_MULT_HPP
#define CT_MULT_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/type_traits.hpp>
#include <limits>

namespace cbn {

template <typename T, std::size_t N>
constexpr auto short_mul(big_int<N, T> a, T b) {

  using TT = typename dbl_bitlen<T>::type;
  big_int<N + 1, T> p{};
  T k = 0;
  for (auto j = 0; j < N; ++j) {
    TT t = static_cast<TT>(a[j]) * static_cast<TT>(b) + k;
    p[j] = t;
    k = t >> std::numeric_limits<T>::digits;
  }
  p[N] = k;
  return p;
}

template <size_t padding_limbs = 0, size_t M, size_t N, typename T>
constexpr auto mul(big_int<M, T> u, big_int<N, T> v) {

  using TT = typename dbl_bitlen<T>::type;
  big_int<M + N + padding_limbs, T> w{};
  for (auto j = 0; j < N; ++j) {
    // if (v[j] == 0)
    //  w[j + M] = static_cast<uint64_t>(0);
    // else {
    T k = 0;
    for (auto i = 0; i < M; ++i) {
      __uint128_t t =
          static_cast<TT>(u[i]) * static_cast<TT>(v[j]) + w[i + j] + k;
      w[i + j] = static_cast<T>(t);
      k = t >> std::numeric_limits<T>::digits;
    }
    w[j + M] = k;
    //}
  }
  return w;
}

template <size_t ResultLength, size_t M, size_t N, typename T>
constexpr auto partial_mul(big_int<M, T> u, big_int<N, T> v) {

  using TT = typename dbl_bitlen<T>::type;
  big_int<ResultLength, T> w{};
  for (auto j = 0; j < N; ++j) {
    // if (v[j] == 0) {
    //  if (j + M < ResultLength)
    //    w[j + M] = static_cast<T>(0);
    //} else {
    T k = 0;
    const auto m = std::min(M, ResultLength - j);
    for (auto i = 0; i < m; ++i) {
      TT t = static_cast<TT>(u[i]) * static_cast<TT>(v[j]) + w[i + j] + k;
      w[i + j] = static_cast<T>(t);
      k = t >> std::numeric_limits<T>::digits;
    }
    if (j + M < ResultLength)
      w[j + M] = k;
    //}
  }
  return w;
}

// for use with mul2 function (see below)
constexpr auto mul128(uint64_t a, uint64_t b) {
  // code for this function is based on:
  // https://stackoverflow.com/questions/28868367/getting-the-high-part-of-64-bit-integer-multiplication
  uint64_t a_lo = static_cast<uint32_t>(a);
  uint64_t a_hi = a >> 32;
  uint64_t b_lo = static_cast<uint32_t>(b);
  uint64_t b_hi = b >> 32;

  uint64_t a_x_b_hi = a_hi * b_hi;
  uint64_t a_x_b_mid = a_hi * b_lo;
  uint64_t b_x_a_mid = b_hi * a_lo;
  uint64_t a_x_b_lo = a_lo * b_lo;

  uint64_t carry_bit =
      (static_cast<uint64_t>(static_cast<uint32_t>(a_x_b_mid)) +
       static_cast<uint64_t>(static_cast<uint32_t>(b_x_a_mid)) +
       (a_x_b_lo >> 32)) >>
      32;

  uint64_t multhi =
      a_x_b_hi + (a_x_b_mid >> 32) + (b_x_a_mid >> 32) + carry_bit;

  return multhi;
}

// multiplication function on different input lengths that uses mul128 function
// this function is approximately two times slower than the mul function that
// uses __uint128
template <int padding_limbs = 0, size_t N1, size_t N2>
constexpr auto mul2(big_int<N1, uint64_t> a, big_int<N2, uint64_t> b) {
  big_int<N1 + N2 + padding_limbs, uint64_t> accum{};
  for (auto j = 0; j < N2; ++j) {
    big_int<N1 + N2, uint64_t> tmp{};
    uint64_t high = 0;
    for (auto i = 0; i < N1; ++i) {
      big_int<2, uint64_t> prod{{a[i] * b[j], mul128(a[i], b[j])}};
      auto sum = accumulate(prod, big_int<2, uint64_t>{{high, 0}});
      tmp[j + i] = sum[0];
      high = sum[1];
    }
    // should we here also write the remaining high word to the proper location?
    accum = accumulate(accum, tmp);
  }
  return accum;
}
}
#endif
