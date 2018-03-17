#ifndef CT_RELATIONAL_HPP
#define CT_RELATIONAL_HPP

#include <algorithm>
#include <array>
#include <boost/hana/equal.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/less.hpp>
#include <cstddef>
#include <ctbignum/bigint.hpp>
#include <ctbignum/slicing.hpp>
#include <functional>
#include <numeric>
#include <sprout/array.hpp>

namespace cbn {

template <typename T, size_t N>
constexpr bool greater_than(std::array<T, N> a, std::array<T, N> b) {

  return !boost::hana::less_equal(a,b);
  //lexicographical_compare(
  //   make_reverse_iterator(end(b)), make_reverse_iterator(begin(b)),
  //    make_reverse_iterator(end(a)), make_reverse_iterator(begin(a)));
}


template <typename T, size_t N>
constexpr bool greater_than_(sprout::array<T, N> a, sprout::array<T, N> b) {
  using sprout::begin;
  using sprout::end;
  using sprout::lexicographical_compare;
  using sprout::make_reverse_iterator;

  return lexicographical_compare(
      make_reverse_iterator(end(b)), make_reverse_iterator(begin(b)),
      make_reverse_iterator(end(a)), make_reverse_iterator(begin(a)));
}

template <size_t N>
constexpr bool is_equal(std::array<uint64_t, N> a, std::array<uint64_t, N> b) {
  return boost::hana::equal(a, b);
}

template <size_t N>
constexpr bool operator==(std::array<uint64_t, N> a, std::array<uint64_t, N> b) {
  return boost::hana::equal(a, b);
}

template <typename T, size_t N1, size_t N2>
constexpr bool greater_than(big_int<N1, T> a, big_int<N2, T> b) {
  //constexpr auto L = std::max(static_cast<int>(N1), static_cast<int>(N2));
  constexpr auto L = std::max(N1, N2);

  return greater_than_(detail::pad<L - N1>(a), detail::pad<L - N2>(b));
}

// should take product of bools ( x1==0 )(x2==1)...
/*
// constant time zero test
template <typename T, std::size_t N>
constexpr bool is_zero(big_int<N, T> x) {
  return 0 == std::accumulate(x.begin(), x.end(), static_cast<T>(1),
                              std::multiplies<T>());
}

// constant time zero test
template <typename T, T... Seq>
constexpr bool is_zero(std::integer_sequence<T, Seq...>) {
  big_int<sizeof...(Seq), T> x{Seq...};
  return 0 == std::accumulate(x.begin(), x.end(), static_cast<T>(1),
                              std::multiplies<T>());
}
*/

/*
template <typename T, size_t N>
constexpr bool is_equal(sprout::array<T, N> a, sprout::array<T, N> b) {
  return sprout::equal(begin(a), end(a), begin(b), end(b));
}
*/

}
#endif
