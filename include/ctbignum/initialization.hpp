#ifndef CT_STRINGINIT_HPP
#define CT_STRINGINIT_HPP

#include <ctbignum/mult.hpp>
#include <boost/hana.hpp>
#include <cstddef>

namespace cbn {

template <int N, template<typename,size_t> class Array, typename T>
constexpr auto limb_int(unsigned long v) {
  Array<T, N> r{};
  r[0] = v;
  return r;
}

// modified from Louis Dionne's string-to-num example
template <size_t N, template <typename, size_t> class Array = sprout::array,
          typename T = uint64_t, typename String>
constexpr auto string_to_big_int(String str) {
  return boost::hana::first(boost::hana::fold_right(
      str, boost::hana::make_pair(limb_int<N,Array,T>(0), limb_int<N,Array,T>(1)),
      [](auto c, auto state) {
        constexpr int i = boost::hana::value(c) - 48; // convert character to decimal
        return boost::hana::make_pair(
            mp_add_ignore_last_carry(boost::hana::first(state),
                                     mul<-static_cast<int>(N)>(limb_int<N,Array,T>(i),
                                               boost::hana::second(state))),
            mul<-static_cast<int>(N)>(limb_int<N, Array, T>(10), boost::hana::second(state)));
      }));
}

} // namespace

#endif
