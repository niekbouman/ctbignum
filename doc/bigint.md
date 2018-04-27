### The [`big_int`](/include/ctbignum/bigint.hpp) class

The `big_int` class is a thin wrapper class around C++'s fixed-size array type ([`std::array`](http://en.cppreference.com/w/cpp/container/array)).

## Creating a `big_int` from Compile-Time Literal
Defined in header [initialization.hpp](/include/ctbigint/initialization.hpp)

The user-defined literal `_Z` returns a `std::integer_sequence`, which can then be converted to a `big_int` using the `to_big_int` conversion function.
```cpp
template <char... Chars> constexpr auto operator"" _Z() 
```

Conversion function:
```cpp
template <size_t ExplicitLength = 0, typename T, T... Limbs>
constexpr auto to_big_int(std::integer_sequence<T, Limbs...>);
```

## Arithmetic operations

### Addition / Subtraction
Defined in header [addition.hpp](/include/ctbigint/addition.hpp)

*The following operations act on big-ints of the same size:*
```cpp
template <typename T, size_t N>
constexpr auto add(big_int<N, T> a, big_int<N, T> b);

template <typename T, size_t N>
constexpr auto subtract(big_int<N, T> a, big_int<N, T> b);

template <typename T, size_t N>
constexpr auto add_ignore_carry(big_int<N, T> a, big_int<N, T> b);

template <typename T, size_t N>
constexpr auto subtract_ignore_carry(big_int<N, T> a, big_int<N, T> b);

template <typename T, size_t N>
constexpr auto mod_add(big_int<N, T> a, big_int<N, T> b, big_int<N, T> modulus);

template <typename T, size_t N, T... Modulus>
constexpr auto mod_add(big_int<N, T> a, big_int<N, T> b, std::integer_sequence<T, Modulus...>);
```
*The following operation acts on big-ints of arbitrary sizes*
The `accumulate` function is used to add a `big_int` b to a accumulator `big_int` accum. Typically, N1 >= N2.
```cpp
template <typename T, size_t N1, size_t N2>
constexpr big_int<N1, T> accumulate(big_int<N1, T> accum, big_int<N2, T> b);
```
### Multiplication
Defined in header [mult.hpp](/include/ctbigint/mult.hpp)

Multiplication (for inputs of arbitrary lengths)
```cpp
template <size_t padding_limbs = 0, size_t M, size_t N, typename T>
constexpr big_int<M + N, T> mul(big_int<M, T> u, big_int<N, T> v);
```
Partial multiplication (computation of most significant limbs beyond `ResultLength` is skipped)
```cpp
template <size_t ResultLength, size_t M, size_t N, typename T>
constexpr big_int<ResultLength, T> partial_mul(big_int<M, T> u, big_int<N, T> v);
```
Short multiplication (second operand is a single limb)
```cpp
template <typename T, std::size_t N>
constexpr auto short_mul(big_int<N, T> a, T b);
```
### Division
Defined in header [division.hpp](/include/ctbigint/division.hpp)

Division. The function returns the pair (quotient, remainder).
```cpp
template <size_t N, size_t M, typename T>
constexpr std::pair<big_int<M, T>, big_int<N, T>> div(big_int<M, T> u, big_int<N, T> v);
```
Short division (second operand, the divisor, is a single limb).
The function returns the pair (quotient, remainder).
```cpp
template <size_t M, typename T> 
constexpr std::pair<big_int<M, T>, big_int<1, T>> short_div(big_int<M, T> u, T v);
```
### Exponentiation
Defined in header [pow.hpp](/include/ctbigint/pow.hpp)

Raise a `big_int` to a power that fits in a single limb
```cpp
template <std::size_t N1, typename T>
constexpr auto pow(big_int<N1, T> base, T exp) {
```
### Modular Exponentiation
Defined in header [mod_exp.hpp](/include/ctbigint/mod_exp.hpp)

Raise a `big_int` to a `big_int` power modulo a compile-time modulus
```cpp
template <std::size_t N1, std::size_t N2, typename T, T... Modulus>
constexpr auto mod_exp(big_int<N1, T> a, big_int<N2, T> exp, std::integer_sequence<T, Modulus...> modulus);
```

### Barrett Reduction
Defined in header [barrett.hpp](/include/ctbigint/barrett.hpp)

Barrett reduction with a compile-time modulus
```cpp
template <typename T, std::size_t N1, T... Modulus>
constexpr auto barrett_reduction(big_int<N1, T> x, std::integer_sequence<T, Modulus...>);
```
### Montgomery Reduction & Multiplication
Defined in header [montgomery.hpp](/include/ctbigint/montgomery.hpp)

Montgomery reduction with a compile-time modulus
```cpp
template <typename T, std::size_t N1, T... Modulus, std::size_t N2 = sizeof...(Modulus)>
constexpr auto montgomery_reduction(big_int<N1, T> A, std::integer_sequence<T, Modulus...>);
```
Montgomery multiplication with compile-time modulus
```cpp
template <typename T, std::size_t N, T... Modulus>
constexpr auto montgomery_mul(big_int<N, T> x, big_int<N, T> y, std::integer_sequence<T, Modulus...>);
```
## Relational Operators
Defined in header [relational_ops.hpp](/include/ctbigint/relational_ops.hpp)

*The following operation acts on big-ints of arbitrary sizes*
```cpp
template <typename T, size_t N1, size_t N2>
constexpr bool operator==(big_int<N1, T> a, big_int<N2, T> b);

template <typename T, size_t N1, size_t N2>
constexpr bool operator!=(big_int<N1, T> a, big_int<N2, T> b);

template <typename T, size_t N1, size_t N2>
constexpr bool operator< (big_int<N1, T> a, big_int<N2, T> b);

template <typename T, size_t N1, size_t N2>
constexpr bool operator> (big_int<N1, T> a, big_int<N2, T> b);

template <typename T, size_t N1, size_t N2>
constexpr bool operator<=(big_int<N1, T> a, big_int<N2, T> b);

template <typename T, size_t N1, size_t N2>
constexpr bool operator>=(big_int<N1, T> a, big_int<N2, T> b);
```

## Output Stream
Defined in header [io.hpp](/include/ctbigint/io.hpp)

Writes a base-10 (decimal-digit) representation of the `big_int` to an output stream
```cpp
template <std::size_t N, typename T>
std::ostream &operator<<(std::ostream &strm, cbn::big_int<N, T> obj)
```

