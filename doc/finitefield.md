# Finite field arithmetic

The library supports finite-field arithmetic in the field Z/pZ, i.e., the integers modulo a prime, where this prime is fixed at compile time.

First, we declare the *type* of the finite field, by creating a dummy instance of the `Zq` class of which we take the type using C++'s `decltype` keyword:

```cpp

using namespace boost::hana::literals; // to enable the "<string>"_s syntax

using GF101 = decltype(Zq("1267650600228229401496703205653"_s));
// define the type of a 101-bit prime field

```



