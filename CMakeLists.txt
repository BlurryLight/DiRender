cmake_minimum_required(VERSION 3.5)
project(DiRender LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
include(${CMAKE_SOURCE_DIR}/source_list.cmake)

#options 
option(EXPERIMENTAL_UNITY_BUILD "UNITY_BUILD to reduce compilation time" OFF)
option(DI_BUILD_TESTS "Build test programs" ON)
option(DI_BUILD_DEBUGGER "Build a real-time debugger" ON)


# global clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        add_compile_definitions(-DFILESYSTEM_TS)
        link_libraries(c++fs)
    endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
endif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" )

# global gcc
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1)
        add_definitions(-DFILESYSTEM_TS)
        link_libraries(stdc++fs)
    endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1)
endif()

# main target
add_compile_definitions(ROOT_DIR="${CMAKE_SOURCE_DIR}")
message(${CMAKE_SOURCE_DIR})
add_executable(DiRender src/main.cc ${SOURCE_HEADERS})
target_include_directories(DiRender PRIVATE "src")
target_link_libraries(DiRender PRIVATE Threads::Threads)

# Flags are added only with the main target
if (MSVC)
    target_compile_options(DiRender PRIVATE /W4)
#clang
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(DiRender PRIVATE -Wall -Wextra -pedantic -Wno-unknown-warning-option )
#gcc
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(DiRender PRIVATE -Wall -Wextra -pedantic )
endif()

if(EXPERIMENTAL_UNITY_BUILD)
    include(${CMAKE_SOURCE_DIR}/cmake/unity_build.cmake)
    UNITY_BUILD(module_unity_build.cpp "${SOURCE_HEADERS}")
    set(SOURCE_HEADERS ${CMAKE_BINARY_DIR}/module_unity_build.cpp)
else(EXPERIMENTAL_UNITY_BUILD)
    #只有在不是unity-build且不是MSVC的环境下，才启动-Werror
    if (NOT MSVC)
        target_compile_options(DiRender PRIVATE -Werror)
    endif()
endif(EXPERIMENTAL_UNITY_BUILD)


if(DI_BUILD_TESTS)
    add_subdirectory(test)
    enable_testing()
    add_test(
        NAME runTests
        COMMAND runTests
        )
endif(DI_BUILD_TESTS)



if(DI_BUILD_DEBUGGER)
    add_subdirectory(debugger)
endif(DI_BUILD_DEBUGGER)
