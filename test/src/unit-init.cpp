//
//   This file is part of:
//
//   CTBignum 
//
//   C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
//   
//   See the NOTICE file for the complete list of authors and contributors 
//
//   Copyright 2018 Niek J. Bouman
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

#include <ctbignum/bigint.hpp>
#include <ctbignum/initialization.hpp>
#include <ctbignum/field.hpp>

TEST_CASE("Initialization") {

  using namespace cbn;
  
  constexpr auto num = 6513020836420374401749667047018991798096360820_Z;

  auto k = to_big_int(658993_Z);


  constexpr big_int<3> res = {{1315566964, 326042948, 19140048}};

  REQUIRE(res == to_big_int(num));
  static_assert(res ==to_big_int(num) , "fail");
}

TEST_CASE("Initialization of Zq") {

  using namespace cbn;
  
  using GF101 = decltype(Zq(1267650600228229401496703205653_Z));

  GF101 x {6765060022822940149_Z};


  //constexpr auto num = 6513020836420374401749667047018991798096360820_Z;
  //constexpr big_int<3> res = {{1315566964, 326042948, 19140048}};

  //REQUIRE(res == num);
  //static_assert(res == num, "fail");
}


