#ifndef CT_STRINGINIT_HPP
#define CT_STRINGINIT_HPP

#include <boost/hana.hpp>
#include <cstddef>
#include <ctbignum/mult.hpp>
#include <ctbignum/utility.hpp>

namespace cbn {

template <int N, template <typename, size_t> class Array, typename T>
constexpr auto limb_int(unsigned long v) {
  Array<T, N> r{};
  r[0] = v;
  return r;
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
                boost::hana::first(state), partial_mul<N>( limb_int<1, Array, T>(i) , boost::hana::second(state) )),
            partial_mul<N>(limb_int<1, Array, T>(10),boost::hana::second(state)));

           //     mul<-static_cast<int>(N)>(limb_int<N, Array, T>(i),
           //                               boost::hana::second(state))),
           // mul<-static_cast<int>(N)>(limb_int<N, Array, T>(10),
            //                          boost::hana::second(state)));
      }));

  constexpr auto L = (ExplicitLength != 0) ? ExplicitLength : detail::tight_length(num);
  return detail::first<L>(num);
}

namespace detail {
template <typename String>
constexpr std::size_t get_ith_limb(String s, const std::size_t i) {
  return string_to_big_int(s)[i];
}

template <typename T, typename String, std::size_t... Is>
constexpr auto string_to_index_seq_impl(String s, std::index_sequence<Is...>) {
  return std::integer_sequence<T,get_ith_limb(s, Is)...>{};
}
} // end of detail namespace

template <typename T=uint64_t, typename String>
constexpr auto string_to_index_seq(String s) {
  const size_t N = cbn::string_to_big_int(s).size();
  return detail::string_to_index_seq_impl<T>(s, std::make_index_sequence<N>{});
}

} // end of cbn namespace

#endif
