#ifndef CT_FIELD_HPP
#define CT_FIELD_HPP

#include <ctbignum/addition.hpp>
#include <ctbignum/barrett.hpp>
#include <ctbignum/initialization.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>

#include <cstddef>
//#include <boost/hana/string.hpp>
#include <type_traits>

namespace cbn {

class skip_reduction {}; 

template <typename T, T... Modulus> struct ZqElement {

  big_int<sizeof...(Modulus), T> data;

  explicit operator auto() const { return data; } // allow casting to big_int

  constexpr ZqElement() {}

  template <T... Limbs>
  constexpr ZqElement(std::integer_sequence<T, Limbs...>)
      // reduce init mod q if necessary, where q = Modulus
      : data(big_int<sizeof...(Limbs), T>{Limbs...} <
                     big_int<sizeof...(Modulus), T>{Modulus...}
                 ? detail::to_length<sizeof...(Modulus)>(
                       big_int<sizeof...(Limbs), T>{Limbs...})
                 : barrett_reduction(big_int<sizeof...(Limbs), T>{Limbs...},
                                     std::integer_sequence<T, Modulus...>())) {}

  template <std::size_t N>
  constexpr ZqElement(big_int<N, T> init)
  // reduce init mod q if necessary, where q = Modulus
      : data(init < big_int<sizeof...(Modulus), T>{Modulus...}
                 ? detail::to_length<sizeof...(Modulus)>(init)
                 : barrett_reduction(init,
                                     std::integer_sequence<T, Modulus...>())) {}

  constexpr ZqElement(big_int<sizeof...(Modulus), T> init, skip_reduction)
      : data(init) {}


  /*
  template <typename String,
            typename = std::enable_if_t< // only bind to Hana strings
                boost::hana::is_a<boost::hana::string_tag, String>>>
  constexpr ZqElement(String s)
      : data(div(string_to_big_int(s),
                 big_int<sizeof...(Modulus), T>{Modulus...})
                 .second) {} // reduce s mod q, where q = Modulus
                 */
};

template <typename T, T... Modulus>  
auto Zq(std::integer_sequence<T, Modulus...>)
{
  return ZqElement<T, Modulus...>{};
}

/*
template <size_t N, typename T, T... Modulus> 
constexpr auto Zq(big_int<N, T> modulus) {
  constexpr auto b = as_integer_seq(modulus);
  return Zq(b);
}
*/
/*
template <typename String>
auto Zq(String s)
{
  return Zq(cbn::string_to_integer_seq(s));
}
*/

template <typename T, T... M>
constexpr auto operator+(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  return ZqElement<T, M...> { mod_add(a.data, b.data, big_int<sizeof...(M), T>{M...}), skip_reduction{} };
}

template <typename T, T... M>
constexpr auto operator*(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  return ZqElement<T, M...> { barrett_reduction(mul(a.data, b.data), std::integer_sequence<T, M...>()), skip_reduction{} };
}

template <typename T, T... M>
std::ostream &operator<<(std::ostream& strm, const ZqElement<T, M...>& obj)
{
  strm << obj.data;
}


}
#endif
