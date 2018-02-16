#ifndef CT_DIV_HPP
#define CT_DIV_HPP

#include <ctbignum/relational_ops.hpp>
#include <ctbignum/addition.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/bitshift.hpp>
#include <cstddef>
#include <ctbignum/print.hpp>

// Knuth's "Algorithm D" for multiprecision division as described in TAOCP Volume 2: Seminumerical Algorithms
namespace cbn {

// this uses GCC and Clang's __uint128_t data type
template <template <typename, size_t> class Array, size_t N, size_t NplusM>
constexpr auto div(Array<uint64_t, NplusM> u, Array<uint64_t, N> v) {

  constexpr size_t M = NplusM - N;

  //uint64_t d = 1;
  uint8_t k = 0;
  while (v[N - 1] < (static_cast<uint64_t>(1) << 63)) {
    //d <<= 1;
    ++k;
    //v = detail::first<N>(mul(v, Array<uint64_t, 1> {2})); // can be replaced by a bitshift operation
    v = detail::first<N>(shift_left(v, 1));
  }

  //auto ud = mul(u, Array<uint64_t, 1>{d});
  auto ud = shift_left(u, k);

  Array<uint64_t, M + 1> q{};

  for (int j = M; j >= 0; --j) {

    __uint128_t tmp = ud[j + N - 1];
    __uint128_t tmp2 = ud[j + N];
    tmp += (tmp2 << 64);

    __uint128_t qhat = tmp / v[N - 1];
    __uint128_t rhat = tmp % v[N - 1];

    auto b = static_cast<__uint128_t>(1) << 64;

    while (qhat == b || (qhat * v[N - 2] > (rhat << 64) + ud[j + N - 2])) {
      qhat -= 1;
      rhat += v[N - 1];
      if (rhat >= b)
        break;
    }

    auto tval = mp_sub_carry_out(detail::take<N+1>(ud,j, j + N + 1),
                                 mul(v, Array<uint64_t, 1>{static_cast<uint64_t>(qhat)}));

    if (tval[N]) {

      auto corrected = mp_add_ignore_last_carry(
          tval, detail::unary_encoding<N + 1, N + 2>());
      auto new_ud_part = mp_add_ignore_last_carry(corrected, detail::pad<2>(v));
      for (auto i = 0; i <= N; ++i)
        ud[j + i] = new_ud_part[i];

      --qhat;
    } else {
      for (auto i = 0; i <= N; ++i)
        ud[j + i] = tval[i];
    }
    q[j] = qhat;
  }

  return std::make_pair( q, shift_right(detail::first<N>(ud),k));
}
}
#endif
