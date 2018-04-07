#include "catch.hpp"

#include <ctbignum/addition.hpp>
#include <ctbignum/barrett.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/division.hpp>
#include <ctbignum/field.hpp>
#include <ctbignum/gcd.hpp>
#include <ctbignum/initialization.hpp>
#include <ctbignum/io.hpp>
#include <ctbignum/mod_exp.hpp>
#include <ctbignum/montgomery.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/print.hpp>
#include <ctbignum/relational_ops.hpp>
#include <ctbignum/slicing.hpp>

#include <iostream>
#include <random>

using namespace std;

TEST_CASE("Addition") {

  using namespace cbn;

  constexpr big_int<2> a = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> b = {{0UL, 9223372036854775808UL}};
  constexpr big_int<2> p = {{181UL, 13835058055282163712UL}};

  constexpr big_int<2> correct_answer = {
      {18446744073709551435UL, 4611686018427387903UL}};

  constexpr auto result_friendly_syntax = mod_add(a, b, p);

  REQUIRE(result_friendly_syntax == correct_answer);
  static_assert(result_friendly_syntax == correct_answer, "fail");
}

TEST_CASE("subtraction") {

  using namespace cbn;
  using cbn::detail::first;
  using cbn::detail::unary_encoding;

  constexpr big_int<2> a = {{15, 0}};
  constexpr big_int<2> b = {{10, 0}};
  constexpr big_int<2> d = {{6, 0}};

  constexpr big_int<2> c = {{5, 0}};

  static_assert(subtract_ignore_carry(a,b) == c, "fail");
  
  constexpr auto res = subtract(b,a);
  
  static_assert(res[2], "fail");

  static_assert(add_ignore_carry(first<2>(res),d) == unary_encoding<0,2>(),"");

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
    static_assert(res == ans, "fail");
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

/*
TEST_CASE("Squaring") {

  using namespace cbn;

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  auto trials = 100;

  for (auto i = 0; i < trials; ++i) {

    big_int<4> x;
    // randomize
    auto sz = x.size();
    for (auto j = 0; j < sz; ++j)
      x[j] = distribution(generator);

    //print("mul: ",mul(x,x)); 
    //print("sq: ",square(x)); 
    print ("iteration: ", i);
    print ("x: ", x);
    print ("sq: ", square(x));
    print ("mul:", mul(x,x));
    REQUIRE(square(x) == mul(x, x));
  }
}
*/

TEST_CASE("String Initialization") {

  using namespace cbn;
  
  constexpr auto num = to_big_int(6513020836420374401749667047018991798096360820_Z);
  constexpr big_int<3> res = {1315566964, 326042948, 19140048};

  REQUIRE(res == num);
  static_assert(res == num, "fail");
}


/*
TEST_CASE("String Initialization other base type") {

  using namespace cbn;
  auto s = BOOST_HANA_STRING("85070591730234618820156358408775751693");
  constexpr auto num = string_to_big_int<0, uint32_t>(s);
  // zero length means deduce automatically

  constexpr big_int<4, uint32_t> res = {{155801613, 659761661, 160, 1073741824}};

  REQUIRE(num == res);
  static_assert(num == res, "fail");
}
*/

TEST_CASE("String output") {

  using namespace cbn;
  constexpr auto num = to_big_int(85070591730234618820156358408775751693_Z);

  std::stringstream ss;
  ss << num;
   
  REQUIRE(ss.str() == "85070591730234618820156358408775751693");
}

TEST_CASE("Barrett reduction") {

  using namespace cbn;

  constexpr auto prime = to_big_int(1606938044258990275541962092341162602522202993782792835301611_Z);
  constexpr auto mu = to_big_int(8343699359066055009355553539724812947666814540455674882604411090793790119337922481889828929536_Z);
  constexpr auto x = to_big_int<5>(1725436586697640946858688965569256363112777243042596638790631055949891_Z);
  constexpr auto ans = to_big_int(1606938044258990275541962092341162602522202993782540505973038_Z);

  static_assert(barrett_reduction(x,prime,mu) == ans, "fail");
  REQUIRE(barrett_reduction(x,prime,mu) == ans);
  
  auto mod = 1606938044258990275541962092341162602522202993782792835301611_Z;

  static_assert(barrett_reduction(x, mod) == ans, "fail");
  REQUIRE(barrett_reduction(x, mod) == ans);

}

TEST_CASE("Montgomery reduction") {
  using namespace cbn;
  constexpr auto modulus = to_big_int(1267650600228229401496703205653_Z);
  constexpr uint64_t mprime = 1265300135019788739UL;
  constexpr auto T = to_big_int<4>(1532495540865888858358347027150309183618739122183602175_Z);
  constexpr auto ans = to_big_int(730531796855002292035529737298_Z);

  static_assert(montgomery_reduction(T,modulus,mprime) == ans, "fail");
  REQUIRE(montgomery_reduction(T,modulus,mprime) == ans);
}




TEST_CASE("Montgomery mult") {
  using namespace cbn;
  
  constexpr auto modulus = to_big_int(1267650600228229401496703205653_Z);

  constexpr uint64_t mprime = 1265300135019788739UL;

  constexpr auto x = to_big_int(924750812939937572408690850011_Z);
  constexpr auto y = to_big_int(478633290783786461322094322310_Z);

  constexpr auto ans = montgomery_reduction(mul(x,y),modulus,mprime);


  static_assert(montgomery_mul(x, y, modulus, mprime) == ans, "fail");
  REQUIRE(montgomery_mul(x,y,modulus,mprime) == ans);
  

  auto modulus_seq = 1267650600228229401496703205653_Z;

  static_assert(montgomery_mul(x,y,modulus_seq) == ans);
  //static_assert(montgomery_mul2(x,y,modulus_seq) == ans);
}

TEST_CASE("Montgomery mult template deduction") {
  using namespace cbn;
  big_int<4> x;
  big_int<4> y;
  big_int<4> modulus;
  montgomery_mul(x, y, modulus, (int) 1);
  montgomery_reduction(mul(x, y), modulus, (uint8_t) 1);
}

TEST_CASE("Montgomery reduction - automatic precomputation") {
  using namespace cbn;

  constexpr auto modulus = 1267650600228229401496703205653_Z;
  constexpr auto T = to_big_int<4>(1532495540865888858358347027150309183618739122183602175_Z);
  constexpr auto ans = to_big_int(730531796855002292035529737298_Z);

  static_assert(montgomery_reduction(T, modulus) == ans, "fail");
  // REQUIRE(montgomery_reduction(T,modulus,mprime) == ans);
}

TEST_CASE("Division") {
  using namespace cbn;

  constexpr auto u = to_big_int(4925250774549309901534880012517951725634967408808180833493536675530715221437151326426783281860614455100828498788859_Z);
  constexpr auto v = to_big_int(14474011154664524427946373126085988481658748083205070504932198000989141205031_Z);

  constexpr auto rem = to_big_int(14474011154664524427946373126085988468387735773288470429860588311150180958754_Z);

  constexpr auto quot = to_big_int(340282366920938463463374607431768211455_Z);
  



  constexpr auto ans = div(u,v);

  static_assert(detail::first<3>(ans.first) == quot, "fail");
  static_assert(ans.second == rem, "fail");

  //static_assert(montgomery_reduction(T,modulus,mprime) == ans, "fail");
  //REQUIRE(montgomery_reduction(T,modulus,mprime) == ans);
}

TEST_CASE("short div") {

  using namespace cbn;

  constexpr auto a = to_big_int(12103081107736073677280037_Z);
  constexpr auto b = to_big_int(2893462387_Z);
      
  constexpr auto ans = to_big_int(4182905975247458_Z);
                                                       //   418290597524622793977

  constexpr auto quotrem = cbn::short_div(a,b[0]);

  static_assert(quotrem.first[0] == 4182905975247458, "fail");
  static_assert(quotrem.second[0] == 936917791, "fail");

  REQUIRE(quotrem.first[0] == 4182905975247458);
  REQUIRE(quotrem.second[0] ==936917791);

}


TEST_CASE("gcd") {

  using namespace cbn;

  static constexpr auto a = 1210308110773251360736775280037_Z;
  static constexpr auto b = 91726531791233920914026205331_Z;
      
  constexpr auto ans = to_big_int(1505621586711374587419632790_Z);
  

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

  static constexpr auto a = 1210308110773251360736775280037_Z;
  static constexpr auto b = 91726531791233920914026205331_Z;
      
  constexpr auto ans = to_big_int(1505621586711374587419632790_Z);
  

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

  constexpr auto x = 1210308110773251360736775280037_Z;
  auto m = 91726531791233920914026205331_Z;
      
  constexpr auto ans = to_big_int(1505621586711374587419632790_Z);

  static_assert(cbn::mod_inv(x,m) == ans, "fail");

}


TEST_CASE("arrayconv") {

  using namespace cbn;
  //static constexpr std::array<size_t, 4> a {{1, 3, 5, 6}};
  //constexpr auto s = 

  constexpr auto x = to_big_int(1725436586697640946858688965569256363112777243042596638790631055949891_Z);

  constexpr auto ans = to_big_int(1606938044258990275541962092341162602522202993782540505973038_Z);

  //static_assert(barrett_reduction(x,prime,mu) == ans, "fail");
  //REQUIRE(barrett_reduction(x,prime,mu) == ans);
  
  auto modulus = 1606938044258990275541962092341162602522202993782792835301611_Z;

 // static_assert(barrett_reduction(x,modulus) == ans, "fail");
  REQUIRE(barrett_reduction(x,modulus) == ans);

  //static_assert(barrett_reduction<235, 0, 0, 256>(x) == ans, "fail");
  //REQUIRE(barrett_reduction<235, 0, 0, 256>(x) == ans);

}

TEST_CASE("Modular Exponentiation") {

  using namespace cbn;

  //constexpr auto x = to_big_int(8720319859187456713659817365803476381756813759_Z);
  constexpr auto x = to_big_int(123512321638732781541098374832654_Z);
  constexpr auto e = to_big_int(1180591620739245727853_Z);
  constexpr auto m = 85070591730234618820156358408775751693_Z;
  //constexpr auto ans = to_big_int(74509724535899236211803247263430915052_Z);
  constexpr auto ans = to_big_int(65447949695390573931730737899088862792_Z);

  //constexpr auto x = to_big_int(2_Z);
  //constexpr auto e = to_big_int(21_Z);
  //constexpr auto m = 1048583_Z;
  //constexpr auto ans = to_big_int(1048569_Z);
  //
  //
  //
  
  //constexpr auto ans = to_big_int(57840141081826923106833721816893554682_Z);


  static_assert(cbn::mod_exp(x,e,m) == ans, "fail");
  REQUIRE(cbn::mod_exp(x,e,m) == ans);


  //static_assert(cbn::mod_exp_montgomery(x,e,m) == ans, "fail");
  //REQUIRE(cbn::mod_exp_montgomery(x,e,m) == ans);

}

TEST_CASE("summation") {

  using namespace cbn;
  auto modulus = 85070591730234618820156358408775751693_Z;
  using F = decltype(Zq(modulus));
  F a{to_big_int(300_Z)};
  F b{to_big_int(450_Z)};

  auto c = a + b;

  print("sum: ", c.data);
}
