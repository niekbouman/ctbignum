# Constexpr Big-Integer / Finite Field library

### Description
This is a header-only template library for compile-time fixed-width big-integer computations.

Currently, the library supports addition, subtraction, multiplication, comparison, modular addition, Montgomery reduction, Barrett reduction, and initialization from a compile-time string.

### Requirements
- Boost.Hana
- Sprout

### Requirements (for the benchmarks)
- NTL (Victor Shoup's number theory library)
- Google Benchmark

### Example
```cpp
// String Initialization

constexpr auto number = cbn::string_to_big_int<4>(
    BOOST_HANA_STRING("6513020836420374401749667047018991798096360820"));

constexpr big_int<4> expected_result = {{1315566964, 326042948, 19140048, 0}};

static_assert(number == expected_result, "initialization failure");
```
