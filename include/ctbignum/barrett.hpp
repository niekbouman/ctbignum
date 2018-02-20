#ifndef CT_BARRETT_HPP
#define CT_BARRETT_HPP

#include <cstddef> // std::size_t

#include <ctbignum/addition.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>

namespace cbn {

template <typename T, T... Modulus> constexpr auto precompute_mu() {
  auto modulus = sprout::make_array(Modulus...);
  const size_t twoN = 2 * sizeof...(Modulus);
  auto quot_rem = div(detail::unary_encoding<twoN, twoN + 1>(), modulus);
  return quot_rem.first;
}

template <uint64_t... Modulus, template <typename, std::size_t> class Array,
          typename T, std::size_t N1>
constexpr auto barrett_reduction(Array<T, N1> x) {

  // Barrett reduction, modulus as a template parameter.

  using detail::skip;
  using detail::first;
  using detail::unary_encoding;
  using detail::pad;

  auto modulus = sprout::make_array(Modulus...);
  auto mu = precompute_mu<uint64_t, Modulus...>();
  const size_t N2 = sizeof...(Modulus);

  auto q2approx = mul(skip<N2 - 1>(x), skip<N2 - 1>(mu));
  auto q3 = skip<2>(q2approx);

  auto r1 = first<N2 + 1>(x);
  auto r2 = partial_mul<N2 + 1>(q3, modulus);
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

template <template <typename, std::size_t> class Array, typename T,
          std::size_t N1, T... Modulus>
constexpr auto barrett_reduction(Array<T, N1> x,
                                 std::integer_sequence<T, Modulus...>) {
  return barrett_reduction<Modulus...>(x);
}

template <template <typename, std::size_t> class Array, typename T,
          std::size_t N1, std::size_t N2, std::size_t N3>
constexpr auto barrett_reduction(Array<T, N1> x, Array<T, N2> modulus,
                                 Array<T, N3> mu) {

  // Barrett reduction, when given modulus and precomputed value mu that depends
  // on modulus as ordinary parameters.

  using detail::skip;
  using detail::first;
  using detail::unary_encoding;
  using detail::pad;

  auto q2approx = mul(skip<N2 - 1>(x), skip<N2 - 1>(mu));
  auto q3 = skip<2>(q2approx);

  auto r1 = first<N2 + 1>(x);
  auto r2 = partial_mul<N2 + 1>(q3, modulus);
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
}
#endif
