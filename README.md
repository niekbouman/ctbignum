# Constexpr C++17 Big-Integer / Finite-Field library

### Description
This is a header-only template library for compile-time fixed-width big-integer computations.

Currently, the library supports the following operations
- addition, 
- subtraction, 
- multiplication (naive O(n^2) "schoolbook" multiplication)
- division: short division (single-limb divisor) and Donald Knuth's "algorithm D"
- comparison (lexicographical compare from the Sprout C++ Libraries),
- modular addition,
- extended GCD and modular inverse,
- Barrett reduction, 
- Montgomery reduction,
- Montgomery multiplication,
- Modular exponentiation (based on Montgomery multiplication)
- initialization from a compile-time string (using BOOST_HANA_STRING)

### Requirements
- C++17 compiler (e.g., recent GCC or Clang) 
- compiler support for \_\_uint128\_t type (used for multiplication and division) 
- Boost.Hana (Louis Dionne)
- Sprout C++ Libraries (Bolero Murakami)

### Requirements (for the benchmarks)
- NTL (Victor Shoup's number theory library)
- SCIPR lab's libff
- GMP (libff dependency)
- Google Benchmark

### Example
```cpp
// String Initialization (with automatic deduction of number of limbs)

constexpr auto number = cbn::string_to_big_int(
    BOOST_HANA_STRING("6513020836420374401749667047018991798096360820"));

constexpr big_int<3> expected_result = {{1315566964, 326042948, 19140048}};

static_assert(number == expected_result, "initialization failure");
```

(See [unit tests](test/src/unit-tests.cpp) for more examples.)
