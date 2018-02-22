#ifndef CT_DIV_HPP
#define CT_DIV_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/bitshift.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/utility.hpp>
#include <ctbignum/barrett.hpp>
#include <ctbignum/print.hpp>
#include <tuple>

//#include <experimental/array>
//#include <sprout/array.hpp>

namespace cbn {

template <template <typename, size_t> class Array, size_t M>
constexpr auto short_div(Array<uint64_t, M> u, uint64_t v) {

  __uint128_t r = 0;
  Array<uint64_t, M> q{};

  for (int i = M - 1; i >= 0; --i) {
    __uint128_t w = (r << 64) + u[i];
    q[i] = w / v;
    r = w % v;
  }
  return std::make_pair(q, Array<uint64_t, 1>{static_cast<uint64_t>(r)});
}

//


// this uses GCC and Clang's __uint128_t data type
template <template <typename, size_t> class Array, size_t NN, size_t NplusM>
constexpr auto divtight(Array<uint64_t, NplusM> u, Array<uint64_t, NN> v) {
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

  size_t N = NN;
  while (N > 0 && v[N - 1] == 0)
    --N;


  
  //static_assert(N>1,  "divisor too short: use short division");
    size_t M = NplusM - N;

    uint8_t k = 0;
    while (v[N - 1] < (static_cast<uint64_t>(1) << 63)) {
      ++k;
      v = detail::first<NN>(shift_left(v, 1));
      //v = detail::first<N>(shift_left(v, 1));
    }

    auto us = shift_left(u, k);
    Array<uint64_t, NplusM> q{};

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
          detail::take<NN + 1>(us, j, j + N + 1),
          mul(v, Array<uint64_t, 1>{static_cast<uint64_t>(qhat)}));

      if (true_value[N]) {
        auto corrected = mp_add_ignore_last_carry(
            true_value, detail::unary_encoding<NN + 2,Array>(N+1));
            //true_value, detail::unary_encoding<N + 1, NN + 2>());
        auto new_us_part =
            mp_add_ignore_last_carry(corrected, detail::pad<2>(v));
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

  static_assert(N>1,  "divisor too short: use short division");
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
        auto new_us_part =
            mp_add_ignore_last_carry(corrected, detail::pad<2>(v));
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

template <size_t L, template <typename, size_t> class Array, size_t N,
          size_t NplusM>
constexpr auto meta_div(Array<uint64_t, NplusM> u, Array<uint64_t, N> v) {

  if constexpr (L == 1) 
    return short_div(u, v[0]);
  else
    //return div(u, v);
    return divtight(u, v);
}


}
#endif


