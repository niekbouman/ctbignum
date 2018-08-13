#include <ctbignum/bigint.hpp>
#include <ctbignum/mult.hpp>
#include <cstring> // for memcpy

template<size_t Na, size_t Nb,typename T>
void mul_wrapper__internal(T* result, T* a, T* b)
{

  using cbn::big_int;

  big_int<Na,T> A;
  big_int<Nb,T> B;

  memcpy(&(A[0]), a, Na * sizeof(T));
  memcpy(&(B[0]), b, Nb * sizeof(T));

  auto r = cbn::mul(A,B);

  memcpy(result, &(r[0]), r.size() * sizeof(T));

}

// macro to define functions 
// (declared as extern "C", to be able to mangle names in our own simpler way)
#define MUL_WRAPPER(T, N1, N2) extern "C" void mul_wrapper_##T##_##N1##_##N2(T* r, T* a, T* b) \
{ mul_wrapper__internal<N1,N2>(r,a,b); }

MUL_WRAPPER(uint8_t, 1, 1); // the name of the function will be: mul_wrapper_uint8_t_1_1
MUL_WRAPPER(uint8_t, 2, 1); // etc
MUL_WRAPPER(uint16_t, 2, 2);

