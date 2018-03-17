#ifndef CT_DIVISION_HPP
#define CT_DIVISION_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/bitshift.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/type_traits.hpp>
#include <ctbignum/utility.hpp>
#include <limits>

namespace cbn {

template <size_t M, typename T> constexpr auto short_div(big_int<M, T> u, T v) {

  using TT = typename dbl_bitlen<T>::type;
  TT r{0};
  big_int<M, T> q{};

  for (int i = M - 1; i >= 0; --i) {
    TT w = (r << std::numeric_limits<T>::digits) + u[i];
    q[i] = w / v;
    r = w % v;
  }
  return std::make_pair(q, big_int<1, T>{{static_cast<T>(r)}});
}

namespace detail {

// this uses GCC and Clang's __uint128_t data type
template <size_t NN, size_t NplusM, typename T>
constexpr auto knuth_div(big_int<NplusM, T> u, big_int<NN, T> v) {
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

  using TT = typename dbl_bitlen<T>::type;
  size_t N = NN;
  while (N > 0 && v[N - 1] == 0)
    --N;

  // static_assert(N>1,  "divisor too short: use short division");
  size_t M = NplusM - N;

  uint8_t k = 0;
  while (v[N - 1] < (static_cast<T>(1) << (std::numeric_limits<T>::digits - 1))) {
    ++k;
    v = detail::first<NN>(shift_left(v, 1));
    // v = detail::first<N>(shift_left(v, 1));
  }

  auto us = shift_left(u, k);
  big_int<NplusM, T> q{};

  for (int j = M; j >= 0; --j) {

    TT tmp = us[j + N - 1];
    TT tmp2 = us[j + N];
    tmp += (tmp2 << 64);

    TT qhat = tmp / v[N - 1];
    TT rhat = tmp % v[N - 1];

    auto b = static_cast<TT>(1) << std::numeric_limits<T>::digits;
    while (qhat == b || (qhat * v[N - 2] > (rhat << std::numeric_limits<T>::digits) + us[j + N - 2])) {
      qhat -= 1;
      rhat += v[N - 1];
      if (rhat >= b)
        break;
    }

    auto true_value = subtract(
        detail::take<NN + 1>(us, j, j + N + 1),
        mul(v, big_int<1, T>{{static_cast<T>(qhat)}}));

    if (true_value[N]) {
      auto corrected = add_ignore_carry(
          true_value, detail::unary_encoding<NN + 2>(N + 1));
      // true_value, detail::unary_encoding<N + 1, NN + 2>());
      auto new_us_part = add_ignore_carry(corrected, detail::pad<2>(v));
      for (auto i = 0; i <= N; ++i)
        us[j + i] = new_us_part[i];
      --qhat;
    } else {
      for (auto i = 0; i <= N; ++i)
        us[j + i] = true_value[i];
    }
    q[j] = qhat;
  }

  return std::make_pair(q, shift_right(detail::first<NN>(us), k));
}
}

template <size_t L = 0, size_t N, size_t NplusM, typename T>
constexpr auto div(big_int<NplusM, T> u, big_int<N, T> v) {

  if
    constexpr(L == 0) // L is left unspecified, hence we assume that v is
                      // tightly represented (no zero limbs at msb side)
    {
      return div<N>(u, v);
    }
  else if
    constexpr(L == 1) { return short_div(u, v[0]); }
  else {
    return detail::knuth_div(u, v);
  }
}

template <size_t N, size_t NplusM, typename T>
constexpr auto div_v(big_int<NplusM, T> u, big_int<N, T> v) {
  // division where the msb of v varies at runtime  

  auto L = detail::tight_length(v);
  if (L == 1) {
    auto qr = short_div(u, v[0]);
    return std::make_pair(qr.first, detail::pad<N-1>(qr.second)); // make same return type as in the 'else'-branch
  } else {
    return detail::knuth_div(u, v);
  }
}
}
#endif

/*


// this uses GCC and Clang's __uint128_t data type
template <size_t N, size_t NplusM>
constexpr auto div(big_int< NplusM, uint64_t> u, big_int< N, uint64_t> v) {
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

  static_assert(N>1,  "divisor too short: use short division");
    constexpr size_t M = NplusM - N;

    uint8_t k = 0;
    while (v[N - 1] < (static_cast<uint64_t>(1) << 63)) {
      ++k;
      v = detail::first<N>(shift_left(v, 1));
    }

    auto us = shift_left(u, k);
    big_int< M + 1, uint64_t> q{};

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

      auto true_value = subtract(
          detail::take<N + 1>(us, j, j + N + 1),
          mul(v, big_int< 1, uint64_t>{static_cast<uint64_t>(qhat)}));

      if (true_value[N]) {
        auto corrected = add_ignore_carry(
            true_value, detail::unary_encoding<N + 1, N + 2>());
        auto new_us_part =
            add_ignore_carry(corrected, detail::pad<2>(v));
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

template <size_t L, size_t N,
          size_t NplusM>
constexpr auto meta_div(big_int< NplusM, uint64_t> u, big_int< N, uint64_t> v) {

  if constexpr (L == 1)
  {
    return short_div(u, v[0]);
  }
  else
  {
    //return div(u, v);
    return divtight(u, v);
  }
}

*/

