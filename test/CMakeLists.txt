cmake_minimum_required(VERSION 3.5)
project(TEST LANGUAGES CXX)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(runTests tests.cc)
target_link_libraries(runTests ${GTEST_LIBRARIES} Threads::Threads)
enable_testing()
add_test(
    NAME runTests
    COMMAND runTests
    )

