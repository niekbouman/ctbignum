//
// This file is part of
//
// CTBignum 	
//
// C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
// 
//
// This file is distributed under the Apache License, Version 2.0. See the LICENSE
// file for details.
#ifndef CT_FIELD_HPP
#define CT_FIELD_HPP

#include <ctbignum/addition.hpp>
#include <ctbignum/invariant_div.hpp>
#include <ctbignum/io.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/mod_inv.hpp>
#include <ctbignum/slicing.hpp>

#include <cstddef>
#include <type_traits>

namespace cbn {

class skip_reduction {};

template <typename T, T... Modulus> struct ZqElement {
  using value_type = T;

  big_int<sizeof...(Modulus), T> data;
  explicit operator auto() const { return data; } // allow casting to big_int
  constexpr ZqElement() : data() {}

  //template <typename U, typename = std::enable_if<std::is_integral_v<U> && std::is_unsigned_v<U>>>
  //explicit constexpr ZqElement(U x) : data{static_cast<T>(x)} {}

  //template <typename U, typename = std::enable_if<std::is_integral_v<U> && std::is_signed_v<U>>>
  //explicit constexpr ZqElement(U x) {
  //explicit constexpr ZqElement(long x) {
  constexpr ZqElement(long x) {

    auto neg = subtract_ignore_carry( to_big_int(std::integer_sequence<T,Modulus...>{}) , big_int<sizeof...(Modulus),T>{static_cast<T>(-x)});
    auto pos = big_int<sizeof...(Modulus),T>{static_cast<T>(x)};

    data = mod((x >= 0) ? pos : neg, std::integer_sequence<T, Modulus...>());
  }

  //constexpr ZqElement(int x) : data{ static_cast<T>(x) } {}

  template <T... Limbs>
  constexpr ZqElement(std::integer_sequence<T, Limbs...>)
      // reduce init mod q if necessary, where q = Modulus
      : data(big_int<sizeof...(Limbs), T>{Limbs...} <
                     big_int<sizeof...(Modulus), T>{Modulus...}
                 ? detail::to_length<sizeof...(Modulus)>(
                       big_int<sizeof...(Limbs), T>{Limbs...})
                 : mod(big_int<sizeof...(Limbs), T>{Limbs...},
                       std::integer_sequence<T, Modulus...>())) {}

  template <std::size_t N>
  constexpr ZqElement(big_int<N, T> init)
      // reduce init mod q if necessary, where q = Modulus
      : data(init < big_int<sizeof...(Modulus), T>{Modulus...}
                 ? detail::to_length<sizeof...(Modulus)>(init)
                 : mod(init, std::integer_sequence<T, Modulus...>())) {}

  constexpr ZqElement(big_int<sizeof...(Modulus), T> init, skip_reduction)
      : data(init) {}
};

template <typename T, T... Modulus>  
auto Zq(std::integer_sequence<T, Modulus...>)
{
  return ZqElement<T, Modulus...>{};
}

template <typename T, T... Modulus>
constexpr auto extract_modulus(ZqElement<T, Modulus...> a) {
  return std::integer_sequence<T, Modulus...>{};
}

template <typename T, T... M>
constexpr auto& operator+=(ZqElement<T, M...>& a, ZqElement<T, M...> b) {
  a = ZqElement<T, M...> { mod_add(a.data, b.data, big_int<sizeof...(M), T>{M...}), skip_reduction{} };
  return a;
}

template <typename T, T... M>
constexpr auto operator+(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  a += b;
  return a;
}

template <typename T, T... M>
constexpr auto& operator-=(ZqElement<T, M...>& a, ZqElement<T, M...> b) {
  a = ZqElement<T, M...>{ mod_sub(a.data, b.data, big_int<sizeof...(M), T>{M...}), skip_reduction{} };
  return a;
}

template <typename T, T... M>
constexpr auto operator-(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  a -= b;
  return a;
}

template <typename T, T... M>
constexpr auto operator-(ZqElement<T, M...> a) {
  big_int<sizeof...(M), T> mod { M... };
  return ZqElement<T, M...>{ mod - a.data };
}


template <typename T, T... M>
constexpr auto& operator*=(ZqElement<T, M...>& a, ZqElement<T, M...> b) {
  a = ZqElement<T, M...> { mod(mul(a.data, b.data), std::integer_sequence<T, M...>()), skip_reduction{} };
  return a;
}

template <typename T, T... M>
constexpr auto operator*(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  a *= b;
  return a;
}

template <typename T, T... M>
constexpr auto& operator/=(ZqElement<T, M...>& a, ZqElement<T, M...> b) {
  a = ZqElement<T, M...>{
      mod(mul(a.data, mod_inv(b.data, big_int<sizeof...(M), T>{M...})),
          std::integer_sequence<T, M...>()),
      skip_reduction{}};
  return a;
}

template <typename T, T... M>
constexpr auto operator/(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  a /= b;
  return a;
}

template <typename T, T... M>
std::ostream &operator<<(std::ostream& strm, const ZqElement<T, M...>& obj)
{
  strm << obj.data;
  return strm;
}

template <typename T, T... M>
constexpr bool operator==(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  return a.data == b.data;
}

template <typename T, T... M>
constexpr bool operator!=(ZqElement<T, M...> a, ZqElement<T, M...> b) {
  return !(a == b);
}


}
#endif
