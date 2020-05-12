cmake_minimum_required(VERSION 3.5)
project(DiRender LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(test)

find_package(Threads REQUIRED)
include(${CMAKE_SOURCE_DIR}/source_list.cmake)

add_executable(DiRender src/main.cc ${SOURCE_HEADERS})
target_include_directories(DiRender PRIVATE "src")
target_link_libraries(DiRender Threads::Threads)
enable_testing()
add_test(
    NAME runTests
    COMMAND runTests
    )



