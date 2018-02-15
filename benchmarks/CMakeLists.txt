cmake_minimum_required (VERSION 2.8)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
project (ctbignum_benchmarks)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(MACOSX true)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(LINUX true)
endif()

if(MACOSX)
  set(CMAKE_CXX_FLAGS "-std=c++1z -stdlib=libc++  -Weverything  -pedantic -Wno-sign-conversion -Wno-sign-compare -Wno-c++98-compat -Wno-range-loop-analysis -Wno-shorten-64-to-32 -Wno-padded -Wno-comma -Wno-conversion -Wno-weak-vtables -Wno-c++98-compat-pedantic -Wno-unneeded-member-function")
elseif(LINUX)
endif()

set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(Boost)
# the order of the libs matters for linking!
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
link_directories (${Boost_LIBRARY_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/../include)

include_directories(SYSTEM "/opt/local/include")
link_directories ("/opt/local/lib")


# for production code: remove debugging stuff:
#add_definitions(-DNDEBUG)

add_executable(bench src/bench.cpp)
target_link_libraries(bench benchmark ntl)
