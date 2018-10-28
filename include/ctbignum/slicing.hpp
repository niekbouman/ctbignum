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
#ifndef CT_SLICING_HPP
#define CT_SLICING_HPP

#include <ctbignum/bigint.hpp>
#include <cstddef>

namespace cbn {
namespace detail {

template <typename T = uint64_t, T... Limbs, std::size_t... Is>
constexpr auto take_first(std::integer_sequence<T, Limbs...>,
                          std::index_sequence<Is...>) {
  constexpr big_int<sizeof...(Limbs), T> num = {Limbs...};
  return std::integer_sequence<T, num[Is]...>{};
}

template <size_t Begin, size_t End, size_t Padding=0, 
          typename T, size_t N1>
constexpr auto take(big_int< N1, T> t) {
  //static_assert(End >= Begin, "invalid range");
  //static_assert(End - Begin <= N1, "invalid range");

  big_int< End - Begin + Padding, T> res{};
  for (auto i = Begin; i < End; ++i) {
    res[i-Begin] = t[i];
  }

  return res;
}

template <size_t ResultLength, 
          typename T, size_t N1>
constexpr auto take(big_int< N1, T> t, const size_t Begin, const size_t End, const size_t Offset = 0) {

  big_int< ResultLength, T> res{};
  for (auto i = Begin; i < End; ++i) {
    res[i-Begin+Offset] = t[i];
  }

  return res;
}

template <size_t N, size_t Padding = 0, typename T,
          size_t N1>
constexpr auto skip(big_int<N1, T> t) {
  // skip first N limbs
  // skip<N>(x) corresponds with right-shifting x by N limbs
  return take<N, N1, Padding>(t);
}

template <typename T, size_t N1>
constexpr auto skip(big_int<N1, T> t, size_t N) {
  // skip first N limbs, runtime version
  // skip<N>(x) corresponds with right-shifting x by N limbs
  return take<N1>(t, N, N1);
}

template <size_t N, typename T,
          size_t N1>
constexpr auto first(big_int<N1, T> t) {
  // take first N limbs
  // first<N>(x) corresponds with x modulo (2^64)^N
  return take<0, N>(t);
}



template <size_t N, typename T,
          size_t N1>
constexpr auto pad(big_int<N1, T> t) {
  // add N extra limbs (at msb side)
  return take<0, N1, N>(t);
}

template <size_t N, typename T, size_t N1>
constexpr auto to_length(big_int<N1, T> t) {
  return (N1 < N) ? pad<N - N1>(t) : first<N>(t);
}

template <typename T,
          size_t N1, size_t N2>
constexpr auto join(big_int< N1, T> a, big_int< N2, T> b) {
  big_int< N1+N2, T> result {};

  for (auto i = 0; i<N1; ++i)
    result[i] = a[i];

  for (auto i = 0; i<N2; ++i)
    result[N1+i] = b[i];
  
  return result;
}

template <size_t ResultLength, typename T,
          size_t N1>
constexpr auto limbwise_shift_left(big_int< N1, T> t, const size_t k) {
  // shift left by k limbs (and produce output of limb-length ResultLength)
  return take<ResultLength>(t, 0, N1, k);
}

template <size_t K, size_t N, typename T = uint64_t>
constexpr auto unary_encoding() {
  // N limbs, Kth limb set to one
  big_int<N, T> res{};
  res[K] = 1;
  return res;
}

template <size_t N, typename T = uint64_t>
constexpr auto unary_encoding(size_t K) {
  big_int<N, T> res{};
  res[K] = 1;
  return res;
}

template <size_t N, typename T = uint64_t>
constexpr auto place_at(uint64_t value, size_t K) {
  // N limbs, Kth limb set to value
  big_int<N, T> res{};
  res[K] = value;
  return res;
}
}
}
#endif
