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
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <benchmark/benchmark.h>
#include <random>

#include <ctbignum/bigint.hpp>
#include <ctbignum/decimal_literals.hpp>
#include <ctbignum/mod_exp.hpp>

#include <libff/algebra/fields/fp.hpp>
#include <libff/algebra/fields/bigint.hpp>



static void modexp_ntl(benchmark::State &state) {
  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  auto modulus =
      conv<ZZ>("1606938044258990275541962092341162602522202993782792835301611");
  ZZ_p::init(modulus);

  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  ZZ_p z; 
  ZZ exp = conv<ZZ>("5208756711089370345167341923545687104");

  ZZ_p a = conv<ZZ_p>( conv<ZZ>("43234613467152613512549871563467271263417258763487658172645"));

  //a = NTL::random_ZZ_p();
  //exp = NTL::RandomBits_ZZ(100);

  for (auto _ : state) {
    NTL::power(z,a,exp);
    benchmark::DoNotOptimize(z);
  }
}


static void modexp_mont(benchmark::State &state) {

  using namespace cbn;

  auto modulus =
      1606938044258990275541962092341162602522202993782792835301611_Z;

  auto a = to_big_int(43234613467152613512549871563467271263417258763487658172645_Z);
 
  auto exp = to_big_int(5208756711089370345167341923545687104_Z);


  //a = NTL::random_ZZ_p();
  //exp = NTL::RandomBits_ZZ(100);

  for (auto _ : state) {
    auto z = mod_exp(a,exp,modulus);
    benchmark::DoNotOptimize(z);
  }
}

static auto mymodulus = libff::bigint<4>("1606938044258990275541962092341162602522202993782792835301611");

static void modexp_libff(benchmark::State &state) {
  using namespace libff;
  using GF200 = Fp_model<4L, mymodulus>;
  GF200::euler = bigint<static_cast<mp_size_t>(4)>("803469022129495137770981046170581301261101496891396417650805");
  GF200::t = bigint<static_cast<mp_size_t>(4)>("803469022129495137770981046170581301261101496891396417650805");
  GF200::t_minus_1_over_2 = bigint<static_cast<mp_size_t>(4)>("401734511064747568885490523085290650630550748445698208825402");
  GF200::Rsquared = bigint<static_cast<mp_size_t>(4)>("286744594012585855785596659781179801600");
  GF200::Rcubed = bigint<static_cast<mp_size_t>(4)>("1602082444755226872864308934888775317659811381481692123365611");
  GF200::s = 1;
  GF200::num_bits = 200;
  GF200::inv = 9890594694840014909UL;

  auto a = GF200(libff::bigint<4>("43234613467152613512549871563467271263417258763487658172645")); //195 bits
  auto exp = libff::bigint<2>("5208756711089370345167341923545687104"); // 122 bits

  for (auto _ : state) {
    auto z = a^exp;
    benchmark::DoNotOptimize(z);
  }
}

BENCHMARK(modexp_ntl);
BENCHMARK(modexp_mont);
BENCHMARK(modexp_libff);
BENCHMARK_MAIN();
