#ifndef CT_MULT_HPP
#define CT_MULT_HPP

#include "relational_ops.hpp"
#include "addition.hpp"
#include <cstddef>

constexpr auto mul128(uint64_t a, uint64_t b) {
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

template <template <typename, size_t> class Array, typename T, size_t N1, size_t N2>
constexpr auto mp_accum(Array<T, N1> accum, Array<T, N2> b) {
  T carry = 0;
  Array<T, N1> r{};

  constexpr auto m = std::min(N1,N2);
  for (auto i = 0; i < m; ++i) {
    auto aa = accum[i];
    auto sum = aa + b[i];
    auto res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }
  //r[N] = carry;
  return r;
}

template <size_t accSize, template <typename, size_t> class Array, size_t N>
constexpr auto mp_mul(Array<uint64_t, N> a, Array<uint64_t, N> b) {
  Array<uint64_t, accSize> accum{};

  for (auto j = 0; j < N; ++j) {

    Array<uint64_t, 2 * N> tmp{};

    uint64_t high = 0;
    for (auto i = 0; i < N; ++i) {
      Array<uint64_t, 2> prod{{a[i] * b[j], mul128(a[i], b[j])}};
      auto sum = mp_accum(prod, Array<uint64_t, 2>{{high, 0}});

      tmp[j + i] = sum[0];
      high = sum[1];
    }
    accum = mp_accum(accum, tmp);
  }
  return accum;
}

template <size_t accSize, template <typename, size_t> class Array, size_t N>
constexpr auto mp_mul2(Array<uint64_t, N> a, Array<uint64_t, N> b) {
  Array<uint64_t, accSize> accum{};
  for (auto j = 0; j < N; ++j) {
    Array<uint64_t, 2 * N> tmp{};
    uint64_t high = 0;
    for (auto i = 0; i < N; ++i) {
      __uint128_t prodsum = a[i] * b[j] + high;
      tmp[j + i] = static_cast<uint64_t>(prodsum);
      high = prodsum >> 64;
    }
    accum = mp_accum(accum, tmp);
  }
  return accum;
}

#endif
