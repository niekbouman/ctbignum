//
//   This file is part of:
//
//   CTBignum 
//
//   C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
//   
//   Copyright 2018 Authors
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
#include "catch.hpp"

#include <ctbignum/ctbignum.hpp>

#include <sstream>

TEST_CASE("Finite Field class") {

  using namespace cbn;

  SECTION("Initialization") {

    auto modulus = 1267650600228229401496703205653_Z;
    using GF101 = decltype(Zq(modulus));

    constexpr GF101 y {1268888540267514781771602329707_Z};
    constexpr GF101 z {1268888540267514781771602329707_Z};

    constexpr auto result = to_big_int(1237940039285380274899124054_Z);

    static_assert(z.data == result);
    static_assert(y.data == result);

    REQUIRE(y.data == result);
    REQUIRE(z.data == result);
  }

  SECTION("Alternative Initialization") {

    using GF101 = decltype(Zq(1267650600228229401496703205653_Z));
    constexpr GF101 x(1268888540267514781771602329707_Z);
    constexpr auto result = to_big_int(1237940039285380274899124054_Z);
    static_assert(x.data == result);
    REQUIRE(x.data == result);
  }

  SECTION("Multiplication") {

    using GF101 = decltype(Zq(1267650600228229401496703205653_Z));

    constexpr GF101 x(543195761203162351763512095426_Z);
    constexpr GF101 y(213461909783715623473362549_Z);

    constexpr GF101 result(432496613435633654896620861341_Z);

    static_assert((x*y).data == result.data);
    REQUIRE((x*y).data == result.data);
  }
}

TEST_CASE("Finite Field class - output to stream") {

  using namespace cbn;

  auto modulus = 83124387682521574012837928367540946873_Z;
  // some dummy modulus

  using GF = decltype(Zq(modulus));
  constexpr GF num{4387682521574012837928367540_Z};
  // some dummy number, smaller than the modulus

  std::stringstream ss;
  ss << num;

  REQUIRE(ss.str() == "4387682521574012837928367540");
}

