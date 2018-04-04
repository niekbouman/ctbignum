#include "catch.hpp"

#include <ctbignum/ctbignum.hpp>

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

