cmake_minimum_required(VERSION 3.5)
project(TEST LANGUAGES CXX)
include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)
find_package(Threads REQUIRED)
include_directories(../src/)

set (SOURCE_HEADERS
    ../src/utils/di_global.h
    ../src/utils/thread_pool.hpp
    ../src/math/geometry.hpp
    ../src/math/math_utils.hpp
    ../src/math/matrix.hpp
    ../src/math/vector.hpp
    ../src/cores/ray.hpp
    )

set(SOURCE_LIST
    thread_pool_test.cc
    vector_test.cc
    )
add_executable(runTests tests.cc ${SOURCE_LIST} ${SOURCE_HEADERS})
target_link_libraries(runTests gtest Threads::Threads)

