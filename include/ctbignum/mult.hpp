#ifndef CT_MULT_HPP
#define CT_MULT_HPP

#include <ctbignum/relational_ops.hpp>
#include <ctbignum/addition.hpp>
#include <ctbignum/slicing.hpp>
#include <cstddef>

namespace cbn {

template <size_t padding_limbs = 0, template <typename, size_t> class Array,
          size_t M, size_t N>
constexpr auto mul(Array<uint64_t, M> u, Array<uint64_t, N> v) {
  Array<uint64_t, M + N + padding_limbs> w{};

  for (auto j = 0; j < N; ++j) {
    if (v[j] == 0)
      w[j + M] = static_cast<uint64_t>(0);
    else {
      uint64_t k = 0;
      for (auto i = 0; i < M; ++i) {
        __uint128_t t =
            static_cast<__uint128_t>(u[i]) * static_cast<__uint128_t>(v[j]) +
            w[i + j] + k;
        w[i + j] = static_cast<uint64_t>(t);
        k = t >> 64;
      }
      w[j + M] = k;
    }
  }
  return w;
}

template <size_t ResultLength, template <typename, size_t> class Array,
          size_t M, size_t N>
constexpr auto partial_mul(Array<uint64_t, M> u, Array<uint64_t, N> v) {
  Array<uint64_t, ResultLength> w{};

  for (auto j = 0; j < N; ++j) {
    if (v[j] == 0) {
      if (j + M < ResultLength)
        w[j + M] = static_cast<uint64_t>(0);
    } else {
      uint64_t k = 0;
      const auto m = std::min(M, ResultLength - j);
      for (auto i = 0; i < m; ++i) {
        __uint128_t t =
            static_cast<__uint128_t>(u[i]) * static_cast<__uint128_t>(v[j]) +
            w[i + j] + k;
        w[i + j] = static_cast<uint64_t>(t);
        k = t >> 64;
      }
      if (j + M < ResultLength)
        w[j + M] = k;
    }
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
// this function is approximately two times slower than the mul function that uses __uint128
template <int padding_limbs = 0, template <typename, size_t> class Array, size_t N1, size_t N2>
constexpr auto mul2(Array<uint64_t, N1> a, Array<uint64_t, N2> b) {
  Array<uint64_t, N1 + N2 + padding_limbs> accum{};
  for (auto j = 0; j < N2; ++j) {
    Array<uint64_t, N1 + N2> tmp{};
    uint64_t high = 0;
    for (auto i = 0; i < N1; ++i) {
      Array<uint64_t, 2> prod{{a[i] * b[j], mul128(a[i], b[j])}};
      auto sum = accumulate(prod, Array<uint64_t, 2>{{high, 0}});
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
