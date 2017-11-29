#ifndef CT_SLICING_HPP
#define CT_SLICING_HPP

#include <cstddef>
#include <sprout/array.hpp>

template <size_t Begin, size_t End, size_t Padding=0, template <typename, size_t> class Array,
          typename T, size_t N1>
constexpr auto take(Array<T, N1> t) {
  static_assert(End >= Begin, "invalid range");
  static_assert(End - Begin <= N1, "invalid range");

  Array<T, End - Begin + Padding> res{};
  for (auto i = Begin; i < End; ++i) {
    res[i-Begin] = t[i];
  }

  return res;
}

template <size_t N, template <typename, size_t> class Array, typename T,
          size_t N1>
constexpr auto skip(Array<T, N1> t) {
  // skip first N limbs
  return take<N, N1>(t);
}

template <size_t N, template <typename, size_t> class Array, typename T,
          size_t N1>
constexpr auto first(Array<T, N1> t) {
  // take first N limbs
  return take<0, N>(t);
}

template <size_t N, template <typename, size_t> class Array, typename T,
          size_t N1>
constexpr auto pad(Array<T, N1> t) {
  // take first N limbs
  return take<0, N1, N>(t);
}

template <size_t K, size_t N,
          template <typename, size_t> class Array = sprout::array,
          typename T = uint64_t>
constexpr auto unary_encoding() {
  Array<T, N> res{};
  res[K] = 1;
  return res;
}

#endif
