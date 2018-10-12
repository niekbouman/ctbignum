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

TEST_CASE("Test division invariant integer") {

  using namespace cbn;
  using namespace cbn::literals;

  SECTION("Division by invariant integer") {
    auto divisor = 12676500229401496703205653_Z;
    constexpr auto divident = to_big_int(1237940039285380274899124054_Z);
    auto quotient_ = quotient(divident, divisor);
    auto z = to_big_int(97_Z);
    REQUIRE(quotient_ == z);

    REQUIRE(quotient(to_big_int(
                    983475380176018765409862740849389839818376450874681244_Z),
                182016134597461982754761_Z) ==
            to_big_int(5403231874751241207206728823349_Z));

  }

  SECTION("Division by power of two") {
    REQUIRE(
        quotient(to_big_int(983475380176018765409862740849389839818376450874681244_Z), 16_Z) 
        ==
        to_big_int(61467211261001172838116421303086864988648528179667577_Z)
    );
  }

  SECTION("Modulo reduction") {
    auto x = to_big_int(31892671234697813764376159476_Z);
    auto reduced = mod(x, 837201975682367548938754_Z);
    REQUIRE(reduced == to_big_int(299173053704355103264600_Z));
    static_assert(mod(to_big_int(31892671234697813764376159476_Z), 837201975682367548938754_Z)== to_big_int(299173053704355103264600_Z));


    static_assert(mod(to_big_int(31892687983792345871234697813764376159476_Z), 83720129120938471902840198708975682367548938754_Z)== to_big_int(31892687983792345871234697813764376159476_Z));

    static_assert(mod(to_big_int(31892687983792345871234697813764376159476_Z), 83720129181230479813709817203921820938471902840198708975682367548938754_Z)== to_big_int(31892687983792345871234697813764376159476_Z));

    auto y = 67299800976976978983750984320398459013870498571098367219058741975409186309861967351904375_Z;
    constexpr auto d = quotient(to_big_int(y), 2708751239578901865981734_Z);
    static_assert(d == to_big_int(24845323554864085035650716243910772137645983897610160892737669527_Z));

    auto z = 1918364187619837651267354918237508473169875469187436983576158736587620837561389746587368276987234_Z;
    constexpr auto n = to_big_int(31892687983792345871234697813764376159476_Z);
    constexpr auto dee = quotient(n, z);
    
    static_assert(dee == big_int<1>{0});

    constexpr auto n2 = to_big_int(1918364187619837651267354918237508463169875469187436983576158736587620837561389746587368276987232_Z);
    constexpr auto dee2 = mod(n2, z);
    
    static_assert(dee2 == n2);


  }

}
