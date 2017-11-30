#ifndef CT_ADDITION_HPP
#define CT_ADDITION_HPP

#include "relational_ops.hpp"
#include <cstddef>

template <template <typename, size_t> class Array, typename T, size_t N>
constexpr auto mp_sub(Array<T, N> u, Array<T, N> v) {
  T carry = static_cast<T>(0);
  Array<T, N> r{};

  for (auto i = 0; i < N; ++i) {
    auto uu = u[i];
    auto diff = uu - v[i];
    auto res = diff - carry;
    carry = (diff > uu) | (res > diff);
    r[i] = res;
  }

  return r;
}

template <template <typename, size_t> class Array, typename T, size_t N>
constexpr auto mp_sub_carry_out(Array<T, N> u, Array<T, N> v) {
  T carry = static_cast<T>(0);
  Array<T, N+1> r{};

  for (auto i = 0; i < N; ++i) {
    auto uu = u[i];
    auto diff = uu - v[i];
    auto res = diff - carry;
    carry = (diff > uu) | (res > diff);
    r[i] = res;
  }
  r[N] = carry;

  return r;
}


template <template <typename, size_t> class Array, typename T, size_t N>
constexpr auto mp_add_ignore_last_carry(Array<T, N> a, Array<T, N> b) {
  T carry = 0;
  Array<T, N> r{};

  for (auto i = 0; i < N; ++i) {
    T aa = a[i];
    T sum = aa + b[i];
    T res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }

  return r;
}

template <template <typename, size_t> class Array, typename T, size_t N>
constexpr auto mod_add(Array<T, N> a, Array<T, N> b, Array<T, N> modulus) {
  T carry = static_cast<T>(0);
  Array<T, N> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto sum = aa + b[i];
    auto res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }

  if (carry || !greater_than(modulus, r)) 
    r = mp_sub(r, modulus);

  return r;
}

template <template <typename, size_t> class Array, typename T, size_t N,
          T... Modulus>
constexpr auto mod_add_(Array<T, N> a, Array<T, N> b) {
  Array<T, sizeof...(Modulus)> modulus{{Modulus...}};
  return mod_add(a, b, modulus);
}

#endif
