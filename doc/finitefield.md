# Finite field arithmetic

The library supports finite-field arithmetic in the field Z/pZ, i.e., the integers modulo a prime, where this prime is fixed at compile time.

First, we declare the *type* of the finite field by calling the [`Zq`](https://github.com/niekbouman/finitefield/search?q=%22auto+Zq%22) template function, which creates a dummy instance of the [`ZqElement`](/include/ctbignum/field.hpp) class, of which we take the type using C++'s `decltype` keyword:

```cpp
using namespace boost::hana::literals; // to enable the "<string>"_s syntax

using GF101 = decltype(Zq("1267650600228229401496703205653"_s));
// define the type of a 101-bit prime field
```
Now, we can create instances of our newly created type, and perform arithmetic using the overloaded operators in the finite field:
```cpp
GF101 x("8732191096651392800298638976"_s);
GF101 y("27349736"_s);

auto sum = x + y;
auto prod = x * y;

```
The `ZqElement` class is mainly defined to be able to write functions like `operator+` and `operator*`
that bind exclusively to `ZqElement`s. 
`ZqElement` has a [`big_int`](/include/ctbignum/bigint.hpp) as its only data member.
Below, we show its interface:

```cpp
class skip_reduction {}; // used in a constructor of ZqElement

template <typename T, T... Modulus> struct ZqElement {
  big_int<sizeof...(Modulus), T> data;

  explicit operator auto() const { return data; } // allow casting to big_int

  //==============
  // constructors
  //==============
  
  constexpr ZqElement();
  // default

  template <std::size_t N>
  constexpr ZqElement(big_int<N, T> init);
  // will reduce init if necessary

  constexpr ZqElement(big_int<sizeof...(Modulus), T> init, skip_reduction);
  // skips modulo reduction, should only be used if it is guaranteed that:
  //   init < modulus

  template <typename String,
            typename = std::enable_if_t< // only bind to Hana strings
                boost::hana::is_a<boost::hana::string_tag, String>>>
  constexpr ZqElement(String s);
  // automatically parses compile-time string
  
};
```

