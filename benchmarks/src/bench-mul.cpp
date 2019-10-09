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

#include <ctbignum/ctbignum.hpp>
#include <ctbignum/bigint.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/mod_exp.hpp>
#include <gmp.h>


template <size_t Len>
static void mul_gmp(benchmark::State &state) {

  using namespace cbn;

  size_t total_sz = 2 * Len  * 1000;

  std::vector<uint64_t> data(total_sz);
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);
  for (auto& limb : data)
    limb = distribution(generator);

  size_t i = 0;
  auto base_ptr = reinterpret_cast<mp_limb_t*>(data.data());

    
  mp_limb_t result[2*Len];

  for (auto _ : state) {

    mpn_mul(result, base_ptr + i, Len, base_ptr + i + Len, Len);
    benchmark::DoNotOptimize(result);

    i += 2 * Len;
    if (i == total_sz)
     i = 0;
  }
}




template <size_t Len>
static void mul_cbn(benchmark::State &state) {

  using namespace cbn;

  size_t total_sz = 2* Len  * 1000;

  std::vector<uint64_t> data(total_sz);
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);
  for (auto& limb : data)
    limb = distribution(generator);

  size_t i = 0;
  auto base_ptr = data.data();


  for (auto _ : state) {

    auto x = reinterpret_cast<big_int<Len>*>(base_ptr + i);
    auto y = reinterpret_cast<big_int<Len>*>(base_ptr + i + Len);
    auto j = cbn::mul(*x, *y);
    benchmark::DoNotOptimize(j);

    i += 2 * Len;
    if (i == total_sz)
     i = 0;
  }
}

/*
template <size_t Len>
static void mul_cbn(benchmark::State &state) {

  using namespace cbn;
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0);

  big_int<Len> x;
  big_int<Len> y;
  for (int i = 0; i < Len; ++i) {
    x[i] = distribution(generator);
    y[i] = distribution(generator);
  }

  for (auto _ : state) {
    auto k = cbn::mul(x, y);
    benchmark::DoNotOptimize(k);
  }
}
*/

template <size_t Len>
static void mul_ntl(benchmark::State &state) {

  using NTL::ZZ;
  using NTL::ZZ_p;
  using NTL::conv;

  auto x = NTL::RandomBits_ZZ(Len*64);
  auto y = NTL::RandomBits_ZZ(Len*64);
  ZZ z;

  for (auto _ : state) {
    mul(z, x, y);
    benchmark::DoNotOptimize(z);
  }
}


BENCHMARK_TEMPLATE(mul_cbn, 2);
BENCHMARK_TEMPLATE(mul_ntl, 2);
BENCHMARK_TEMPLATE(mul_gmp, 2);

BENCHMARK_TEMPLATE(mul_cbn, 3);
BENCHMARK_TEMPLATE(mul_ntl, 3);
BENCHMARK_TEMPLATE(mul_gmp, 3);

BENCHMARK_TEMPLATE(mul_cbn, 4);
BENCHMARK_TEMPLATE(mul_ntl, 4);
BENCHMARK_TEMPLATE(mul_gmp, 4);

BENCHMARK_TEMPLATE(mul_cbn, 5);
BENCHMARK_TEMPLATE(mul_ntl, 5);
BENCHMARK_TEMPLATE(mul_gmp, 5);

BENCHMARK_TEMPLATE(mul_cbn, 6);
BENCHMARK_TEMPLATE(mul_ntl, 6);
BENCHMARK_TEMPLATE(mul_gmp, 6);

BENCHMARK_TEMPLATE(mul_cbn, 7);
BENCHMARK_TEMPLATE(mul_ntl, 7);
BENCHMARK_TEMPLATE(mul_gmp, 7);

BENCHMARK_TEMPLATE(mul_cbn, 8);
BENCHMARK_TEMPLATE(mul_ntl, 8);
BENCHMARK_TEMPLATE(mul_gmp, 8);


BENCHMARK_MAIN();
