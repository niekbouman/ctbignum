cmake_minimum_required (VERSION 2.8)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
project (finitefield)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(MACOSX true)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(LINUX true)
endif()

if(MACOSX)
  set(CMAKE_CXX_FLAGS "-std=c++1z -stdlib=libc++  -Weverything  -pedantic -Wno-sign-conversion -Wno-sign-compare -Wno-c++98-compat -Wno-range-loop-analysis -Wno-shorten-64-to-32 -Wno-padded -Wno-comma -Wno-conversion -Wno-weak-vtables -Wno-c++98-compat-pedantic")
elseif(LINUX)
endif()

set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# for production code: remove debugging stuff:
#add_definitions(-DNDEBUG)

add_executable(unittests unit_tests.cpp tests_main.cpp)
#target_link_libraries (unittests)


