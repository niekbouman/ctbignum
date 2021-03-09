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
#ifndef CT_MODINV_HPP
#define CT_MODINV_HPP

#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/utility.hpp>
#include <tuple>

namespace cbn {

// Niels Moller's inversion algorithm
// see: 
// "Fast Software Polynomial Multiplication
// on ARM Processors Using the NEON Engine", Algorithm 5
// (its exposition contains a typo: v must be initialized to 0 instead of 1)
//
// This version is not constant-time;
// it can be made constant-time using conditional adds/subs/swaps.
template <size_t N, typename T>
constexpr auto mod_inv(big_int<N, T> const &x, big_int<N, T> const &n) {
  using detail::first;
  using detail::to_length;
  using std::tie;
  using std::tuple;

  big_int<N, T> a{x};
  big_int<N, T> b{n};
  big_int<N, T> u{1};
  big_int<N, T> v{0};

  int ell = detail::bit_length(n);

  for (int i = 0; i < 2 * ell; ++i) {

    T odd = a[0] & 1;
    T gteq = (a >= b);
    if (odd && gteq) {
      a = subtract_ignore_carry(a, b);
    } else if (odd && !gteq) {
      tie(a, b, u, v) = tuple(subtract_ignore_carry(b, a), a, v, u);
    }
    a = shift_right(a, 1);

    T gteq2 = (u >= v);

    if (odd && gteq2) {
      u = subtract_ignore_carry(u, v);
    } else if (odd && !gteq2) {
      u = first<N>(subtract(add_same(u, n), v));
    }

    auto tmp = to_length<N + 1>(u);
    if (u[0] & 1) {
      tmp = add(u, n);
    }
    u = detail::first<N>(shift_right(tmp, 1));
  }
  return v;
}
}
#endif

