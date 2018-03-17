#ifndef CT_INPUTOUTPUT_HPP
#define CT_INPUTOUTPUT_HPP

#include <cmath>
#include <cstddef>
#include <ctbignum/bigint.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/pow.hpp>
#include <ctbignum/utility.hpp>
#include <limits>

template <std::size_t N, typename T>
std::ostream &operator<<(std::ostream &strm, cbn::big_int<N, T> obj) {
  using namespace cbn;
  size_t bitlen = detail::bit_length(obj);
  auto power_of_ten = pow(big_int<N, T>{10}, static_cast<T>(25 * bitlen / 83));
  auto zero = big_int<N, T>{};
  bool skip_zeros = true;
  while (power_of_ten != zero) {
    auto qr = div_v(obj, power_of_ten);
    detail::assign(obj, qr.second);
    if (qr.first[0] > 9)
      throw std::runtime_error("division output");
    char digit = 48 + qr.first[0];
    if (digit != '0' || (digit == '0' && !skip_zeros)) {
      strm << digit;
      skip_zeros = false;
    }
    detail::assign(power_of_ten,
                   short_div(power_of_ten, static_cast<T>(10)).first);
  }
  return strm;
}

#endif
