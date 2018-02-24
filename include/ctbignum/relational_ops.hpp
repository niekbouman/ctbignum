#ifndef CT_RELATIONAL_HPP
#define CT_RELATIONAL_HPP

#include <cstddef>
#include <sprout/array.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/equal.hpp>
#include <array>

namespace cbn {

template <typename T, size_t N>
constexpr bool greater_than(std::array<T, N> a, std::array<T, N> b) {

  return !boost::hana::less_equal(a,b);
  //lexicographical_compare(
  //   make_reverse_iterator(end(b)), make_reverse_iterator(begin(b)),
  //    make_reverse_iterator(end(a)), make_reverse_iterator(begin(a)));
}


template <typename T, size_t N>
constexpr bool greater_than(sprout::array<T, N> a, sprout::array<T, N> b) {
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

/*
template <typename T, size_t N>
constexpr bool is_equal(sprout::array<T, N> a, sprout::array<T, N> b) {
  return sprout::equal(begin(a), end(a), begin(b), end(b));
}
*/

}
#endif
