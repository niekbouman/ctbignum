#ifndef CT_BIGINT_HPP
#define CT_BIGINT_HPP

#include <sprout/array.hpp>

namespace cbn {

template <size_t N> using big_int = sprout::array<uint64_t, N>;
}
#endif
