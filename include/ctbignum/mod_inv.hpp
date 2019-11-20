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
  // compute a^-1 mod m, where m = modulus
  big_int<N, T> u1{1};
  big_int<N, T> u3{a};

  big_int<N, T> v1{0};
  big_int<N, T> v3{modulus};

  bool iter_parity = false;
  while (v3 != big_int<1, T>{0}) {

    auto qr = div(u3, v3);
    auto tmp = add_ignore_carry(u1, partial_mul<N>(v1, qr.quotient));
    u1 = v1;
    v1 = tmp;
    u3 = v3;
    v3 = qr.remainder;

    iter_parity = !iter_parity;
  }

  // if u3 != 1 then we could notify the caller that "a is not invertible"
  // TODO: maybe via a pointer-to-bool (that is passed by the caller)?

  if (iter_parity)
    u1 = subtract_ignore_carry(modulus, u1);
  return u1;
}

}
#endif

