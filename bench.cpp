#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <benchmark/benchmark.h>
#include <random>
#include <sprout/array.hpp>

#include "all.hpp"

template <size_t N> using big_int = sprout::array<uint64_t, N>;

static void modadd(benchmark::State &state) {
  constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING(
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
    benchmark::DoNotOptimize(mod_add(x, y, prime));
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
  constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));
  constexpr auto mu = string_to_big_int<5>(
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
    auto j = mul(x, y);
    auto k = barrett_reduction(j, prime, mu);
    benchmark::DoNotOptimize(k);
  }
}
static void modmul2(benchmark::State &state) {
  constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));
  constexpr auto mu = string_to_big_int<5>(
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
    benchmark::DoNotOptimize(barrett_reduction(mul2(x, y), prime, mu));
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
  constexpr auto prime = string_to_big_int<4>(BOOST_HANA_STRING(
      "1606938044258990275541962092341162602522202993782792835301611"));
  constexpr auto mu = string_to_big_int<8>(
      BOOST_HANA_STRING("834369935906605500935555353972481294766681454045567488"
                        "2604411090793790119337922481889828929536"));

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<5> x;

  for (int i = 0; i < 5; ++i) {
    x[i] = distribution(generator);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(barrett_reduction(x, prime, mu));
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
    auto k = mul(x, y);
    benchmark::DoNotOptimize(k);
  }
}
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
    auto k = mul2(x, y);
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






// BENCHMARK(creation);
// BENCHMARK(creation_ntl);
BENCHMARK(modadd);
BENCHMARK(modadd_ntl);
BENCHMARK(modadd_immediate);
BENCHMARK(modadd_immediate_ntl);

BENCHMARK(mul_immediate);
BENCHMARK(mul2_immediate);
BENCHMARK(mul_immediate_ntl);

BENCHMARK(mul_);
BENCHMARK(mul2_);
BENCHMARK(mul_ntl);



BENCHMARK(modmul);
BENCHMARK(modmul2);
BENCHMARK(modmul_ntl);
BENCHMARK(reduce);
BENCHMARK(reduce_ntl);
BENCHMARK(big_int_from_string);
BENCHMARK(big_int_from_string_ntl);

BENCHMARK_MAIN();
