#ifndef CT_MONTGOMERY_HPP
#define CT_MONTGOMERY_HPP

#include <cstddef> // std::size_t

#include <ctbignum/addition.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>

namespace cbn {

template <template <typename, std::size_t> class Array, typename T,
          std::size_t N1, std::size_t N2>
constexpr auto montgomery_reduction(Array<T, N1> A, Array<T, N2> m,
                                    uint64_t mprime) {
  // Montgomery reduction
  //
  // inputs: 
  //  A       (2n limbs)  number to be reduced
  //  m       ( n limbs)  modulus
  //  mprime  (uint64_t)  mprime = - m^{-1} mod 2^64
  //
  // output:
  //  T R^-1 mod m,       where R = (2^64)^n
  //

  using detail::skip;
  using detail::first;
  using detail::unary_encoding;
  using detail::pad;
  using detail::limbwise_shift_left;

 
  auto accum = pad<1>(A);

  for (auto i = 0; i < N2; ++i) {
    Array<T, 1> u_i = {accum[i] * mprime};
    auto prod = limbwise_shift_left<N2+1>(m, i);
    auto prod2 = mul(u_i, prod);
    accum = accumulate(accum, prod2);
  }

  auto result = skip<N2>(accum);

  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, result))
    result = mp_sub(result, padded_mod);

  return first<N2>(result);
}
}
#endif
