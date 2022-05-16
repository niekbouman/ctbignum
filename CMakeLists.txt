#
# This file is part of
#
# CTBignum 	
#
# C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
# 
#
# This file is distributed under the Apache License, Version 2.0. See the LICENSE
# file for details.

cmake_minimum_required(VERSION 3.0.0)

# This cmake file was inspired by that of nlohmann/json

##
## PROJECT
## name and version
##
project(ctbignum VERSION 0.1 LANGUAGES CXX)

##
## INCLUDE
##
##
include(ExternalProject)

## External dependencies
find_package(Boost 1.61 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

##
## OPTIONS
##
option(default_cxx_standard "C++ standard" 20)
set(CMAKE_CXX_STANDARD default_cxx_standard)

option(CTBIGNUM_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ON)
option(CTBIGNUM_BuildBenchmarks "Build the benchmarks." OFF)

##
## CONFIGURATION
##
set(CTBIGNUM_TARGET_NAME               ${PROJECT_NAME})
set(CTBIGNUM_CONFIG_INSTALL_DIR        "lib/cmake/${PROJECT_NAME}")
set(CTBIGNUM_INCLUDE_INSTALL_DIR       "include")
set(CTBIGNUM_TARGETS_EXPORT_NAME       "${PROJECT_NAME}Targets")
set(CTBIGNUM_CMAKE_CONFIG_TEMPLATE     "cmake/config.cmake.in")
set(CTBIGNUM_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}/cmake_config")
set(CTBIGNUM_CMAKE_VERSION_CONFIG_FILE "${CTBIGNUM_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(CTBIGNUM_CMAKE_PROJECT_CONFIG_FILE "${CTBIGNUM_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(CTBIGNUM_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include/")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")


## Append compiler flag to silence compiler warning for GNU extension
if (CMAKE_COMPILER_IS_GNUCXX OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-string-literal-operator-template")
endif()


##
## TARGET
## create target and add include path
##
add_library(${CTBIGNUM_TARGET_NAME} INTERFACE)

target_include_directories(
    ${CTBIGNUM_TARGET_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${CTBIGNUM_INCLUDE_BUILD_DIR}>
    $<INSTALL_INTERFACE:include>
)

##
## TESTS
## create and configure the unit test target
##
include(CTest) #adds option BUILD_TESTING (default ON)

if(BUILD_TESTING AND CTBIGNUM_BuildTests)
    enable_testing()
    add_subdirectory(test)
endif()

if(CTBIGNUM_BuildBenchmarks)
    add_subdirectory(benchmarks)
endif()

##
## INSTALL
## install header files, generate and install cmake config files for find_package()
##
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CTBIGNUM_CMAKE_VERSION_CONFIG_FILE} COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
    ${CTBIGNUM_CMAKE_CONFIG_TEMPLATE}
    ${CTBIGNUM_CMAKE_PROJECT_CONFIG_FILE}
    INSTALL_DESTINATION ${CTBIGNUM_CONFIG_INSTALL_DIR}
)

install(
    DIRECTORY ${CTBIGNUM_INCLUDE_BUILD_DIR}
    DESTINATION ${CTBIGNUM_INCLUDE_INSTALL_DIR}
)
install(
    FILES ${CTBIGNUM_CMAKE_PROJECT_CONFIG_FILE} ${CTBIGNUM_CMAKE_VERSION_CONFIG_FILE}
    DESTINATION ${CTBIGNUM_CONFIG_INSTALL_DIR}
)
install(
    TARGETS ${CTBIGNUM_TARGET_NAME}
    EXPORT ${CTBIGNUM_TARGETS_EXPORT_NAME}
    INCLUDES DESTINATION ${CTBIGNUM_INCLUDE_INSTALL_DIR}
)
install(
    EXPORT ${CTBIGNUM_TARGETS_EXPORT_NAME}
    DESTINATION ${CTBIGNUM_CONFIG_INSTALL_DIR}
)
