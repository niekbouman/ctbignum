#include "catch.hpp"

#include <boost/hana.hpp>
#include <boost/hana/string.hpp>

#include <sprout/array.hpp>
#include <iostream>

#include <ctbignum/addition.hpp>
#include <ctbignum/field.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/initialization.hpp>
#include <ctbignum/slicing.hpp>
#include <ctbignum/barrett.hpp>
#include <ctbignum/montgomery.hpp>
#include <ctbignum/division.hpp>
//#include <ctbignum/print.hpp>

#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>


template <size_t N> using big_int = sprout::array<uint64_t, N>;
using namespace std;


TEST_CASE("Addition") {

  using namespace cbn;

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

TEST_CASE("subtraction") {

  using namespace cbn;
  using cbn::detail::first;
  using cbn::detail::unary_encoding;

  constexpr big_int<2> a = {{15, 0}};
  constexpr big_int<2> b = {{10, 0}};
  constexpr big_int<2> d = {{6, 0}};

  constexpr big_int<2> c = {{5, 0}};

  static_assert(mp_sub(a,b) == c, "fail");
  
  constexpr auto res = mp_sub_carry_out(b,a);
  
  static_assert(res[2], "fail");

  static_assert(mp_add_ignore_last_carry(first<2>(res),d) == unary_encoding<0,2>(),"");

}


TEST_CASE("For any prime p, assert that p + 0 = 0 mod p") {

  using namespace cbn;

  constexpr big_int<2> p = {{181UL, 13835058055282163712UL}};
  constexpr big_int<2> zero = {{0UL, 0UL}};
  constexpr big_int<2> correct_answer = zero;

  constexpr auto result = mod_add(p, zero, p);

  REQUIRE(result == correct_answer);
  static_assert(result == correct_answer, "fail");
}

TEST_CASE("Multiplication") {

  using namespace cbn;
  SECTION("") {
    constexpr big_int<2> a = {{9223372036854753777UL, 0}};
    constexpr big_int<2> b = {{51461, 0}};
    constexpr big_int<4> res = {{9223372035721038517, 25730, 0, 0}};

    constexpr auto ans = mul(a, b);
    static_assert(res == ans, "fail");
  }
  SECTION("") {
    constexpr big_int<2> a = {{151, 23}};
    constexpr big_int<2> b = {{8712364, 832176}};
    constexpr big_int<4> res = {{1315566964, 326042948, 19140048, 0}};

    constexpr auto ans = mul(a, b);
    constexpr auto ans2 = mul2(a, b);
    static_assert(res == ans, "fail");
    static_assert(res == ans2, "fail");
  }
}

TEST_CASE("String Initialization") {

  using namespace cbn;
  auto s = BOOST_HANA_STRING("6513020836420374401749667047018991798096360820");
  constexpr auto num = string_to_big_int<4>(s);
  constexpr big_int<4> res = {{1315566964, 326042948, 19140048, 0}};

  REQUIRE(res == num);
  static_assert(res == num, "fail");
}

TEST_CASE("String Initialization -- auto length deduction") {

  using namespace cbn;
  auto s = BOOST_HANA_STRING("6513020836420374401749667047018991798096360820");
  constexpr auto num = string_to_big_int(s);

  constexpr big_int<3> res = {{1315566964, 326042948, 19140048}};

  REQUIRE(num == res);
  static_assert(num == res, "fail");
}


TEST_CASE("Barrett reduction") {

  using namespace cbn;

  constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING("1606938044258990275541962092341162602522202993782792835301611"));
  constexpr auto mu = string_to_big_int<5>(
      BOOST_HANA_STRING("834369935906605500935555353972481294766681454045567488"
                        "2604411090793790119337922481889828929536"));

  constexpr auto x = string_to_big_int<5>(
      BOOST_HANA_STRING("1725436586697640946858688965569256363112777243042596638790631055949891"
                        ));

  constexpr auto ans = string_to_big_int<4>(
      BOOST_HANA_STRING("1606938044258990275541962092341162602522202993782540505973038"));

  static_assert(barrett_reduction(x,prime,mu) == ans, "fail");
  REQUIRE(barrett_reduction(x,prime,mu) == ans);

}

TEST_CASE("Montgomery reduction") {
  using namespace cbn;
  constexpr auto modulus = string_to_big_int<2>(BOOST_HANA_STRING("1267650600228229401496703205653"));
  constexpr uint64_t mprime = 1265300135019788739UL;
  constexpr auto T = string_to_big_int<4>(BOOST_HANA_STRING("1532495540865888858358347027150309183618739122183602175"));
  constexpr auto ans = string_to_big_int<2>(BOOST_HANA_STRING("730531796855002292035529737298"));

  static_assert(montgomery_reduction(T,modulus,mprime) == ans, "fail");
  REQUIRE(montgomery_reduction(T,modulus,mprime) == ans);
}

TEST_CASE("Division") {
  using namespace cbn;

  constexpr auto u = string_to_big_int<6>(BOOST_HANA_STRING(
      "49252507745493099015348800125179517256349674088081808334935366755307"
      "15221437151326426783281860614455100828498788859"));
  constexpr auto v = string_to_big_int<4>(
      BOOST_HANA_STRING("144740111546645244279463731260859884816587480832050705"
                        "04932198000989141205031"));

  constexpr auto remainder = string_to_big_int<4>(
      BOOST_HANA_STRING("14474011154664524427946373126085988468387735773288470429860588311150180958754"));

  constexpr auto quotient = string_to_big_int<3>(
      BOOST_HANA_STRING("340282366920938463463374607431768211455"));
  



  constexpr auto ans = div(u,v);

  static_assert(ans.first == quotient, "fail");
  static_assert(ans.second == remainder, "fail");

  //static_assert(montgomery_reduction(T,modulus,mprime) == ans, "fail");
  //REQUIRE(montgomery_reduction(T,modulus,mprime) == ans);
}

TEST_CASE("") {

  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  auto modulus = conv<ZZ>("1606938044258990275541962092341162602522202993782792835301611");
  ZZ_p::init(modulus);


  ZZ_p x = conv<ZZ_p>(modulus-1);
  //print(x._ZZ_p__rep.size());


}

