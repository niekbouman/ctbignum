#include <array>

template <size_t N, typename T = uint64_t,
          typename = std::enable_if_t<std::is_integral<T>::value>>
struct big_int {
  std::array<T, N> repr;

  constexpr T &operator[](size_t i) { return repr[i]; }
  constexpr const T &operator[](size_t i) const { return repr[i]; }
  constexpr size_t size() const noexcept { return repr.size(); }
};

template <typename T, size_t N>
constexpr auto add(big_int<N, T> a, big_int<N, T> b) {
  T carry{};
  big_int<N + 1, T> r{};

  for (auto i = 0; i < N; ++i) {
    auto aa = a[i];
    auto sum = aa + b[i];
    auto res = sum + carry;
    carry = (sum < aa) | (res < sum);
    r[i] = res;
  }

  r[N] = carry;
  return r;
}

// explicit instantiation
template auto add(big_int<4, uint64_t>, big_int<4, uint64_t>);
