#ifndef CT_DIV_HPP
#define CT_DIV_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/bitshift.hpp>
#include <ctbignum/slicing.hpp>

namespace cbn {

// this uses GCC and Clang's __uint128_t data type
template <template <typename, size_t> class Array, size_t N, size_t NplusM>
constexpr auto div(Array<uint64_t, NplusM> u, Array<uint64_t, N> v) {
  // Knuth's "Algorithm D" for multiprecision division as described in TAOCP
  // Volume 2: Seminumerical Algorithms
  //
  // input:
  // u  big_int<N+M>
  // v  big_int<N>
  //
  // computes:
  // quotient = floor[ u/v ]    
  // rem = u % v                
  //
  // returns:
  // std::pair<big_int<M+1>, big_int<N>>(quotient, rem)

  constexpr size_t M = NplusM - N;

  uint8_t k = 0;
  while (v[N - 1] < (static_cast<uint64_t>(1) << 63)) {
    ++k;
    v = detail::first<N>(shift_left(v, 1));
  }

  auto us = shift_left(u, k);
  Array<uint64_t, M + 1> q{};

  for (int j = M; j >= 0; --j) {

    __uint128_t tmp = us[j + N - 1];
    __uint128_t tmp2 = us[j + N];
    tmp += (tmp2 << 64);

    __uint128_t qhat = tmp / v[N - 1];
    __uint128_t rhat = tmp % v[N - 1];

    auto b = static_cast<__uint128_t>(1) << 64;
    while (qhat == b || (qhat * v[N - 2] > (rhat << 64) + us[j + N - 2])) {
      qhat -= 1;
      rhat += v[N - 1];
      if (rhat >= b)
        break;
    }

    auto true_value = mp_sub_carry_out(
        detail::take<N + 1>(us, j, j + N + 1),
        mul(v, Array<uint64_t, 1>{static_cast<uint64_t>(qhat)}));

    if (true_value[N]) {
      auto corrected = mp_add_ignore_last_carry(
          true_value, detail::unary_encoding<N + 1, N + 2>());
      auto new_us_part = mp_add_ignore_last_carry(corrected, detail::pad<2>(v));
      for (auto i = 0; i <= N; ++i)
        us[j + i] = new_us_part[i];
      --qhat;
    } else {
      for (auto i = 0; i <= N; ++i)
        us[j + i] = true_value[i];
    }
    q[j] = qhat;
  }

  return std::make_pair(q, shift_right(detail::first<N>(us), k));
}

template <template <typename, size_t> class Array, size_t N>
constexpr auto ext_gcd_imp(Array<uint64_t, N> a, Array<uint64_t, N> b) {

  // todo make padding so that outputs have same length in both branches
  if (a == Array<uint64_t, N>{})
    return std::make_tuple(b, 0, 1);
  else {

    auto qr = div(b, a);

    auto tup = egcd(qr.second, a);

    auto g = std::get<0>(tup);
    auto y = std::get<1>(tup);
    auto x = std::get<2>(tup);
    return std::make_tuple(g, mp_sub(x, mul(qr.first, y)), y);
  }
}
}

template <template <typename, size_t> class Array, size_t N1, size_t N2>
constexpr auto ext_gcd(Array<uint64_t, N1> a, Array<uint64_t, N2> b) {

  if (a == Array<uint64_t, N1>{})
    return std::make_tuple(b, 0, 1);
  else {

    auto qr = div(b, a);

    auto tup = egcd(qr.second, a);

    auto g = std::get<0>(tup);
    auto y = std::get<1>(tup);
    auto x = std::get<2>(tup);
    return std::make_tuple(g, mp_sub(x, mul(qr.first, y)), y);
  }
}
}

#endif
