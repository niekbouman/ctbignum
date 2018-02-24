#ifndef CT_GCD_HPP
#define CT_GCD_HPP

#include <array>
#include <cstddef>
#include <ctbignum/addition.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/utility.hpp>
#include <ctbignum/bigint.hpp>

namespace cbn {
namespace detail {

//template <typename, size_t> class Array, 
template <typename T, T... A, T... B,
          T... Is, size_t N = sizeof...(Is)>
constexpr auto ext_gcd_impl(std::integer_sequence<T, A...>,
                            std::integer_sequence<T, B...>,
                            std::integer_sequence<T, Is...>) {

  using detail::pad;
  using detail::first;
  using detail::take;
  using detail::skip;
  using detail::join;

constexpr auto a = big_int<N ,T>{A...};
constexpr auto b = big_int<N, T>{B...};
constexpr auto dummy = big_int<N, T>{};

  //constexpr auto a = Array<T, N>{A...};
  //constexpr auto b = Array<T, N>{B...};
  //constexpr auto dummy = Array<T, N>{};

  constexpr bool a_equals_zero =
      std::is_same<std::integer_sequence<T, A...>,
                   std::integer_sequence<T, dummy[Is]...>>::value;
  if
    //constexpr(a_equals_zero) return join(b,
    //                                     join(Array<T, N>{0}, Array<T, N>{1}));
    constexpr(a_equals_zero) return join(b,
                                         join(big_int<N, T>{0}, big_int<N, T>{1}));


  else {
    constexpr auto L = detail::tight_length(std::integer_sequence<T, A...>{});
    constexpr auto qr = meta_div<L>(b, a);
    constexpr auto rem = qr.second;
    constexpr auto arg1 = pad<N - rem.size()>(rem);

    constexpr auto triple =
        ext_gcd_impl(std::integer_sequence<T, arg1[Is]...>(),
                     std::integer_sequence<T, a[Is]...>(),
                     std::integer_sequence<T, Is...>());

    constexpr auto x = first<N>(triple);
    constexpr auto y = take<N, 2 * N>(triple);
    constexpr auto z = skip<2 * N>(triple);
    constexpr auto qy = partial_mul<N>(qr.first, y);

    return join(join(x, mp_sub(z, qy)), y);
  }
}
}

/*
template <template <typename, size_t> class Array, typename T, std::size_t N>
struct extract_array
{
  extract_array (Array<T, N> k) {}
  //template <template <typename, size_t> 

  using type = Array;
};*/
//template <typename, size_t> class Array = std::array, 
template <typename T,
          T... A, T... B, T... Is>
constexpr auto ext_gcd(std::integer_sequence<T, A...>,
                       std::integer_sequence<T, B...>) {
  constexpr size_t N = std::max(sizeof...(A), sizeof...(B));
  return detail::ext_gcd_impl(std::integer_sequence<T, A...>{},
                                     std::integer_sequence<T, B...>{},
                                     std::make_integer_sequence<T, N>{});
}

//template <typename, size_t> class Array = std::array, 
template <typename T,
          T... X, T... Modulus>
constexpr auto mod_inv(std::integer_sequence<T, X...>,
                       std::integer_sequence<T, Modulus...>) {

  constexpr auto triple = ext_gcd(
      std::integer_sequence<T, X...>{}, std::integer_sequence<T, Modulus...>{});
  constexpr auto N = std::max(sizeof...(X), sizeof...(Modulus));

  if (triple[0] != 1) {
    throw std::runtime_error("modular inverse does not exist");
  } else {
    using namespace detail;
    constexpr auto mod_inverse = take<N, 2 * N>(triple);
    constexpr auto L = tight_length(mod_inverse);
    return first<L>(mod_inverse);
  }
}
}
#endif

/*
template <template <typename, size_t> class Array = std::array, typename T,
          T... A, T... B, T... Is>
constexpr auto ext_gcd_(std::integer_sequence<T, A...>,
                       std::integer_sequence<T, B...>) {
  //constexpr size_t N = std::max(sizeof...(A), sizeof...(B));
  //return detail::ext_gcd_impl(std::integer_sequence<T, A...>{},
  //                            std::integer_sequence<T, B...>{},
  //                            std::make_integer_sequence<T, N>{});

//template <size_t L, template <typename, size_t> class Array, typename T, size_t N>
//constexpr auto ext_gcd_(Array<T, N> a, Array<T, N> b) {

  using cbn::detail::join;
  const size_t N = sizeof...(A);

  auto a = Array<T, N>{A...};
  auto b = Array<T, N>{B...};

  Array<T, N> x0{1};
  Array<T, N> x1{};
  Array<T, N> y0{};
  Array<T, N> y1{1};
  Array<T, N> zero{};

  while (b != zero)
  {

    constexpr auto L = detail::tight_length(b);
    constexpr auto qr = cbn::meta_div<L>(a, b);

    a = b;
    b = qr.second;
    constexpr auto tmp = x1;
    x1 = cbn::mp_sub(x0, cbn::partial_mul<N>(qr.first, x1));
    x0 = tmp;

    constexpr auto tmp2 = y1;
    y1 = cbn::mp_sub(y0, cbn::partial_mul<N>(qr.first, y1));
    y0 = tmp2;
  }

  return join(a, join(x0, y0));
}
*/


/*
template <template <typename, size_t> class Array, typename T, size_t N>
constexpr auto ext_gcd(Array<T, N> a, Array<T, N> b) {

  using detail::pad;
  using detail::first;
  using detail::take;
  using detail::skip;
  using detail::join;

  // todo make padding so that outputs have same length in both branches
  if (a == Array<T, N>{}) {
    return join(b, join(Array<T, N>{0}, Array<T, N>{1}));
  } else {
    constexpr auto qr = div(b, a);
    constexpr auto rem = qr.second;
    constexpr auto triple = ext_gcd(pad<N - rem.size()>(rem), a);

    constexpr auto x = first<N>(triple);
    constexpr auto y = take<N, 2 * N>(triple);
    constexpr auto z = skip<2 * N>(triple);

    constexpr auto qy = mul(qr.first, y);
    return join(join(x, mp_sub(z, pad<N - qy.size()>(qy))), y);
  }
}

template <template <typename, size_t> class Array, typename T, size_t N,
          T... Modulus>
constexpr auto mod_inv(Array<T, N> a, std::integer_sequence<T, Modulus...>) {

  auto modulus = sprout::make_array(Modulus...);
  auto triple = ext_gcd(a, modulus);

  if (triple[0] != 1) {
    throw std::runtime_error("modular inverse does not exist");
  } else {
    auto y = detail::take<N, 2 * N>(triple);
    return y; //barrett_reduction<Modulus...>(y);
  }
}
*/

