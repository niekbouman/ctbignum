#
#   This file is part of:
#
#   CTBignum 
#
#   C++ Library for Compile-Time and Run-Time Multi-Precision and Modular Arithmetic
#   
#   See the NOTICE file for the complete list of authors and contributors 
#
#   Copyright 2018 Niek J. Bouman
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

# Check dependencies
find_package(NTL REQUIRED)
find_package(GMP REQUIRED)

#############################################################################
# Catch library with the main function to speed up build
#############################################################################

add_library(catch_main OBJECT
    "src/tests_main.cpp"
)
set_target_properties(catch_main PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
    COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)
target_include_directories(catch_main PRIVATE "thirdparty/catch")


# https://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # If needed, disable particular warnings explicitly in the line below
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

#############################################################################
# one executable for each unit test file
#############################################################################

file(GLOB files "src/unit-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})
    set_target_properties(${testcase} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )

    target_compile_definitions(${testcase} PRIVATE CATCH_CONFIG_FAST_COMPILE BOOST_HANA_CONFIG_ENABLE_STRING_UDL)
    target_include_directories(${testcase} PRIVATE "thirdparty/catch")
    #target_include_directories(${testcase} PRIVATE ${CTBIGNUM_INCLUDE_BUILD_DIR})
    target_include_directories(${testcase} PRIVATE "../include")
    target_include_directories(${testcase} PRIVATE ${NTL_INCLUDES})
    target_link_libraries(${testcase} ${NTL_LIBRARIES} ${GMP_LIBRARIES})

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set_target_properties(${testcase} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-float-equal -fconstexpr-steps=2300000")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${testcase} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-float-equal -fconstexpr-depth=30")
    endif()

    add_test(NAME "${testcase}_default"
      COMMAND ${testcase} ${CATCH_TEST_FILTER}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set_tests_properties("${testcase}_default" PROPERTIES LABELS "default")

    add_test(NAME "${testcase}_all"
      COMMAND ${testcase} ${CATCH_TEST_FILTER} "*"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set_tests_properties("${testcase}_all" PROPERTIES LABELS "all")

endforeach()

