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
#ifndef CT_GCD_HPP
#define CT_GCD_HPP

#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/utility.hpp>

#include <cstddef>

namespace cbn {
namespace detail {

template <typename T, T... A, T... B, T... Is, std::size_t N = sizeof...(Is)>
constexpr auto ext_gcd_impl(std::integer_sequence<T, A...>,
                            std::integer_sequence<T, B...>,
                            std::integer_sequence<T, Is...>) {

  using detail::pad;
  using detail::first;
  using detail::take;
  using detail::skip;
  using detail::join;

  constexpr auto a = big_int<N, T>{A...};
  constexpr auto b = big_int<N, T>{B...};
  constexpr auto dummy = big_int<N, T>{};

  constexpr bool a_equals_zero =
      std::is_same<std::integer_sequence<T, A...>,
                   std::integer_sequence<T, dummy[Is]...>>::value;
  if
    constexpr(a_equals_zero) return join(
        b, join(big_int<N, T>{0}, big_int<N, T>{1}));

  else {
    constexpr auto qr = div(b, a);
    constexpr auto rem = qr.remainder;
    constexpr auto arg1 = pad<N - rem.size()>(rem);

    constexpr auto triple =
        ext_gcd_impl(std::integer_sequence<T, arg1[Is]...>(),
                     std::integer_sequence<T, a[Is]...>(),
                     std::integer_sequence<T, Is...>());

    constexpr auto x = first<N>(triple);
    constexpr auto y = take<N, 2 * N>(triple);
    constexpr auto z = skip<2 * N>(triple);
    constexpr auto qy = partial_mul<N>(qr.quotient, y);

    return join(join(x, subtract_ignore_carry(z, qy)), y);
  }
}
}

template <typename T, T... A, T... B>
constexpr auto ext_gcd(std::integer_sequence<T, A...>,
                       std::integer_sequence<T, B...>) {
  constexpr std::size_t N = std::max(sizeof...(A), sizeof...(B));
  return detail::ext_gcd_impl(std::integer_sequence<T, A...>{},
                              std::integer_sequence<T, B...>{},
                              std::make_integer_sequence<T, N>{});
}

template <typename T, T... X, T... Modulus>
constexpr auto mod_inv(std::integer_sequence<T, X...>,
                       std::integer_sequence<T, Modulus...>) {

  constexpr auto triple = ext_gcd(std::integer_sequence<T, X...>{},
                                  std::integer_sequence<T, Modulus...>{});
  constexpr auto N = std::max(sizeof...(X), sizeof...(Modulus));

  if (triple[0] != 1) {
    throw std::runtime_error("modular inverse does not exist");
  } else {
    using namespace detail;
    constexpr auto mod_inverse = take<N, 2 * N>(triple);
    constexpr auto L = tight_length(mod_inverse);
    return first<L>(mod_inverse);
  }
}
}
#endif

/*
template <template <typename, size_t> class Array = std::array, typename T,
          T... A, T... B, T... Is>
constexpr auto ext_gcd_(std::integer_sequence<T, A...>,
                       std::integer_sequence<T, B...>) {
  //constexpr size_t N = std::max(sizeof...(A), sizeof...(B));
  //return detail::ext_gcd_impl(std::integer_sequence<T, A...>{},
  //                            std::integer_sequence<T, B...>{},
  //                            std::make_integer_sequence<T, N>{});

//template <size_t L, typename T, size_t N>
//constexpr auto ext_gcd_(big_int<N, T> a, big_int<N, T> b) {

  using cbn::detail::join;
  const size_t N = sizeof...(A);

  auto a = big_int<N, T>{A...};
  auto b = big_int<N, T>{B...};

  big_int<N, T> x0{1};
  big_int<N, T> x1{};
  big_int<N, T> y0{};
  big_int<N, T> y1{1};
  big_int<N, T> zero{};

  while (b != zero)
  {

    constexpr auto L = detail::tight_length(b);
    constexpr auto qr = cbn::meta_div<L>(a, b);

    a = b;
    b = qr.remainder;
    constexpr auto tmp = x1;
    x1 = cbn::subtract_ignore_carry(x0, cbn::partial_mul<N>(qr.quotient, x1));
    x0 = tmp;

    constexpr auto tmp2 = y1;
    y1 = cbn::subtract_ignore_carry(y0, cbn::partial_mul<N>(qr.quotient, y1));
    y0 = tmp2;
  }

  return join(a, join(x0, y0));
}
*/


/*
template <typename T, size_t N>
constexpr auto ext_gcd(big_int<N, T> a, big_int<N, T> b) {

  using detail::pad;
  using detail::first;
  using detail::take;
  using detail::skip;
  using detail::join;

  // todo make padding so that outputs have same length in both branches
  if (a == big_int<N, T>{}) {
    return join(b, join(big_int<N, T>{0}, big_int<N, T>{1}));
  } else {
    constexpr auto qr = div(b, a);
    constexpr auto rem = qr.remainder;
    constexpr auto triple = ext_gcd(pad<N - rem.size()>(rem), a);

    constexpr auto x = first<N>(triple);
    constexpr auto y = take<N, 2 * N>(triple);
    constexpr auto z = skip<2 * N>(triple);

    constexpr auto qy = mul(qr.quotient, y);
    return join(join(x, subtract_ignore_carry(z, pad<N - qy.size()>(qy))), y);
  }
}

template <typename T, size_t N,
          T... Modulus>
constexpr auto mod_inv(big_int<N, T> a, std::integer_sequence<T, Modulus...>) {

  auto modulus = sprout::make_array(Modulus...);
  auto triple = ext_gcd(a, modulus);

  if (triple[0] != 1) {
    throw std::runtime_error("modular inverse does not exist");
  } else {
    auto y = detail::take<N, 2 * N>(triple);
    return y; //barrett_reduction<Modulus...>(y);
  }
}
*/

