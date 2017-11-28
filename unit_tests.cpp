#include "catch.hpp"

#include "addition.hpp"
#include "field.hpp"
#include <sprout/array.hpp>

template <size_t N> using big_int = sprout::array<uint64_t, N>;

TEST_CASE("Addition") {

  constexpr big_int<2> a = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> b = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> p = {{181UL, 13835058055282163712UL}};

  constexpr big_int<2> correct_answer = {
      {18446744073709551435UL, 4611686018427387903UL}};

  constexpr auto result_ugly_syntax =
      mod_add_<sprout::array, uint64_t, 2UL, 181UL, 13835058055282163712UL>(a,
                                                                            b);

  REQUIRE(result_ugly_syntax == correct_answer);
  static_assert(result_ugly_syntax == correct_answer, "fail");

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

