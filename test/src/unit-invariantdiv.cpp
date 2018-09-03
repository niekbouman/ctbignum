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

#include <ctbignum/ctbignum.hpp>

#include <iostream>

TEST_CASE("Finite Field class") {

  using namespace cbn;

  SECTION("Division by invariant integer") {
    auto divisor = 12676500229401496703205653_Z;
    constexpr auto divident = to_big_int(1237940039285380274899124054_Z);
    auto quotient = div(divident, divisor);
    auto z = to_big_int(97_Z);
    REQUIRE(quotient == z);

    REQUIRE(div(to_big_int(
                    983475380176018765409862740849389839818376450874681244_Z),
                182016134597461982754761_Z) ==
            to_big_int(5403231874751241207206728823349_Z));
  }

  SECTION("Modulo reduction") {
    auto x = to_big_int(31892671234697813764376159476_Z);
    auto reduced = mod(x, 837201975682367548938754_Z);
    REQUIRE(reduced == to_big_int(299173053704355103264600_Z));
  }

}
