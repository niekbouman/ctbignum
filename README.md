# Constexpr Big-Integer / Finite Field library

### Description
This is a header-only template library for compile-time fixed-width big-integer computations.

Currently, the library supports the following operations
- addition, 
- subtraction, 
- multiplication (naive O(n^2) "schoolbook" multiplication)
- division (Donald Knuth's algorithm D)
- comparison (lexicographical compare from the Sprout C++ Libraries),
- modular addition,
- Montgomery reduction,
- Barrett reduction, 
- initialization from a compile-time string (using BOOST_HANA_STRING)

### Requirements
- Boost.Hana (Louis Dionne)
- Sprout C++ Libraries (Bolero Murakami)

### Requirements (for the benchmarks)
- NTL (Victor Shoup's number theory library)
- SCIPR lab's libff
- GMP (libff dependency)
- Google Benchmark

### Example
```cpp
// String Initialization

constexpr auto number = cbn::string_to_big_int<4>(
    BOOST_HANA_STRING("6513020836420374401749667047018991798096360820"));

constexpr big_int<4> expected_result = {{1315566964, 326042948, 19140048, 0}};

static_assert(number == expected_result, "initialization failure");
```
