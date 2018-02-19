#ifndef CT_ADDITION_HPP
#define CT_ADDITION_HPP

#include <ctbignum/relational_ops.hpp>
#include <cstddef>

namespace cbn {

template <template <typename, size_t> class Array, typename T, size_t N>
constexpr auto mp_sub(Array<T, N> a, Array<T, N> b) {
  T carry = static_cast<T>(0);
  Array<T, N> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto diff = aa - b[i];
    auto res = diff - carry;
    carry = (diff > aa) | (res > diff);
    r[i] = res;
  }

  return r;
}

template <template <typename, size_t> class Array, typename T, size_t N>
constexpr auto mp_sub_carry_out(Array<T, N> a, Array<T, N> b) {
  T carry = static_cast<T>(0);
  Array<T, N+1> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto diff = aa - b[i];
    auto res = diff - carry;
    carry = (diff > aa) | (res > diff);
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

template <template <typename, size_t> class Array, typename T, size_t N1, size_t N2>
constexpr auto accumulate(Array<T, N1> accum, Array<T, N2> b) {
  T carry = 0;
  Array<T, N1> r{};

  auto m = std::min(N1,N2);
  for (auto i = 0; i < m; ++i) {
    auto aa = accum[i];
    auto sum = aa + b[i];
    auto res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }
  if (N1>N2)
    r[N2] = carry;
  return r;
}


} // end namespace cbn

#endif
