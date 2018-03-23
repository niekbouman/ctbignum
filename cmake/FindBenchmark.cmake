# Try to find the Google Benchmark library

if (Benchmark_INCLUDES AND Benchmark_LIBRARIES)
  set(Benchmark_FIND_QUIETLY TRUE)
endif (Benchmark_INCLUDES AND Benchmark_LIBRARIES)

find_path(Benchmark_INCLUDES
  NAMES
    benchmark/benchmark.h
  PATHS
    $ENV{BenchmarkDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(Benchmark_LIBRARIES benchmark PATHS $ENV{BenchmarkDIR} ${LIB_INSTALL_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Benchmark DEFAULT_MSG
                                  Benchmark_INCLUDES Benchmark_LIBRARIES)
mark_as_advanced(Benchmark_INCLUDES Benchmark_LIBRARIES)
