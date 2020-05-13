cmake_minimum_required(VERSION 3.5)
project(TEST LANGUAGES CXX)
include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)
find_package(Threads REQUIRED)
include(${CMAKE_SOURCE_DIR}/source_list.cmake)
set(SOURCE_LIST
    thread_pool_test.cc
    vector_test.cc
    matrix4.cc
    )
add_executable(runTests tests.cc ${SOURCE_LIST} ${SOURCE_HEADERS})
target_include_directories(runTests PRIVATE ../src/)
target_link_libraries(runTests gtest Threads::Threads)

