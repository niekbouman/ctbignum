#ifndef CT_UTILITY_HPP
#define CT_UTILITY_HPP

#include <cstddef>
#include<utility>

namespace cbn {
namespace detail {

template <size_t N, template <typename, std::size_t> class Array, typename T>
constexpr auto tight_length(Array<T, N> num) {
  // count the effective number of limbs
  // (ignoring zero-limbs at the most-significant-limb side)
  size_t L = N;
  while (L > 0 && num[L - 1] == 0)
    --L;

  return L;
}


template <typename T, T... Is>
constexpr auto tight_length(std::integer_sequence<T, Is...>)

{
  // count the effective number of limbs
  // (ignoring zero-limbs at the most-significant-limb side)
  
  size_t L = sizeof...(Is);
  std::array<T, sizeof...(Is)> num { Is... };
  while (L > 0 && num[L - 1] == 0)
    --L;

  return L;
}


} // end of detail namespace
} // end of cbn namespace

#endif
