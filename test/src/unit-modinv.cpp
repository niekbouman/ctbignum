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
#include "catch.hpp"

#include <ctbignum/addition.hpp>
#include <ctbignum/decimal_literals.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/mod_inv.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/print.hpp>

#include <NTL/ZZ.h>
#include <NTL/ZZ_limbs.h>
#include <random>
#include <cstddef>

template <typename T> class Randomizer {
public:
  template <size_t N> void operator()(cbn::big_int<N, T> &a) {
    for (auto &limb : a)
      limb = distribution(generator);
  }

private:
  std::default_random_engine generator;
  std::uniform_int_distribution<T> distribution;
};

TEST_CASE("Runtime modular inverses") {
 
  using namespace cbn;
  using namespace cbn::literals;

  using T = uint64_t;

  big_int<4,T> a{};
  big_int<4,T> m{};

  Randomizer<T> randomize;

  for (auto i = 0; i < 100; ++i)
  {

    NTL::ZZ m_ZZ = NTL::RandomPrime_ZZ(255);
    std::copy(NTL::ZZ_limbs_get(m_ZZ), NTL::ZZ_limbs_get(m_ZZ) + 4, m.begin());

    do
      randomize(a);
    while (a >= m);

    auto b = mod_inv(a, m);
    REQUIRE(mul(a, b) % m == to_big_int(1_Z));
  }
}

TEST_CASE("New modinv test")
{
    using namespace cbn::literals;

    constexpr auto p = cbn::to_big_int(115792089237316195423570985008687907853269984665640564039457584007908834671663_Z);
    constexpr auto a = cbn::to_big_int(65341020041517633956166170261014086368942546761318486551877808671514674964848_Z);

    static_assert (cbn::mod_inv(a, p) == cbn::to_big_int(83174505189910067536517124096019359197644205712500122884473429251812128958118_Z));

    REQUIRE(cbn::mod_inv(a, p) == cbn::to_big_int(83174505189910067536517124096019359197644205712500122884473429251812128958118_Z));
}


