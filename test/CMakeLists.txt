cmake_minimum_required(VERSION 3.5)
project(TEST LANGUAGES CXX)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set (SOURCE_HEADERS
    ../src/utils/config.h
    ../src/utils/thread_pool.hpp
    )

set(SOURCE_LIST
    thread_pool_test.cc
    )
add_executable(runTests tests.cc ${SOURCE_LIST} ${SOURCE_HEADERS})
target_link_libraries(runTests ${GTEST_LIBRARIES} Threads::Threads)
enable_testing()
add_test(
    NAME runTests
    COMMAND runTests
    )

