#include "catch.hpp"

#include <ctbignum/all.hpp>
#include <boost/hana.hpp>

TEST_CASE("Finite Field class") {

  using namespace cbn;

  SECTION("Initialization") {

    auto m = BOOST_HANA_STRING("1267650600228229401496703205653");
    auto modulus = string_to_integer_seq(m);
    using GF101 = decltype(Zq(modulus));

    constexpr auto s = BOOST_HANA_STRING("1268888540267514781771602329707");

    constexpr GF101 y {s};
    constexpr GF101 z {string_to_big_int<2>(s)};

    constexpr auto result = string_to_big_int(BOOST_HANA_STRING("1237940039285380274899124054"));

    static_assert(z.data == result);
    static_assert(y.data == result);

    REQUIRE(y.data == result);
    REQUIRE(z.data == result);
  }

  SECTION("Alternative Initialization") {

    using namespace boost::hana::literals;
    using GF101 = decltype(Zq("1267650600228229401496703205653"_s));

    constexpr GF101 x("1268888540267514781771602329707"_s);

    constexpr auto result = string_to_big_int(BOOST_HANA_STRING("1237940039285380274899124054"));

    static_assert(x.data == result);
    REQUIRE(x.data == result);


  }



}
