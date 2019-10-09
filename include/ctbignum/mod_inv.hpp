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

#include <ctbignum/addition.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/utility.hpp>

#include <cstddef>

namespace cbn {

template <typename T, size_t N>
constexpr auto mod_inv(big_int<N, T> a, big_int<N, T> modulus) {
  big_int<N, T> t{0};
  big_int<N, T> newt{1};
  auto r = modulus;
  auto newr = a;

  while (newr != big_int<1, T>{0}) {
    auto qr = div(r, newr);
    auto tmp = t;
    t = newt;
    newt = subtract_ignore_carry(tmp, partial_mul<N>(qr.quotient, newt));
    r = newr;
    newr = qr.remainder;
  }
  // if r > 1 then return "a is not invertible"
  if (t > modulus)
    t = add_ignore_carry(t, modulus);
  return t;
}

}
#endif

