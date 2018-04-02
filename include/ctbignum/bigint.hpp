#ifndef CT_BIGINT_HPP
#define CT_BIGINT_HPP

#include <array>

namespace cbn {

// TODO ensure using SFINAE that T is an integral type

template <size_t N, typename T = uint64_t> struct big_int {
  std::array<T, N> repr;

  constexpr T &operator[](size_t i) { return repr[i]; }
  constexpr const T &operator[](size_t i) const { return repr[i]; }
  constexpr size_t size() const noexcept { return repr.size(); }
};
}

#endif
