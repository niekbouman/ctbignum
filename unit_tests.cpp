#include "catch.hpp"

#include <boost/hana.hpp>
#include <boost/hana/string.hpp>

#include <sprout/array.hpp>
#include <iostream>

#include "addition.hpp"
#include "field.hpp"
#include "mult.hpp"
#include "initialization.hpp"  

template <size_t N> using big_int = sprout::array<uint64_t, N>;
using namespace std;

TEST_CASE("Addition") {

  constexpr big_int<2> a = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> b = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> p = {{181UL, 13835058055282163712UL}};

  constexpr big_int<2> correct_answer = {
      {18446744073709551435UL, 4611686018427387903UL}};

  constexpr auto result_ugly_syntax =
      mod_add_<sprout::array, uint64_t, 2UL, 181UL, 13835058055282163712UL>(a,b);

  constexpr auto result_friendly_syntax = mod_add(a, b, p);

  REQUIRE(result_ugly_syntax == correct_answer);
  REQUIRE(result_friendly_syntax == correct_answer);
  static_assert(result_ugly_syntax == correct_answer, "fail");
  static_assert(result_friendly_syntax == correct_answer, "fail");

  Zq<181UL, 13835058055282163712UL> F{};
  constexpr auto result = F.add(a, b);

  REQUIRE(result == correct_answer);
  static_assert(result == correct_answer, "fail");
}

TEST_CASE("For any prime p, assert that p + 0 = 0 mod p") {

  constexpr big_int<2> p = {{181UL, 13835058055282163712UL}};
  constexpr big_int<2> zero = {{0UL, 0UL}};
  constexpr big_int<2> correct_answer = zero;

  constexpr auto result = mod_add(p, zero, p);

  REQUIRE(result == correct_answer);
  static_assert(result == correct_answer, "fail");
}

TEST_CASE("Multiplication") {

  SECTION("") {
    constexpr big_int<2> a = {{9223372036854753777UL, 0}};
    constexpr big_int<2> b = {{51461, 0}};
    constexpr big_int<4> res = {{9223372035721038517, 25730, 0, 0}};

    constexpr auto ans = mp_mul<4>(a, b);
    static_assert(res == ans, "fail");
  }
  SECTION("") {
    constexpr big_int<2> a = {{151, 23}};
    constexpr big_int<2> b = {{8712364, 832176}};
    constexpr big_int<4> res = {{1315566964, 326042948, 19140048, 0}};

    constexpr auto ans = mp_mul<4>(a, b);
    constexpr auto ans2 = mp_mul2<4>(a, b);
    static_assert(res == ans, "fail");
    static_assert(res == ans2, "fail");
  }
}

TEST_CASE("String Initialization") {

  auto s = BOOST_HANA_STRING("6513020836420374401749667047018991798096360820");
  constexpr auto num = string_to_big_int<4>(s);
  constexpr big_int<4> res = {{1315566964, 326042948, 19140048, 0}};

  REQUIRE(res == num);
  static_assert(res == num, "fail");
}

