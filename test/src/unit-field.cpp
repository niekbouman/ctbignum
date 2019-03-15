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

#include <sstream>

TEST_CASE("Finite Field class") {

  using namespace cbn;
  using namespace cbn::literals;

  SECTION("Initialization") {

    auto modulus = 1267650600228229401496703205653_Z;
    using GF101 = decltype(Zq(modulus));

    constexpr GF101 y{1268888540267514781771602329707_Z};
    constexpr GF101 z{1268888540267514781771602329707_Z};
    constexpr auto result = to_big_int(1237940039285380274899124054_Z);

    static_assert(z.data == result);
    static_assert(y.data == result);

    REQUIRE(y.data == result);
    REQUIRE(z.data == result);
  }

  SECTION("Alternative initialization") {

    using GF101 = decltype(Zq(1267650600228229401496703205653_Z));
    constexpr GF101 x(1268888540267514781771602329707_Z);
    constexpr auto result = to_big_int(1237940039285380274899124054_Z);
    static_assert(x.data == result);
    REQUIRE(x.data == result);
  }

  SECTION("Compound multiplication assignment and multiplication") {

    using GF101 = decltype(Zq(1267650600228229401496703205653_Z));

    constexpr GF101 x(543195761203162351763512095426_Z);
    constexpr GF101 y(213461909783715623473362549_Z);
    constexpr GF101 result(432496613435633654896620861341_Z);

    auto z = x;
    z *= y;

    REQUIRE(result.data == z.data);
    REQUIRE((x * y).data == result.data);
    static_assert((x * y).data == result.data);
  }

  SECTION("Compound addition assignment and addition") {

    using GF101 = decltype(Zq(1267650600228229401496703205653_Z));

    constexpr GF101 x(543195761203162758351763512095426_Z);
    constexpr GF101 y(213461909783715623473362549_Z);
    constexpr GF101 result(641517767390358226798013438491_Z);

    auto z = x;
    z += y;

    REQUIRE(result.data == z.data);
    REQUIRE(result.data == (x + y).data);
    static_assert((x + y).data == result.data);
  }

  SECTION("Compound subtraction assignment and subtraction") {

    using GF101 = decltype(Zq(1267650600228229401496703205653_Z));

    constexpr GF101 x(543195761203162758351763512095426_Z);
    constexpr GF101 y(213461909783715623473362549_Z);
    constexpr GF101 result(626559756657438605945636492260_Z);

    auto z = y;
    z -= x;

    REQUIRE(result.data == z.data);
    REQUIRE((y - x).data == z.data);
    static_assert((y - x).data == result.data);
  }

  SECTION("Compound division assignment and division") {

    using GF101 = decltype(Zq(1267650600228229401496703205653_Z));

    constexpr GF101 x(543195761203162758351763512095426_Z);
    constexpr GF101 y(213461909783715623473362549_Z);
    constexpr GF101 result(193451197817739039113324731771_Z);

    auto z = x;
    z /= y;

    REQUIRE(result.data == z.data);
    REQUIRE((x / y).data == z.data);
    static_assert((x / y).data == result.data);
  }
}

TEST_CASE("Finite Field class - output to stream") {

  using namespace cbn;
  using namespace cbn::literals;

  auto modulus = 83124387682521574012837928367540946873_Z;
  // some dummy modulus

  using GF = decltype(Zq(modulus));
  constexpr GF num{4387682521574012837928367540_Z};
  // some dummy number, smaller than the modulus

  std::stringstream ss;
  ss << num;

  REQUIRE(ss.str() == "4387682521574012837928367540");
}

