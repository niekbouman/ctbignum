#
#   This file is part of:
#
#   CTBignum 
#
#   C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
#   
#   Copyright 2018 Authors
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#

add_executable(bench src/bench.cpp)

set_target_properties(bench PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )

  message("Setting CMAKE_BUILD_TYPE to ${CMAKE_BUILD_TYPE}")
   
endif()

find_package(GMP REQUIRED)
find_package(GMPXX REQUIRED)
find_package(NTL REQUIRED)
find_package(Benchmark REQUIRED)
find_package(Threads REQUIRED)

# optimizations for libff
add_definitions(-DUSE_ASM)
add_definitions(-DNDEBUG)
set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -save-temps" # -march=native -flto=thin"
)
set(
   CMAKE_EXE_LINKER_FLAGS
   "${CMAKE_EXE_LINKER_FLAGS}" #  -flto=thin"
)

target_include_directories(bench PRIVATE ${CTBIGNUM_INCLUDE_BUILD_DIR} ${Benchmark_INCLUDES} ${NTL_INCLUDES} ${GMP_INCLUDES})
target_link_libraries(bench ${Benchmark_LIBRARIES}  ${NTL_LIBRARIES} ${GMP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})


file(GLOB files "src/bench-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "bench-([^$]+)" "benchmark-\\1" benchmark ${file_basename})

    add_executable(${benchmark} ${file})
    set_target_properties(${benchmark} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        #COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        #COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )

    #target_compile_definitions(${benchmark} PRIVATE )
    target_include_directories(${benchmark} PRIVATE ${CTBIGNUM_INCLUDE_BUILD_DIR} ${Benchmark_INCLUDES} ${NTL_INCLUDES} ${GMP_INCLUDES} ${GMPXX_INCLUDE_DIR})
    #target_include_directories(${benchmark} PRIVATE ${CTBIGNUM_INCLUDE_BUILD_DIR})
    #target_include_directories(${benchmark} PRIVATE "../include")
    target_link_libraries(${benchmark} ${Benchmark_LIBRARIES}  ${NTL_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT})
      

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set_target_properties(${benchmark} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-float-equal -fconstexpr-steps=2300000")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${benchmark} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-float-equal -fconstexpr-depth=30")
    endif()

endforeach()

