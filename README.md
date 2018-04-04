# Constexpr C++17 Big-Integer / Finite-Field library

### Description
This is a header-only template library for compile-time fixed-width "small big-integer" computations.
By "small big integers", we mean numbers with a few limbs (in other words, a few hundred bits), typically
occurring in cryptographic applications.

Currently, the library supports the following operations
- addition, 
- subtraction, 
- multiplication (naive O(n^2) "schoolbook" multiplication)
- division: short division (single-limb divisor) and Donald Knuth's "algorithm D"
- comparison
- modular addition,
- extended GCD and modular inverse,
- Barrett reduction, 
- Montgomery reduction,
- Montgomery multiplication,
- Modular exponentiation (based on Montgomery multiplication)
- initialization from a compile-time string (using BOOST_HANA_STRING)

### Installation
Because this is a header-only library, installation is as easy as downloading and copying the contents of the `include` directory into your system's `include` directory (e.g. `/usr/local/include` or `/opt/local/include`).

### Dependencies
- C++17 compiler (e.g., recent GCC or Clang) 

### Dependencies for compiling and running the benchmarks
- [NTL (Victor Shoup's number theory library)](http://www.shoup.net/ntl/), version 10.5.0 or newer.
- [SCIPR lab's libff](https://github.com/scipr-lab/libff)
- [GMP](https://gmplib.org/) (libff dependency)
- [Google Benchmark](https://github.com/google/benchmark)

### Example
```cpp
// Initialization via (user-defined) literal 
// (with automatic deduction of number of limbs)

constexpr auto number = cbn::to_big_int(6513020836420374401749667047018991798096360820_Z);
constexpr big_int<3> expected_result = {{1315566964, 326042948, 19140048}};

static_assert(number == expected_result, "initialization failure");
```

(See [unit tests](test/src/unit-tests.cpp) for more examples.)

### Documentation
- [A Fixed-Size Big-Integer Type: `big_int`](doc/bigint.md)
- [Arithmetic modulo *q*](doc/finitefield.md)

### Development
To run the tests in a docker image using `.gitlab-ci.yml`:
- [Get gitlab-runner](https://docs.gitlab.com/runner/install/index.html)
- Run `gitlab-runner exec docker build` from the project root directory

