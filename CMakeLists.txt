cmake_minimum_required(VERSION 3.0.0)

# This cmake file was inspired by that of nlohmann/json

##
## PROJECT
## name and version
##
project(njbouman_ctbignum VERSION 0.1 LANGUAGES CXX)

##
## INCLUDE
##
##
include(ExternalProject)

##
## OPTIONS
##
option(CTBIGNUM_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ON)
option(CTBIGNUM_BuildBenchmarks "Build the benchmarks." ON)

##
## CONFIGURATION
##
set(NJBOUMAN_CTBIGNUM_TARGET_NAME               ${PROJECT_NAME})
set(NJBOUMAN_CTBIGNUM_CONFIG_INSTALL_DIR        "lib/cmake/${PROJECT_NAME}")
set(NJBOUMAN_CTBIGNUM_INCLUDE_INSTALL_DIR       "include")
set(NJBOUMAN_CTBIGNUM_TARGETS_EXPORT_NAME       "${PROJECT_NAME}Targets")
set(NJBOUMAN_CTBIGNUM_CMAKE_CONFIG_TEMPLATE     "cmake/config.cmake.in")
set(NJBOUMAN_CTBIGNUM_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}/cmake_config")
set(NJBOUMAN_CTBIGNUM_CMAKE_VERSION_CONFIG_FILE "${NJBOUMAN_CTBIGNUM_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(NJBOUMAN_CTBIGNUM_CMAKE_PROJECT_CONFIG_FILE "${NJBOUMAN_CTBIGNUM_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(NJBOUMAN_CTBIGNUM_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include/")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

##
## TARGET
## create target and add include path
##
add_library(${NJBOUMAN_CTBIGNUM_TARGET_NAME} INTERFACE)

target_include_directories(
    ${NJBOUMAN_CTBIGNUM_TARGET_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${NJBOUMAN_CTBIGNUM_INCLUDE_BUILD_DIR}>
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
    ${NJBOUMAN_CTBIGNUM_CMAKE_VERSION_CONFIG_FILE} COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
    ${NJBOUMAN_CTBIGNUM_CMAKE_CONFIG_TEMPLATE}
    ${NJBOUMAN_CTBIGNUM_CMAKE_PROJECT_CONFIG_FILE}
    INSTALL_DESTINATION ${NJBOUMAN_CTBIGNUM_CONFIG_INSTALL_DIR}
)

install(
    DIRECTORY ${NJBOUMAN_CTBIGNUM_INCLUDE_BUILD_DIR}
    DESTINATION ${NJBOUMAN_CTBIGNUM_INCLUDE_INSTALL_DIR}
)
install(
    FILES ${NJBOUMAN_CTBIGNUM_CMAKE_PROJECT_CONFIG_FILE} ${NJBOUMAN_CTBIGNUM_CMAKE_VERSION_CONFIG_FILE}
    DESTINATION ${NJBOUMAN_CTBIGNUM_CONFIG_INSTALL_DIR}
)
install(
    TARGETS ${NJBOUMAN_CTBIGNUM_TARGET_NAME}
    EXPORT ${NJBOUMAN_CTBIGNUM_TARGETS_EXPORT_NAME}
    INCLUDES DESTINATION ${NJBOUMAN_CTBIGNUM_INCLUDE_INSTALL_DIR}
)
install(
    EXPORT ${NJBOUMAN_CTBIGNUM_TARGETS_EXPORT_NAME}
    DESTINATION ${NJBOUMAN_CTBIGNUM_CONFIG_INSTALL_DIR}
)
