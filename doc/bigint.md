## The [`big_int`](/include/ctbignum/bigint.hpp) class

The `big_int` class is a thin wrapper class around C++'s fixed-size array type ([`std::array`](http://en.cppreference.com/w/cpp/container/array)).

### Creating a `big_int` from Compile-Time Literal
Defined in header [decimal_literals.hpp](/include/ctbignum/decimal_literals.hpp)

The user-defined literal `_Z` returns a `std::integer_sequence`, which can then be converted to a `big_int` using the `to_big_int` conversion function.
```cpp
template <char... Chars> constexpr auto operator"" _Z() 
```

#### Example:
```cpp
auto i_am_an_integer_sequence = 1457094817350874187634298576109387589746_Z;
```

### Converting a `std::integer_sequence` to a `big_int`:
```cpp
template <size_t ExplicitLength = 0, typename T, T... Limbs>
constexpr auto to_big_int(std::integer_sequence<T, Limbs...>);
```

#### Example:
```cpp
constexpr auto i_am_a_big_int = to_big_int(7656523141023855493084520646_Z);
```

## Arithmetic operations
To start, some examples (also to show the overloaded operators)  
```cpp
#include <ctbignum/ctbignum.hpp>
using namespace cbn;

constexpr auto x = to_big_int(29519863647905179352825_Z);
constexpr auto y = to_big_int(1180591620717411303424_Z);

static_assert(x + y == to_big_int(30700455268622590656249_Z)); // invokes add
static_assert(x - y == to_big_int(28339272027187768049401_Z)); // invokes subtract 
static_assert(x * y == to_big_int(34850903667437369155084628453170676026572800_Z));  // invokes mul
static_assert(x / y == to_big_int(25_Z));  // invokes div, selects the quotient 
static_assert(x % y == to_big_int(5073129969896767225_Z));  // invokes div, selects the remainder
```

### Addition / Subtraction
Defined in header [addition.hpp](/include/ctbignum/addition.hpp)

*The following operation acts on big-ints of arbitrary sizes*
```cpp
template <typename T, size_t M, size_t N>
constexpr auto add(big_int<M, T> a, big_int<N, T> b);

template <typename T, size_t M, size_t N>
constexpr auto subtract(big_int<M, T> a, big_int<N, T> b);
```

*The following operations act on big-ints of the same size:*
```cpp
template <typename T, size_t N>
constexpr auto add_ignore_carry(big_int<N, T> a, big_int<N, T> b);

template <typename T, size_t N>
constexpr auto subtract_ignore_carry(big_int<N, T> a, big_int<N, T> b);

template <typename T, size_t N>
constexpr auto mod_add(big_int<N, T> a, big_int<N, T> b, big_int<N, T> modulus);

template <typename T, size_t N, T... Modulus>
constexpr auto mod_add(big_int<N, T> a, big_int<N, T> b, std::integer_sequence<T, Modulus...>);
```

### Multiplication
Defined in header [mult.hpp](/include/ctbignum/mult.hpp)

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
Defined in header [division.hpp](/include/ctbignum/division.hpp)

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

#### Division and modular reduction by an invariant (compile-time) divisor/modulus
Defined in header [invariant_div.hpp](/include/ctbignum/invariant_div.hpp)

Division via multiplication and bitshifts (see paper by Granlund and Montgomery, _"Division by Invariant Integers using Multiplication"_, 1994) 
```cpp
template <typename T, size_t N, T... Divisor>
constexpr auto div(big_int<N, T> n, std::integer_sequence<T, Divisor...>);
```
Remainder (modulo) operation
```cpp
template <typename T, size_t N, T... Modulus>
constexpr auto mod(big_int<N, T> n, std::integer_sequence<T, Modulus...>);
```

### Exponentiation
Defined in header [pow.hpp](/include/ctbignum/pow.hpp)

Raise a `big_int` to a power that fits in a single limb
```cpp
template <std::size_t N1, typename T>
constexpr auto pow(big_int<N1, T> base, T exp) {
```
### Modular Exponentiation
Defined in header [mod_exp.hpp](/include/ctbignum/mod_exp.hpp)

Raise a `big_int` to a `big_int` power modulo a compile-time modulus
```cpp
template <std::size_t N1, std::size_t N2, typename T, T... Modulus>
constexpr auto mod_exp(big_int<N1, T> a, big_int<N2, T> exp, std::integer_sequence<T, Modulus...> modulus);
```

### Barrett Reduction
Defined in header [barrett.hpp](/include/ctbignum/barrett.hpp)

Barrett reduction with a compile-time modulus
```cpp
template <typename T, std::size_t N1, T... Modulus>
constexpr auto barrett_reduction(big_int<N1, T> x, std::integer_sequence<T, Modulus...>);
```
### Montgomery Reduction & Multiplication
Defined in header [montgomery.hpp](/include/ctbignum/montgomery.hpp)

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
Defined in header [relational_ops.hpp](/include/ctbignum/relational_ops.hpp)

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
Defined in header [io.hpp](/include/ctbignum/io.hpp)

Writes a base-10 (decimal-digit) representation of the `big_int` to an output stream
```cpp
template <std::size_t N, typename T>
std::ostream &operator<<(std::ostream &strm, cbn::big_int<N, T> obj)
```
#### Example
```cpp
#include <ctbignum/decimal_literals.hpp>
#include <ctbignum/io.hpp>
#include <iostream>

using namespace cbn;
std::cout << to_big_int(123456789012345678901234567890_Z) << '\n';
```
