#ifndef CT_MODULAR_EXP_HPP
#define CT_MODULAR_EXP_HPP

#include <cstddef>
#include <ctbignum/barrett.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>
#include <limits>
namespace cbn {

template <std::size_t N1, std::size_t N2, typename T, T... Modulus>
constexpr auto
mod_exp_montgomery(big_int<N1, T> a, big_int<N2, T> exp,
                   std::integer_sequence<T, Modulus...> modulus) {
  // modular exponentiation using Montgomery multiplication

  constexpr auto N = modulus.size();
  constexpr big_int<N, T> m{Modulus...};
  constexpr auto R_mod_m = div(detail::unary_encoding<N, N + 1>(), m).second;
  constexpr auto Rsq_mod_m = div(detail::unary_encoding<2 * N, 2 * N + 1>(), m).second;

  auto result = R_mod_m;
  auto base = montgomery_mul(a, Rsq_mod_m, modulus);

  big_int<N2, T> zero{};
  if (exp == zero)
    return big_int<N, T>{1};
  if (m == big_int<N, T>{1})
    return big_int<N, T>{0};

  while (true) {
    auto lsb = exp[0] & 1;
    exp = shift_right(exp, 1);
    if (lsb) {
      result = montgomery_mul(base, result, modulus);
      if (exp == zero)
        break;
    }
    base = montgomery_mul(base, base, modulus);
  }

  return montgomery_mul(result, big_int<N, T>{1}, modulus);
}

template <std::size_t N1, std::size_t N2, typename T, T... Modulus>
constexpr auto mod_exp(big_int<N1, T> a, big_int<N2, T> exp,
                       std::integer_sequence<T, Modulus...> modulus) {
  // modular exponentiation

  big_int<N2, T> zero{};
  big_int<modulus.size(), T> base{};
  big_int<modulus.size(), T> result{1};
  big_int<modulus.size(), T> m{Modulus...};

  if (exp == zero)
    return result;
  if (m == result)
    return base;

  if (greater_than(a, m)) {
    base = barrett_reduction(detail::pad<2*modulus.size() - N1>(a), modulus);
  } else {
    for (auto i = 0; i < a.size(); ++i)
      base[i] = a[i];
  }

  while (true) {
    auto lsb = exp[0] & 1;
    exp = shift_right(exp, 1);
    if (lsb) {
      result = barrett_reduction(mul(base, result), modulus);
      if (exp == zero)
        break;
    }
    base = barrett_reduction(mul(base, base), modulus); // square
  }
  return result;
}
}
#endif
