cmake_minimum_required(VERSION 3.5)
project(DiRender LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(test)

find_package(Threads REQUIRED)
set (SOURCE_HEADERS
    src/utils/di_global.h
    src/utils/thread_pool.hpp
    src/math/geometry.hpp
    src/math/math_utils.hpp
    src/math/matrix.hpp
    src/math/vector.hpp
    src/cores/ray.hpp
    src/cores/render.h
    src/cores/render.cc
    src/cores/bounds.h
    src/cores/bounds.cc
    )
include_directories("src")
add_executable(DiRender src/main.cc ${SOURCE_HEADERS})
target_link_libraries(DiRender Threads::Threads)
enable_testing()
add_test(
    NAME runTests
    COMMAND runTests
    )



