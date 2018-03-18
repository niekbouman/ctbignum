#ifndef CT_FIELD_HPP
#define CT_FIELD_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/barrett.hpp>
#include <ctbignum/initialization.hpp>
#include <boost/hana/string.hpp>
#include <type_traits>

namespace cbn {

template <typename T, T... Modulus> struct ZqElement {


  //TODO: add cast operator  big_int
  
  big_int<sizeof...(Modulus), T> data;

  constexpr ZqElement() {}

  template <std::size_t N>
  constexpr ZqElement(big_int<N, T> init)
  // reduce init if necessary
      : data(greater_than(big_int<sizeof...(Modulus), T>{Modulus...}, init)
                 ? detail::first<sizeof...(Modulus)>(init)
                 : barrett_reduction(init,
                                     std::integer_sequence<T, Modulus...>())) {}

  template <typename String,
            typename = std::enable_if_t< // only bind to Hana strings
                boost::hana::is_a<boost::hana::string_tag, String>>>
  constexpr ZqElement(String s)
      : data(div(string_to_big_int(s),
                 big_int<sizeof...(Modulus), T>{Modulus...})
                 .second) {} // reduce s mod q, where q = Modulus
};

template <typename T, T... Modulus>  
auto Zq(std::integer_sequence<T, Modulus...>)
{
  return ZqElement<T, Modulus...>{};
}

template <typename String>
auto Zq(String s)
{
  return Zq(cbn::string_to_integer_seq(s));
}




template <typename T, T... M>
constexpr auto operator+(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  //ZqElement<T, M...> result{};
  //result.data = ;
  return ZqElement<T, M...> { mod_add(a.data, b.data, big_int<sizeof...(M), T>{M...}) };
}

template <typename T, T... M>
constexpr auto operator*(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  //ZqElement<T, M...> result{};
  //result.data = barrett_reduction(mul(a.data, b.data), std::integer_sequence<T, M...>());
  //return result;

  return ZqElement<T, M...> { barrett_reduction(mul(a.data, b.data), std::integer_sequence<T, M...>()) };
}

template <typename T, T... M>
std::ostream &operator<<(std::ostream& strm, const ZqElement<T, M...>& obj)
{
  strm << obj.data;
}


/*
template <size_t N> constexpr auto add(big_int<N, T> a, big_int<N, T> b) {
  big_int<sizeof...(Modulus), T> modulus{{Modulus...}};
  return mod_add(a, b, modulus);
  }

  template <size_t N>
  constexpr auto mul(big_int<N, T> a, big_int<N, T> b) {
    big_int<sizeof...(Modulus), T> modulus{{Modulus...}};
    return mod_add(a, b, modulus);
  }
*/








}
#endif
