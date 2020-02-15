# Constexpr C++17 Big-Integer / Finite-Field library

### Description
This is a header-only template library for fixed-width "small big-integer" computations, for use during run-time as well as compile-time. By "small big integers", we mean numbers with a few limbs (in other words, a few hundred bits), typically
occurring in cryptographic applications.

Important note: __not all functions in the library are constant-time__ (when used at run-time); only those for which this is explicitly indicated.

Currently, the library is a __work in progress__ and supports the following operations
- addition, __*formal verification: correctness using [SAW](https://saw.galois.com/) and constant-timeness using [ct-verif](https://www.usenix.org/system/files/conference/usenixsecurity16/sec16_paper_almeida.pdf)*__ ![new][newpic] 
- subtraction, 
- multiplication (naive O(n^2) "schoolbook" multiplication) __*constant-time-verified using ct-verif*__ ![new][newpic]
- division: short division (single-limb divisor) and Donald Knuth's "algorithm D"
- division: Granlund--Montgomery division by invariant integer (gives constant-time modulo reduction),
- comparison __*constant-time-verified using ct-verif*__ ![new][newpic]
- modular addition,
- extended GCD and modular inverse,
- Barrett reduction, 
- Montgomery reduction,
- Montgomery multiplication,
- Modular exponentiation (based on Montgomery multiplication)
- Compile-time initialization from a base-10 literal
- Serialization to ostream as base-10 string (binary serialization is trivial, by just copying the limbs)

[newpic]: https://github.com/niekbouman/ctbignum/raw/master/doc/new.png

### Playground ![new][newpic]
ctbignum is available in Matt Godbolt's [*Compiler Explorer*](https://godbolt.org/z/_Cslx0)! Play around with ctbignum's API,  and see the assembly code it gets compiles down to, for the compiler of your choice..!

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
// compile with: -std=c++17 (or a more recent standard)
#include <ctbignum/ctbignum.hpp>

// Initialization via (user-defined) literal 
// (with automatic deduction of number of limbs)

using namespace cbn::literals;
constexpr auto number = cbn::to_big_int(6513020836420374401749667047018991798096360820_Z);
constexpr big_int<3> expected_result = {1315566964, 326042948, 19140048};

static_assert(number == expected_result, "initialization failure");
```

(See [unit tests](test/src/unit-tests.cpp) for more examples.)

### Documentation
- [A Fixed-Size Big-Integer Type: `big_int`](doc/bigint.md)
- [Arithmetic modulo *q*](doc/finitefield.md)
- [Presentation at CPPCon 2018 (slides, pdf, 1.7 MB)](https://github.com/CppCon/CppCon2018/raw/master/Presentations/multiprecision_arithmetic_for_cryptology_in_cpp/multiprecision_arithmetic_for_cryptology_in_cpp__niek_j_bouman__cppcon_2018.pdf)

  Or, watch the video:
  
[![ctbignum - Multi-Precision Arithmetic for Cryptology in C++, at Run-Time and at Compile-Time](https://github.com/niekbouman/ctbignum/raw/master/doc/youtube.png)](https://www.youtube.com/watch?v=G33yF26UGMo&t=1s)


### Citing our work
If you would like to mention our library in your academic publication, then please cite the following work:
*Multiprecision Arithmetic for Cryptology in C++ - Compile-Time Computations and Beating the Performance of Hand-Optimized Assembly at Run-Time*, Niek J. Bouman, 2018
```
  @misc{Bouman2018,
    author       = {Bouman, Niek J.},
    title        = {Multiprecision Arithmetic for Cryptology in C++ - Compile-Time Computations and Beating the Performance of Hand-Optimized Assembly at Run-Time},
    howpublished = {arXiv:1804.07236}, 
    year         = {2018},
    note         = {\url{https://arxiv.org/abs/1804.07236}},
  }
```

### Development
To run the tests in a docker image using `.gitlab-ci.yml`:
- [Get gitlab-runner](https://docs.gitlab.com/runner/install/index.html)
- Run `gitlab-runner exec docker build` from the project root directory

