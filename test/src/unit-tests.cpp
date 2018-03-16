#include "catch.hpp"

#include <boost/hana.hpp>

#include <boost/hana/ext/std/array.hpp>
#include <array>

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
#include <ctbignum/gcd.hpp>
#include <ctbignum/print.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/mod_exp.hpp>

#include <random>
#include<NTL/ZZ.h>
#include<NTL/ZZ_p.h>


//template <size_t N> using big_int = sprout::array<uint64_t, N>;
//template <size_t N> using big_int = std::array<uint64_t, N>;
using namespace std;


TEST_CASE("Addition") {

  using namespace cbn;

  constexpr big_int<2> a = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> b = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> p = {{181UL, 13835058055282163712UL}};

  constexpr big_int<2> correct_answer = {
      {18446744073709551435UL, 4611686018427387903UL}};

  //constexpr auto result_ugly_syntax =
  //    mod_add_<sprout::array, uint64_t, 2UL, 181UL, 13835058055282163712UL>(a,b);

  constexpr auto result_friendly_syntax = mod_add(a, b, p);

  //REQUIRE(result_ugly_syntax == correct_answer);
  REQUIRE(result_friendly_syntax == correct_answer);
  //static_assert(result_ugly_syntax == correct_answer, "fail");

  static_assert(result_friendly_syntax == correct_answer, "fail");
  //static_assert(result_friendly_syntax == correct_answer, "fail");

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

  /*
  SECTION("") {
    constexpr big_int<2> a = {{151, 23}};
    constexpr big_int<2> b = {{8712364, 832176}};
    constexpr big_int<4> res = {{1315566964, 326042948, 19140048, 0}};

    constexpr auto ans = knuth_mul(a, b);
    constexpr auto ans2 = knuth_mul(a, b);
    static_assert(res == ans, "fail");
    static_assert(res == ans2, "fail");
  }
  */

}

TEST_CASE("Squaring") {

  using namespace cbn;

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  auto trials = 100;

  //for (auto i = 0; i < trials; ++i) {

    big_int<4> x;
    // randomize
    auto sz = x.size();
    for (auto j = 0; j < sz; ++j)
      x[j] = distribution(generator);

    //print("mul: ",mul(x,x)); 
    //print("sq: ",square(x)); 
    REQUIRE(square(x) == mul(x, x));
  //}
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

TEST_CASE("String Initialization other base type -- auto length deduction") {

  using namespace cbn;
  auto s = BOOST_HANA_STRING("85070591730234618820156358408775751693");
  constexpr auto num = string_to_big_int<0, uint32_t>(s);
  // zero length means deduce automatically

  constexpr big_int<4, uint32_t> res = {{155801613, 659761661, 160, 1073741824}};

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
  
  auto mod = string_to_integer_seq(BOOST_HANA_STRING("1606938044258990275541962092341162602522202993782792835301611"));

  static_assert(barrett_reduction(x, mod) == ans, "fail");
  REQUIRE(barrett_reduction(x, mod) == ans);

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




TEST_CASE("Montgomery mult") {
  using namespace cbn;
  
  constexpr auto modulus = string_to_big_int(BOOST_HANA_STRING("1267650600228229401496703205653"));

  constexpr uint64_t mprime = 1265300135019788739UL;

  constexpr auto x = string_to_big_int(BOOST_HANA_STRING("924750812939937572408690850011"));
  constexpr auto y = string_to_big_int(BOOST_HANA_STRING("478633290783786461322094322310"));

  constexpr auto ans = montgomery_reduction(mul(x,y),modulus,mprime);


  static_assert(montgomery_mul(x, y, modulus, mprime) == ans, "fail");
  REQUIRE(montgomery_mul(x,y,modulus,mprime) == ans);
  

  auto modulus_seq = string_to_integer_seq(BOOST_HANA_STRING("1267650600228229401496703205653"));

  static_assert(montgomery_mul(x,y,modulus_seq) == ans);
  //static_assert(montgomery_mul2(x,y,modulus_seq) == ans);
}



TEST_CASE("Montgomery reduction - automatic precomputation") {
  using namespace cbn;
  
  constexpr auto modulus = string_to_integer_seq(BOOST_HANA_STRING("1267650600228229401496703205653"));
  constexpr auto T = string_to_big_int<4>(BOOST_HANA_STRING("1532495540865888858358347027150309183618739122183602175"));
  constexpr auto ans = string_to_big_int<2>(BOOST_HANA_STRING("730531796855002292035529737298"));

     static_assert(montgomery_reduction(T,modulus) == ans, "fail");
  //REQUIRE(montgomery_reduction(T,modulus,mprime) == ans);
}




TEST_CASE("Division") {
  using namespace cbn;

  constexpr auto u = string_to_big_int<6>(BOOST_HANA_STRING(
      "49252507745493099015348800125179517256349674088081808334935366755307"
      "15221437151326426783281860614455100828498788859"));
  constexpr auto v = string_to_big_int<4>(
      BOOST_HANA_STRING("144740111546645244279463731260859884816587480832050705"
                        "04932198000989141205031"));

  constexpr auto rem = string_to_big_int<4>(
      BOOST_HANA_STRING("14474011154664524427946373126085988468387735773288470429860588311150180958754"));

  constexpr auto quot = string_to_big_int<3>(
      BOOST_HANA_STRING("340282366920938463463374607431768211455"));
  



  constexpr auto ans = div(u,v);

  static_assert(detail::first<3>(ans.first) == quot, "fail");
  static_assert(ans.second == rem, "fail");

  //static_assert(montgomery_reduction(T,modulus,mprime) == ans, "fail");
  //REQUIRE(montgomery_reduction(T,modulus,mprime) == ans);
}

TEST_CASE("short div") {

  using namespace cbn;

  constexpr auto a = string_to_big_int(BOOST_HANA_STRING("12103081107736073677280037"));
  constexpr auto b = string_to_big_int(BOOST_HANA_STRING("2893462387"));
      
  constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("4182905975247458"));
                                                       //   418290597524622793977

  constexpr auto quotrem = cbn::short_div(a,b[0]);

  static_assert(quotrem.first[0] == 4182905975247458, "fail");
  static_assert(quotrem.second[0] == 936917791, "fail");

  REQUIRE(quotrem.first[0] == 4182905975247458);
  REQUIRE(quotrem.second[0] ==936917791);

}


TEST_CASE("gcd") {

  using namespace cbn;

  static constexpr auto a = string_to_integer_seq(BOOST_HANA_STRING("1210308110773251360736775280037"));
  static constexpr auto b = string_to_integer_seq(BOOST_HANA_STRING("91726531791233920914026205331"));
      
  constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("1505621586711374587419632790"));
  

  constexpr auto gcd = cbn::ext_gcd(a,b); 
  
  //,std::make_integer_sequence<uint64_t,2>{});
  //constexpr auto gcd = cbn::ext_gcd(a,b);

  //auto N = a.size();
  constexpr auto N = 2;
  constexpr auto modinv = detail::take<N,2*N>(gcd);
  static_assert(modinv == ans, "fail");

}

/*
TEST_CASE("gcd easy") {

  using namespace cbn;

  static constexpr auto a = string_to_integer_seq(BOOST_HANA_STRING("1210308110773251360736775280037"));
  static constexpr auto b = string_to_integer_seq(BOOST_HANA_STRING("91726531791233920914026205331"));
      
  constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("1505621586711374587419632790"));
  

  constexpr auto gcd = cbn::ext_gcd_(a,b); 
  
  //,std::make_integer_sequence<uint64_t,2>{});
  //constexpr auto gcd = cbn::ext_gcd(a,b);

  //auto N = a.size();
  constexpr auto N = 2;
  constexpr auto modinv = detail::take<N,2*N>(gcd);
  static_assert(modinv == ans, "fail");

}
*/



TEST_CASE("modular inverse") {

  using namespace cbn;

  constexpr auto x = string_to_integer_seq(BOOST_HANA_STRING("1210308110773251360736775280037"));
  auto m = string_to_integer_seq(BOOST_HANA_STRING("91726531791233920914026205331"));
      
  constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("1505621586711374587419632790"));

  static_assert(cbn::mod_inv(x,m) == ans, "fail");

}


TEST_CASE("arrayconv") {

  using namespace cbn;
  //static constexpr std::array<size_t, 4> a {{1, 3, 5, 6}};
  //constexpr auto s = 

  constexpr auto x = string_to_big_int<5>(
      BOOST_HANA_STRING("1725436586697640946858688965569256363112777243042596638790631055949891"
                        ));

  constexpr auto ans = string_to_big_int<4>(
      BOOST_HANA_STRING("1606938044258990275541962092341162602522202993782540505973038"));

  //static_assert(barrett_reduction(x,prime,mu) == ans, "fail");
  //REQUIRE(barrett_reduction(x,prime,mu) == ans);
  
  auto modulus = string_to_integer_seq(BOOST_HANA_STRING("1606938044258990275541962092341162602522202993782792835301611"));

 // static_assert(barrett_reduction(x,modulus) == ans, "fail");
  REQUIRE(barrett_reduction(x,modulus) == ans);

  //static_assert(barrett_reduction<235, 0, 0, 256>(x) == ans, "fail");
  //REQUIRE(barrett_reduction<235, 0, 0, 256>(x) == ans);

}

TEST_CASE("Modular Exponentiation") {

  using namespace cbn;

  //constexpr auto x = string_to_big_int(BOOST_HANA_STRING("8720319859187456713659817365803476381756813759"));
  constexpr auto x = string_to_big_int(BOOST_HANA_STRING("123512321638732781541098374832654"));
  constexpr auto e = string_to_big_int(BOOST_HANA_STRING("1180591620739245727853"));
  constexpr auto m = string_to_integer_seq(BOOST_HANA_STRING("85070591730234618820156358408775751693"));
  //constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("74509724535899236211803247263430915052"));
  constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("65447949695390573931730737899088862792"));

  //constexpr auto x = string_to_big_int(BOOST_HANA_STRING("2"));
  //constexpr auto e = string_to_big_int(BOOST_HANA_STRING("21"));
  //constexpr auto m = string_to_integer_seq(BOOST_HANA_STRING("1048583"));
  //constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("1048569"));
  //
  //
  //
  
  //constexpr auto ans = string_to_big_int(BOOST_HANA_STRING("57840141081826923106833721816893554682"));


  static_assert(cbn::mod_exp(x,e,m) == ans, "fail");
  REQUIRE(cbn::mod_exp(x,e,m) == ans);


  //static_assert(cbn::mod_exp_montgomery(x,e,m) == ans, "fail");
  //REQUIRE(cbn::mod_exp_montgomery(x,e,m) == ans);

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

