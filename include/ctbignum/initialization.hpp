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

template <size_t N, template <typename, size_t> class Array, typename T>
constexpr auto tight_length(Array<T, N> num) {
  // count the effective number of limbs 
  // (ignoring zero-limbs at the most-significant-limb side)
  int L = N;
  while (L > 0 && num[L - 1] == 0)
    --L;

  return L;
}

// modified from Louis Dionne's string-to-num example
template <size_t ExplicitLength = 0 /* optional */,
          template <typename, size_t> class Array = sprout::array,
          typename T = uint64_t, typename String>
constexpr auto string_to_big_int(String str) {
  // Convert a BOOST_HANA_STRING to a big num with automatic deduction of the
  // number of required limbs.
  //
  // Automatic length deduction can be overridden by specifying the number of
  // limbs explicitly by passing it as (the only) template parameter

  constexpr int len = decltype(boost::hana::length(str))::value;
  constexpr size_t N =
      (ExplicitLength != 0) ? ExplicitLength : 1 + (10 * len) / 192;
  // upper bound on how many limbs we need based on string len
  // log(10)/log(2^64) < 10/192

  constexpr auto num = boost::hana::first(boost::hana::fold_right(
      str, boost::hana::make_pair(limb_int<N, Array, T>(0),
                                  limb_int<N, Array, T>(1)),
      [](auto c, auto state) {
        constexpr int i =
            boost::hana::value(c) - 48; // convert character to decimal
        return boost::hana::make_pair(
            mp_add_ignore_last_carry(
                boost::hana::first(state),
                mul<-static_cast<int>(N)>(limb_int<N, Array, T>(i),
                                          boost::hana::second(state))),
            mul<-static_cast<int>(N)>(limb_int<N, Array, T>(10),
                                      boost::hana::second(state)));
      }));

  constexpr auto L = (ExplicitLength != 0) ? ExplicitLength : tight_length(num);
  return detail::first<L>(num);
}

} // namespace

#endif
