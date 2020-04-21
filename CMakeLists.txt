cmake_minimum_required(VERSION 3.5)
project(DiRender LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(test)

enable_testing()
add_test(
    NAME runTests
    COMMAND runTests
    )


