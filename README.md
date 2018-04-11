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
- Compile-time initialization from a base-10 literal
- Serialization to ostream as base-10 string (binary serialization is trivial, by just copying the limbs)

### Installation
Because this is a header-only library, installation is as easy as downloading and copying the contents of the `include` directory into your system's `include` directory (e.g. `/usr/local/include` or `/opt/local/include`).

### Required compiler
- Clang 5 or Apple LLVM/Clang 9.00 (C++17 compliant, older compilers may work but are untested)

### Dependencies
- C++ Standard library

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
constexpr big_int<3> expected_result = {1315566964, 326042948, 19140048};

static_assert(number == expected_result, "initialization failure");
```

(See [unit tests](test/src/unit-tests.cpp) for more examples.)

### Documentation
- [A Fixed-Size Big-Integer Type: `big_int`](doc/bigint.md)
- [Arithmetic modulo *q*](doc/finitefield.md)

### Citing our work
If you would like to mention our library in your academic publication, then please cite the following work:
*Multiprecision Arithmetic for Cryptology in C++ - Compile-Time Computations and Beating the Performance of Hand-Optimized Assembly at Run-Time*, Niek J. Bouman, 2018
```
  @misc{bouman18multi,
    title={Multiprecision Arithmetic for Cryptology in C++ - Compile-Time Computations and Beating the Performance of Hand-Optimized Assembly at Run-Time},
    author={Bouman, Niek J.},
    note={\url{https://eprint.iacr.org/2018/}},
  }
```

### Development
To run the tests in a docker image using `.gitlab-ci.yml`:
- [Get gitlab-runner](https://docs.gitlab.com/runner/install/index.html)
- Run `gitlab-runner exec docker build` from the project root directory

