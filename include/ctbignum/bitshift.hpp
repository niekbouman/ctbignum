#ifndef CT_BITSHIFTS_HPP
#define CT_BITSHIFTS_HPP

#include <cstddef>
#include <cstdint>
#include <ctbignum/bigint.hpp>

namespace cbn {

template <size_t N>
constexpr auto shift_right(big_int<N, uint64_t> a, size_t k) {
  // shift-right the big integer a by k bits
  big_int<N, uint64_t> res{};

  for (auto i = 0; i < N - 1; ++i) {
    res[i] = (a[i] >> k) | (a[i + 1] << (64 - k));
  }
  res[N - 1] = (a[N - 1] >> k);
  return res;
}

template <size_t N>
constexpr auto shift_left(big_int<N, uint64_t> a, size_t k) {
  // shift-left the big integer a by k bits
  // answer has 1 limb more
  big_int<N + 1, uint64_t> res{};

  res[0] = (a[0] << k);

  for (auto i = 1; i < N; ++i) {
    res[i] = (a[i] << k) | (a[i - 1] >> (64 - k));
  }

  res[N] = a[N - 1] >> (64 - k);
  return res;
}
}
#endif
