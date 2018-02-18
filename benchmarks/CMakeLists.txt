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

find_package(GMP)

# optimizations for libff
add_definitions(-DUSE_ASM)
add_definitions(-DNDEBUG)
set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -flto -fuse-linker-plugin"
)
set(
   CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -flto"
)


target_include_directories(bench PRIVATE ${NJBOUMAN_CTBIGNUM_INCLUDE_BUILD_DIR} ${GMP_INCLUDES})
target_link_libraries(bench benchmark ntl ${GMP_LIBRARIES})
