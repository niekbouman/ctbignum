#ifndef CT_BITSHIFTS_HPP
#define CT_BITSHIFTS_HPP

#include <cstddef>
#include <cstdint>

namespace cbn {

template <template <typename, size_t> class Array, size_t N>
constexpr auto shift_right(Array<uint64_t, N> a, size_t k) {
  // shift-right the big integer a by k bits
  Array<uint64_t, N> res{};

  for (auto i = 0; i < N - 1; ++i) {
    res[i] = (a[i] >> k) | (a[i + 1] << (64 - k));
  }
  res[N - 1] = (a[N - 1] >> k);
  return res;
}

template <template <typename, size_t> class Array, size_t N>
constexpr auto shift_left(Array<uint64_t, N> a, size_t k) {
  // shift-left the big integer a by k bits
  // answer has 1 limb more
  Array<uint64_t, N + 1> res{};

  res[0] = (a[0] << k);

  for (auto i = 1; i < N; ++i) {
    res[i] = (a[i] << k) | (a[i - 1] >> (64 - k));
  }

  res[N] = a[N - 1] >> (64 - k);
  return res;
}
}
#endif
