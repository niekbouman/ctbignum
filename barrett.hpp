#ifndef CT_BARRETT_HPP
#define CT_BARRETT_HPP

#include <cstddef> // std::size_t

#include "relational_ops.hpp"
#include "slicing.hpp"
#include "mult.hpp"
#include "addition.hpp"
#include "print.hpp"

template <template <typename, std::size_t> class Array, typename T, std::size_t N1,
          std::size_t N2, std::size_t N3>
constexpr 
auto barrett_reduction(Array<T, N1> x, Array<T, N2> modulus,
                                 Array<T, N3> mu) {
  // Barrett reduction, when given precomputed value mu that depends on modulus.
  //
  // terminology:
  // skip<N>(x) corresponds with right-shifting x by N limbs
  // first<N>(x) corresponds with x modulo (2^64)^N

  auto q2approx = mul(skip<N2 - 1>(x), skip<N2 - 1>(mu));
  auto q3 = skip<2>(q2approx);
  
  auto r1 = first<N2 + 1>(x);
  auto r2 = first<N2 + 1>(mul<1>(q3, modulus));
  auto r_with_carry = mp_sub_carry_out(r1, r2);
  auto r = first<N2 + 1>(r_with_carry);

  if (r_with_carry[N2 + 1])
    r = mp_add_ignore_last_carry(r, unary_encoding<N2, N2 + 1>());
 
  auto padded_mod = pad<1>(modulus);
  if (!greater_than(padded_mod, r))
    r = mp_sub(r, padded_mod);
  if (!greater_than(padded_mod, r))
    r = mp_sub(r, padded_mod);

  return first<N2>(r);
}
#endif
