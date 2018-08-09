extern "C" {
#include "ct-verif.h"
}

#include <cstdint>
#include <cstring>

#include <ctbignum/bigint.hpp>
#include <ctbignum/addition.hpp>
#include <ctbignum/mult.hpp>
#include <ctbignum/relational_ops.hpp>

// this suppresses warning about undefined function
void *__gxx_personality_v0; 
// ------------------------------------------------

using MachineWord_t = unsigned long;

using cbn::add;
using cbn::mul;
using cbn::big_int;

// Explicit example for wrapping a binary function
void add_wrapper(MachineWord_t *r, MachineWord_t* a, MachineWord_t* b) {
  // ct-verif annotations
  public_in(__SMACK_value(a));
  public_in(__SMACK_value(b));
  public_in(__SMACK_value(r));
  // (pointer values are public, memory where the pointers point to is secret)

  const size_t num_limbs = 4;
  big_int<num_limbs, MachineWord_t> A, B;

  memcpy(&(A[0]), a, num_limbs * sizeof(MachineWord_t));
  memcpy(&(B[0]), b, num_limbs * sizeof(MachineWord_t)); 

  auto res = add(A,B);

  memcpy(r, &(res[0]), res.size() * sizeof(MachineWord_t)); 
}

// Instead, we will use the following generic wrapper for binary functions
template<typename Functor, typename Return_t, size_t num_limbs>
void generic_wrapper(Return_t *r, MachineWord_t* a, MachineWord_t* b) {
  // ct-verif annotations
  public_in(__SMACK_value(a));
  public_in(__SMACK_value(b));
  public_in(__SMACK_value(r));
  // (pointer values are public, memory where the pointers point to is secret)

  //const size_t num_limbs = 4;
  big_int<num_limbs, MachineWord_t> A, B;

  memcpy(&(A[0]), a, num_limbs * sizeof(MachineWord_t));
  memcpy(&(B[0]), b, num_limbs * sizeof(MachineWord_t)); 

  auto res = Functor{}(A,B);

  if constexpr (std::is_same<Return_t, bool>::value) // 'if constexpr' generates C++1z warning in Clang 3.9.1, but that's okay
    *r = res;
  else
    memcpy(r, &(res[0]), sizeof(res)); 
}

// some ugly macros

// declares a functor class that wraps a binary function
#define BINFOBJ(name, funname) struct name {\
template<typename A, typename B> \
auto operator()(A a, B b) { return funname (a, b); } \
};

// declares a functor class that wraps a binary operator
#define OPERATORFOBJ(name, operator_) struct name {\
template<typename A, typename B> \
auto operator()(A a, B b) { return a operator_ b; } \
};

// macro to explicitly instantiate the generic wrapper with the functor 'baked into' the class 
#define EXPLICIT_INST(name, rettype, limbs) \
template void generic_wrapper<name, rettype, limbs>(rettype *r, MachineWord_t* a, MachineWord_t* b);

BINFOBJ(Add, add)
EXPLICIT_INST(Add, MachineWord_t, 4)

BINFOBJ(Mul, mul)
EXPLICIT_INST(Mul, MachineWord_t, 4)

OPERATORFOBJ(LessThan, <)
EXPLICIT_INST(LessThan, bool, 4)

OPERATORFOBJ(GreaterThan, >)
EXPLICIT_INST(GreaterThan, bool, 4)

OPERATORFOBJ(GreaterEqThan, >=)
EXPLICIT_INST(GreaterEqThan, bool, 4)

OPERATORFOBJ(LessEqThan, <=)
EXPLICIT_INST(LessEqThan, bool, 4)
