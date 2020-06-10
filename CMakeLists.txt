cmake_minimum_required(VERSION 3.5)
project(DiRender LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)
include(${CMAKE_SOURCE_DIR}/source_list.cmake)

#options 
option(EXPERIMENTAL_UNITY_BUILD "UNITY_BUILD to reduce compilation time" OFF)
option(BUILD_TESTS "Build test programs" ON)
if (MSVC)
    add_compile_options(/W4)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -pedantic -Wno-unknown-warning-option )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic )
endif()

if(EXPERIMENTAL_UNITY_BUILD)
    include(${CMAKE_SOURCE_DIR}/cmake/unity_build.cmake)
    UNITY_BUILD(module_unity_build.cpp "${SOURCE_HEADERS}")
    set(SOURCE_HEADERS ${CMAKE_BINARY_DIR}/module_unity_build.cpp)
else(EXPERIMENTAL_UNITY_BUILD)
    #只有在不是unity-build且不是MSVC的环境下，才启动-Werror
    if (NOT MSVC)
        add_compile_options(-Werror)
    endif()

endif(EXPERIMENTAL_UNITY_BUILD)
add_executable(DiRender src/main.cc ${SOURCE_HEADERS})
target_include_directories(DiRender PRIVATE "src")
target_link_libraries(DiRender Threads::Threads)

if(BUILD_TESTS)
    add_subdirectory(test)
    enable_testing()
    add_test(
        NAME runTests
        COMMAND runTests
        )
endif(BUILD_TESTS)



