#ifndef CT_BIGINT_HPP
#define CT_BIGINT_HPP

#include <sprout/array.hpp>

namespace cbn {

  //TODO enable if op integral type
template <size_t N, typename T = uint64_t> using big_int = sprout::array<T, N>;
}
#endif
