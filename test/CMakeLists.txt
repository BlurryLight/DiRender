cmake_minimum_required(VERSION 3.5)
project(TEST LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(GOOGLETEST_ROOT third_party/gtest/ CACHE STRING "Google Test source root")
if(MSVC)
    #make Windows happy
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(MSVC)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/${GOOGLETEST_ROOT} extern)

find_package(Threads REQUIRED)
include(${CMAKE_SOURCE_DIR}/source_list.cmake)
set(SOURCE_LIST
    thread_pool_test.cc
    vector_test.cc
    matrix4_test.cc
    math_utils_test.cc
    )

add_executable(runTests tests.cc ${SOURCE_LIST} ${SOURCE_HEADERS})
target_include_directories(runTests PRIVATE 
    ../src/
    ${CMAKE_SOURCE_DIR}/${GOOGLETEST_ROOT}
    ${CMAKE_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/spdlog/include/
    )
target_link_libraries(runTests gtest_main Threads::Threads)

