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

#include <ctbignum/bigint.hpp>
#include <ctbignum/decimal_literals.hpp>
#include <ctbignum/field.hpp>
#include <type_traits>

TEST_CASE("Initialization") {

  using namespace cbn;
  using namespace cbn::literals;
 
  auto num = 6513020836420374401749667047018991798096360820_Z;
  constexpr big_int<3> res = {{1315566964, 326042948, 19140048}};

  REQUIRE(res == to_big_int(num));

  static_assert(std::is_same<decltype(num), std::integer_sequence<uint64_t, 1315566964, 326042948, 19140048>>::value);
  static_assert(res == to_big_int(num));
}

TEST_CASE("Initialization of Zq") {

  using namespace cbn;
  using namespace cbn::literals;

  using GF101 = decltype(Zq(1267650600228229401496703205653_Z));

  GF101 x {6765060022822940149_Z};
  constexpr cbn::ZqElement<uint64_t, 277, 68719476736> y {6765060022822940149_Z};

  static_assert(std::is_same<decltype(x), cbn::ZqElement<uint64_t, 277, 68719476736>>::value);
  REQUIRE(x.data == y.data);
}


