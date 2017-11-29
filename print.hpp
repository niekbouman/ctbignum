#ifndef MYPRINT_HPP
#define MYPRINT_HPP

#include <cppitertools/itertools.hpp>
#include <iostream>
#include <sprout/array.hpp>

template <template <typename, size_t> class Array, typename T, size_t N>
std::ostream &operator<<(std::ostream &os, Array<T, N> big_int) {
  os << '{';
  for (auto &&e : iter::enumerate(big_int)) {
    os << e.element;
    if (e.index != big_int.size() - 1)
      os << ", ";
  }
  os << "}";
  return os;
}

template <typename T, size_t N>
std::ostream &operator<<(std::ostream &os, sprout::array<T, N> big_int) {
  std::array<T, N> tmp;

  auto i = 0;
  for (auto elem : big_int)
    tmp[i++] = elem;

  os << tmp;
  return os;
}

template <typename Arg, typename... Args>
void print(Arg &&arg, Args &&... args) {
  std::cout << std::forward<Arg>(arg);
  using expander = int[];
  (void)expander{0, (void(std::cout << " " << std::forward<Args>(args)), 0)...};
  std::cout << std::endl;
}
//template <typename Arg, typename... Args>
//void print(Arg &&arg, Args &&... args) {
//  print_(std::forward<Arg>(arg),std::forward<Args...>(args...));
//  std::cout << std::endl;
//}

/*
template <template <typename, size_t> class Array, typename T, size_t N>
void print(Array<T,N> big_int)
{
  print_('{');
  for (auto&& e: iter::enumerate(big_int))
  {
    print_(e.element);
    if (e.index != big_int.size()-1)
      print_(", ");
  }
  print_("}\n");
}

template <typename T, size_t N> void print(sprout::array<T, N> big_int) {
  std::array<T, N> tmp;

  auto i = 0;
  for (auto elem : big_int)
    tmp[i++] = elem;

  print(tmp);
}
*/



#endif
