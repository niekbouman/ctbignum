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
    //Array<T, 1> u_i = {accum[i] * mprime};
    //auto prod = limbwise_shift_left<N2+1>(m, i);
    //auto prod2 = mul(u_i, prod);
    
    auto prod = mul(Array<T, 1>{accum[i] * mprime}, m);
    auto prod2 = limbwise_shift_left<N1+1>(prod, i);
    
    //accum = accumulate(accum, prod2);
    accum = mp_add_ignore_last_carry(accum, prod2);
  }

  auto result = skip<N2>(accum);

  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, result))
    result = mp_sub(result, padded_mod);

  return first<N2>(result);
}

//template <template <typename, std::size_t> class Array, typename T,
//          std::size_t N1, std::size_t... Modulus>
//constexpr auto barrett_reduction(Array<T, N1> x,
//                                 std::index_sequence<Modulus...>) {

template <template <typename, std::size_t> class Array, typename T,
          std::size_t N, T... Modulus>
constexpr auto montgomery_mul(Array<T, N> x, Array<T, N> y,  std::integer_sequence<T,Modulus...>)
    

   // Array<T, N> m,
   //                           uint64_t mprime) 
                              
  {
  // Montgomery multiplication

  using detail::skip;
  using detail::first;
  using detail::pad;


  // compute mprime
  auto m = sprout::make_array(Modulus...);

  
  Array<T, N + 1> A{};

  for (auto i = 0; i < N; ++i) {
    uint64_t u_i = (A[0] + x[i] * y[0]) * mprime;
    auto prod = mul(Array<T, 1>{x[i]}, y);
    auto prod2 = mul(Array<T, 1>{u_i}, m);
    A = accumulate(A, prod);
    A = accumulate(A, prod2);
    A = skip<1, 1>(A);
  }
  
  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, A))
    A = mp_sub(A, padded_mod);
  return first<N>(A);
}




template <template <typename, std::size_t> class Array, typename T,
          std::size_t N>
constexpr auto montgomery_mul(Array<T, N> x, Array<T, N> y, Array<T, N> m,
                              uint64_t mprime) {
  // Montgomery multiplication

  using detail::skip;
  using detail::first;
  using detail::pad;
  
  Array<T, N + 1> A{};

  for (auto i = 0; i < N; ++i) {
    uint64_t u_i = (A[0] + x[i] * y[0]) * mprime;
    auto prod = mul(Array<T, 1>{x[i]}, y);
    auto prod2 = mul(Array<T, 1>{u_i}, m);
    A = accumulate(A, prod);
    A = accumulate(A, prod2);
    A = skip<1, 1>(A);
  }
  
  auto padded_mod = pad<1>(m);
  if (!greater_than(padded_mod, A))
    A = mp_sub(A, padded_mod);
  return first<N>(A);
}


}
#endif
