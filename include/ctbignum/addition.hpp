// This file is distributed under the Apache License, Version 2.0. See the LICENSE
// file for details.

#ifndef CT_ADDITION_HPP
#define CT_ADDITION_HPP

#include <ctbignum/bigint.hpp>
#include <ctbignum/relational_ops.hpp>

#include <algorithm>
#include <cstddef>

namespace cbn {

template <typename T, size_t N>
constexpr auto add(big_int<N, T> a, big_int<N, T> b) {
  T carry{};
  big_int<N + 1, T> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto sum = aa + b[i];
    auto res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }

  r[N] = carry;
  return r;
}

template <typename T, size_t N>
constexpr auto subtract(big_int<N, T> a, big_int<N, T> b) {
  T carry{};
  big_int<N + 1, T> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto diff = aa - b[i];
    auto res = diff - carry;
    carry = (diff > aa) | (res > diff);
    r[i] = res;
  }
  r[N] = carry;

  return r;
}

template <typename T, size_t N>
constexpr auto add_ignore_carry(big_int<N, T> a, big_int<N, T> b) {
  T carry{};
  big_int<N, T> r{};

  for (auto i = 0; i < N; ++i) {
    T aa = a[i];
    T sum = aa + b[i];
    T res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }

  return r;
}

template <typename T, size_t N>
constexpr auto subtract_ignore_carry(big_int<N, T> a, big_int<N, T> b) {
  T carry{};
  big_int<N, T> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto diff = aa - b[i];
    auto res = diff - carry;
    carry = (diff > aa) | (res > diff);
    r[i] = res;
  }

  return r;
}

template <typename T, size_t N1, size_t N2>
constexpr auto accumulate(big_int<N1, T> accum, big_int<N2, T> b) {
  T carry{};
  big_int<N1, T> r{};

  auto m = std::min(N1, N2);
  for (auto i = 0; i < m; ++i) {
    auto aa = accum[i];
    auto sum = aa + b[i];
    auto res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }
  if (N1 > N2)
    r[N2] = carry;
  return r;
}

template <typename T, size_t N>
constexpr auto mod_add(big_int<N, T> a, big_int<N, T> b,
                       big_int<N, T> modulus) {
  T carry{};
  big_int<N, T> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto sum = aa + b[i];
    auto res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }

  if (carry + (r >= modulus) != 0)
    r = subtract_ignore_carry(r, modulus);

  return r;
}

template <typename T, size_t N, T... Modulus>
constexpr auto mod_add(big_int<N, T> a, big_int<N, T> b, std::integer_sequence<T, Modulus...>) {
  big_int<sizeof...(Modulus), T> modulus{{Modulus...}};
  return mod_add(a, b, modulus);
}


} // end namespace cbn

#endif
