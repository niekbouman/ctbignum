#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <benchmark/benchmark.h>
#include <random>
#include <sprout/array.hpp>

#include <ctbignum/all.hpp>
//#include <ctbignum/bigint.hpp>

template <size_t N> using big_int = sprout::array<uint64_t, N>;

static void modadd(benchmark::State &state) {
  constexpr auto prime = cbn::string_to_big_int<4>(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<4> x;
  big_int<4> y;

  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(cbn::mod_add(x, y, prime));
  }
}

static void modadd_ntl(benchmark::State &state) {
  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  auto modulus = conv<ZZ>("1606938044258990275541962092341162602522202993782792835301611");
  ZZ_p::init(modulus);

  ZZ_p x = NTL::random_ZZ_p();
  ZZ_p y = NTL::random_ZZ_p();
  ZZ_p z;

  for (auto _ : state) {
    add(z,x,y);
    benchmark::DoNotOptimize(z);
  }
}


static void modmul(benchmark::State &state) {
  constexpr auto prime = cbn::string_to_index_seq(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));
  //constexpr auto mu = cbn::string_to_big_int<5>(
  //    BOOST_HANA_STRING("834369935906605500935555353972481294766681454045567488"
  //                      "2604411090793790119337922481889828929536"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<4> x;
  big_int<4> y;

  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto j = cbn::mul(x, y);
    auto k = cbn::barrett_reduction(j, prime); //, mu);
    benchmark::DoNotOptimize(k);
  }
}

class MyFixture : public ::benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State &state) {

    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(0);

    data.reserve(8 * 1000);

    for (auto limb : data)
      limb = distribution(generator);
  }

  void TearDown(const ::benchmark::State &state) { data.clear(); }

  ~MyFixture() {}

  std::vector<uint64_t> data;
};

/*
BENCHMARK_F(MyFixture, fixture_mul)(benchmark::State &st) {

  constexpr auto prime = cbn::string_to_index_seq(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));

  size_t i = 0;
  auto base_ptr = data.data();

  for (auto _ : st) {

    auto x = reinterpret_cast<big_int<4>*>(base_ptr + i);
    auto y = reinterpret_cast<big_int<4>*>(base_ptr + i + 4);

    auto j = cbn::mul(*x, *y);
    auto k = cbn::barrett_reduction(j, prime);
    benchmark::DoNotOptimize(k);

    i += 8;
    if (i == 8000)
      i = 0;
  }
}
*/

static void modmul2(benchmark::State &state) {
  constexpr auto prime = cbn::string_to_big_int<4>(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));
  constexpr auto mu = cbn::string_to_big_int<5>(
      BOOST_HANA_STRING("834369935906605500935555353972481294766681454045567488"
                        "2604411090793790119337922481889828929536"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<4> x;
  big_int<4> y;

  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(cbn::barrett_reduction(cbn::mul2(x, y), prime, mu));
  }
}

static void modmul_ntl(benchmark::State &state) {
  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  auto modulus =
      conv<ZZ>("1606938044258990275541962092341162602522202993782792835301611");
  ZZ_p::init(modulus);

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  ZZ_p x, y, z;
  x = NTL::random_ZZ_p();
  y = NTL::random_ZZ_p();

  for (auto _ : state) {
    mul(z,x,y);
    benchmark::DoNotOptimize(z);
  }
}

static void reduce(benchmark::State &state) {
  using namespace cbn;

  //constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING(
  //    "1606938044258990275541962092341162602522202993782792835301611"));
  //constexpr auto mu = string_to_big_int<8>(
  //    BOOST_HANA_STRING("834369935906605500935555353972481294766681454045567488"
  //                      "2604411090793790119337922481889828929536"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<5> x;

  for (int i = 0; i < 5; ++i) {
    x[i] = distribution(generator);
  }
  // precompute mu
  
  //constexpr auto z =  detail::unary_encoding<8,9>();
  //constexpr auto quot_rem  = div(z,prime);
  //constexpr auto quot_rem  = div(detail::unary_encoding<2*N2,2*N2+1>(),modulus);
  //constexpr auto mu = quot_rem.first;

  for (auto _ : state) {
    //benchmark::DoNotOptimize(barrett_reduction(x, prime, mu));
    benchmark::DoNotOptimize(barrett_reduction<235, 0, 0, 256>(x));
  }
}

static void reduce_intseq(benchmark::State &state) {
  using namespace cbn;

  //constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING(
  //    "1606938044258990275541962092341162602522202993782792835301611"));
  //constexpr auto mu = string_to_big_int<8>(
  //    BOOST_HANA_STRING("834369935906605500935555353972481294766681454045567488"
  //                      "2604411090793790119337922481889828929536"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<5> x;

  for (int i = 0; i < 5; ++i) {
    x[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto modulus = string_to_index_seq(BOOST_HANA_STRING("1606938044258990275541962092341162602522202993782792835301611"));
    benchmark::DoNotOptimize(barrett_reduction(x,modulus));
  }
}


static void reduce_ntl(benchmark::State &state) {
  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  auto modulus =
      conv<ZZ>("1606938044258990275541962092341162602522202993782792835301611");
  ZZ_p::init(modulus);
  ZZ y = NTL::RandomBits_ZZ(320);

  for (auto _ : state) {
    auto z = conv<ZZ_p>(y);
    benchmark::DoNotOptimize(z);
  }
}

static void big_int_from_string_ntl(benchmark::State &state) {
  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  for (auto _ : state) {
    auto x = conv<ZZ>(
        "1606938044258990275541962092341162602522202993782792835301611");
    benchmark::DoNotOptimize(x);
  }
}
static void big_int_from_string(benchmark::State &state) {
  using namespace cbn;
  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  for (auto _ : state) {
    auto x = string_to_big_int<4>(BOOST_HANA_STRING(
        "1606938044258990275541962092341162602522202993782792835301611"));
    benchmark::DoNotOptimize(x);
  }
}

static void modadd_immediate(benchmark::State &state) {
  using namespace cbn;
  constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));

  auto y = string_to_big_int<4>(BOOST_HANA_STRING(
      "1000431856504897268487649876408467273524360183457791240097764"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  // big_int<4> x;
  big_int<4> x;

  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto k = mod_add(x, y, prime);
    benchmark::DoNotOptimize(k);
  }
}

static void modadd_immediate_ntl(benchmark::State &state) {

  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  auto modulus =
      conv<ZZ>("1606938044258990275541962092341162602522202993782792835301611");

  ZZ_p::init(modulus);

  auto y = conv<ZZ_p>(
      "1000431856504897268487649876408467273524360183457791240097764");
  //{ NTL::random() };

  // preallocate
  ZZ_p x = conv<ZZ_p>(modulus - 1);
  // ZZ_p y = conv<ZZ_p>(modulus-1);

  x = NTL::random_ZZ_p();
  // y = NTL::random_ZZ_p();

  ZZ_p z;
  for (auto _ : state) {
    /*
    state.PauseTiming();

    auto p = reinterpret_cast<uint64_t*>(x._ZZ_p__rep.rep.rep);
    for (int i=0; i<4; ++i) {
      p[2+i] = distribution(generator);
    }


    //x = NTL::random_ZZ_p();
    //y = NTL::random_ZZ_p();

    state.ResumeTiming();
*/
    //auto z = x + y;
    add(z,x,y);
    benchmark::DoNotOptimize(z);
  }
}

static void mul_immediate(benchmark::State &state) {
  using namespace cbn;

  auto y = string_to_big_int<4>(BOOST_HANA_STRING(
      "1000431856504897268487649876408467273524360183457791240097764"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  // big_int<4> x;
  big_int<4> x;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
  }

  for (auto _ : state) {
    /*
    state.PauseTiming();

    for (int i=0; i<4; ++i) {
      x[i] = distribution(generator);
    }

    state.ResumeTiming();
*/
    auto k = mul(x, y);
    benchmark::DoNotOptimize(k);
  }
}
static void mul2_immediate(benchmark::State &state) {
  using namespace cbn;

  auto y = string_to_big_int<4>(BOOST_HANA_STRING(
      "1000431856504897268487649876408467273524360183457791240097764"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  // big_int<4> x;
  big_int<4> x;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
  }

  for (auto _ : state) {
    /*
    state.PauseTiming();

    for (int i=0; i<4; ++i) {
      x[i] = distribution(generator);
    }

    state.ResumeTiming();
*/
    auto k = mul2(x, y);
    benchmark::DoNotOptimize(k);
  }
}

static void mul_immediate_ntl(benchmark::State &state) {

  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  auto modulus =
      conv<ZZ>("1606938044258990275541962092341162602522202993782792835301611");

  auto y =
      conv<ZZ>("1000431856504897268487649876408467273524360183457791240097764");
  ZZ x = conv<ZZ>(modulus - 1);
  ZZ z;

  auto p = reinterpret_cast<uint64_t *>(x.rep.rep);
  for (int i = 0; i < 4; ++i) {
    p[2 + i] = distribution(generator);
  }

  for (auto _ : state) {
    /*
    state.PauseTiming();

    for (int i=0; i<4; ++i) {
      p[2+i] = distribution(generator);
    }

    state.ResumeTiming();
*/
    mul( z,  x , y);
    benchmark::DoNotOptimize(z);
  }
}


static void mul_(benchmark::State &state) {

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<4> x;
  big_int<4> y;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto k = cbn::mul(x, y);
    benchmark::DoNotOptimize(k);
  }
}

/*
static void knuthmul_(benchmark::State &state) {

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<4> x;
  big_int<4> y;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto k = cbn::knuth_mul(x, y);
    benchmark::DoNotOptimize(k);
  }
}
*/


static void mul2_(benchmark::State &state) {

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<4> x;
  big_int<4> y;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto k = cbn::mul2(x, y);
    benchmark::DoNotOptimize(k);
  }
}


static void mul_ntl(benchmark::State &state) {

  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  auto x = NTL::RandomBits_ZZ(4*64);
  auto y = NTL::RandomBits_ZZ(4*64);
  ZZ z;

  for (auto _ : state) {
    mul(z, x, y);
    benchmark::DoNotOptimize(z);
  }
}


static void montmul(benchmark::State &state) {

  auto prime = cbn::string_to_big_int(
          BOOST_HANA_STRING("14474011154664524427946373126085988481658748083205"
                            "070504932198000989141205031"));

  auto inv = 10405855631323336809UL;
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);


  big_int<4> x;
  big_int<4> y;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto z = cbn::montgomery_mul(x, y, prime, inv);
    benchmark::DoNotOptimize(z);
  }
}


static void montmul_auto(benchmark::State &state) {

  auto prime = cbn::string_to_index_seq(
          BOOST_HANA_STRING("14474011154664524427946373126085988481658748083205"
                            "070504932198000989141205031"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);


  big_int<4> x;
  big_int<4> y;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto z = cbn::montgomery_mul(x, y, prime);
    benchmark::DoNotOptimize(z);
  }
}

/*
static void montmul_auto2(benchmark::State &state) {

  auto prime = cbn::string_to_index_seq(
          BOOST_HANA_STRING("14474011154664524427946373126085988481658748083205"
                            "070504932198000989141205031"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);


  big_int<4> x;
  big_int<4> y;
  for (int i = 0; i < 4; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto z = cbn::montgomery_mul2(x, y, prime);
    benchmark::DoNotOptimize(z);
  }
}
*/




//#include "modulus.hpp"
#include <libff/algebra/fields/fp.hpp>
#include <libff/algebra/fields/bigint.hpp>

static void montmul_libff(benchmark::State &state) {

  using namespace libff;
  //extern bigint<4L> mymodulus;
  static auto mymodulus = libff::bigint<static_cast<mp_size_t>(4)>("14474011154664524427946373126085988481658748083205070504932198000989141205031");

  using GF253 = Fp_model<4L, mymodulus>;

  GF253::euler = bigint<static_cast<mp_size_t>(4)>("723700557733226221397318656"
                                                   "304299424082937404160253525"
                                                   "2466099000494570602515");
  GF253::t = bigint<static_cast<mp_size_t>(4)>("7237005577332262213973186563042"
                                               "9942408293740416025352524660990"
                                               "00494570602515");
  GF253::t_minus_1_over_2 = bigint<static_cast<mp_size_t>(4)>(
      "361850278866613110698659328152149712041468702080126762623304950024728530"
      "1257");
  GF253::Rsquared = bigint<static_cast<mp_size_t>(4)>("97344");
  GF253::Rcubed = bigint<static_cast<mp_size_t>(4)>(
      "144740111546645244279463731260859884816587480832050705049321980009891108"
      "33703");
  GF253::s = 1;
  GF253::num_bits = 253;
  GF253::inv = 10405855631323336809UL;

  // auto x = NTL::RandomBits_ZZ(4*64);
  // auto y = NTL::RandomBits_ZZ(4*64);
  // ZZ z;

  GF253 x, y, z;

  x.random_element();
  y.random_element();

  for (auto _ : state) {
    z = x * y;
    benchmark::DoNotOptimize(z);
  }
}

static void mont_reduction(benchmark::State &state) {


  auto prime = cbn::string_to_big_int(
          BOOST_HANA_STRING("14474011154664524427946373126085988481658748083205"
                            "070504932198000989141205031"));


  auto inv = 10405855631323336809UL;

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<8> x;
  for (int i = 0; i < 8; ++i) 
    x[i] = distribution(generator);


  for (auto _ : state) {
    auto z = cbn::montgomery_reduction(x, prime, inv);
    benchmark::DoNotOptimize(z);
  }
}

/*
static void mont_reduction2(benchmark::State &state) {


  auto prime = cbn::string_to_index_seq(
          BOOST_HANA_STRING("14474011154664524427946373126085988481658748083205"
                            "070504932198000989141205031"));


  auto inv = 10405855631323336809UL;

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<8> x;
  for (int i = 0; i < 8; ++i) 
    x[i] = distribution(generator);


  for (auto _ : state) {
    auto z = cbn::montgomery_reduction2(x, prime);
    benchmark::DoNotOptimize(z);
  }
}
*/

static void mont_reduction_auto(benchmark::State &state) {


  auto prime = cbn::string_to_index_seq(
          BOOST_HANA_STRING("14474011154664524427946373126085988481658748083205"
                            "070504932198000989141205031"));


  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<8> x;
  for (int i = 0; i < 8; ++i) 
    x[i] = distribution(generator);


  for (auto _ : state) {
    auto z = cbn::montgomery_reduction(x, prime);
    benchmark::DoNotOptimize(z);
  }
}

/*
static void mont_reduction_auto2(benchmark::State &state) {


  auto prime = cbn::string_to_index_seq(
          BOOST_HANA_STRING("14474011154664524427946373126085988481658748083205"
                            "070504932198000989141205031"));


  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<8> x;
  for (int i = 0; i < 8; ++i) 
    x[i] = distribution(generator);


  for (auto _ : state) {
    auto z = cbn::montgomery_reduction2(x, prime);
    benchmark::DoNotOptimize(z);
  }
}
*/



// BENCHMARK(creation);
// BENCHMARK(creation_ntl);

/*
 
BENCHMARK(modadd);
BENCHMARK(modadd_ntl);
BENCHMARK(modadd_immediate);
BENCHMARK(modadd_immediate_ntl);

BENCHMARK(mul_immediate);
BENCHMARK(mul2_immediate);
BENCHMARK(mul_immediate_ntl);

BENCHMARK(mul_);
//BENCHMARK(knuthmul_);
BENCHMARK(mul2_);
BENCHMARK(mul_ntl);



BENCHMARK(modmul);
BENCHMARK(modmul2);
BENCHMARK(modmul_ntl);


BENCHMARK(montmul);
BENCHMARK(montmul_auto);
//BENCHMARK(montmul_auto2);
BENCHMARK(montmul_libff);
//BENCHMARK(mont_reduction_auto2);
BENCHMARK(big_int_from_string);
BENCHMARK(big_int_from_string_ntl);

BENCHMARK(modmul);
BENCHMARK(modmul2);
BENCHMARK(modmul_ntl);

//BENCHMARK(montmul);
BENCHMARK(montmul_auto);
//BENCHMARK(montmul_auto2);
BENCHMARK(montmul_libff);


BENCHMARK(montmul);
BENCHMARK(montmul_auto);
BENCHMARK(montmul_libff);

BENCHMARK(mont_reduction);
//BENCHMARK(mont_reduction2);
BENCHMARK(mont_reduction_auto);
*/

BENCHMARK(reduce);
BENCHMARK(reduce_intseq);
BENCHMARK(reduce_ntl);

BENCHMARK_MAIN();
