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
#include <ctbignum/io.hpp>

#include <sstream>

TEST_CASE("String output") {

  using namespace cbn;
  using namespace cbn::literals;

  std::stringstream ss;

  constexpr auto num = to_big_int(98237634176419028461881263_Z);
  ss << num;
  REQUIRE(ss.str() == "98237634176419028461881263");

  ss.str(std::string());
  ss << to_big_int(9839428129_Z);
  REQUIRE(ss.str() == "9839428129");

  ss.str(std::string());
  ss << to_big_int(1928170198273428374918347129851872667431_Z);
  REQUIRE(ss.str() == "1928170198273428374918347129851872667431");

  ss.str(std::string());
  ss << to_big_int(0_Z);
  REQUIRE(ss.str() == "0");

  ss.str(std::string());
  ss << to_big_int(1_Z);
  REQUIRE(ss.str() == "1");

}


