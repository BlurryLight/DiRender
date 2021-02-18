cmake_minimum_required(VERSION 3.5)
project(DiRender LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
#spdlog
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/spdlog)
include(${CMAKE_SOURCE_DIR}/source_list.cmake)

#options 
option(EXPERIMENTAL_UNITY_BUILD "UNITY_BUILD to reduce compilation time" OFF)
option(DI_BUILD_TESTS "Build test programs" ON)
option(DI_BUILD_DEBUGGER "Build a real-time debugger" ON)
option(DI_USE_GLM "USE GLM for matrix. DO NOT USE. BROKEN" OFF)
# options build
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

# global clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        add_definitions(-DFILESYSTEM_TS)
        link_libraries(stdc++fs)
    endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
endif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" )

# global gcc
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1)
        add_definitions(-DFILESYSTEM_TS)
        link_libraries(stdc++fs) 
        # Here clang uses libstdc++ as STL in Linux, which in OSX and FreeBSD should be libc++, and thus it should link c++fs. However, I have neither machine nor time to verify it.
    endif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1)
endif()

# generate header
set(ROOT_DIR_VAR ${CMAKE_SOURCE_DIR})
if (DEFINED CMAKE_BUILD_TYPE)
    set(BUILD_TYPE_VAR ${CMAKE_BUILD_TYPE})
else()
    # In Linux the default is empty.
    set(BUILD_TYPE_VAR "Debug")
endif()
set(BUILD_SYSTEM_VERSION_VAR ${CMAKE_SYSTEM_VERSION})
set(BUILD_SYSTEM_NAME_VAR ${CMAKE_SYSTEM_NAME})
string(TIMESTAMP BUILD_UTC_TIMESTAMP_VAR UTC)
configure_file(${CMAKE_SOURCE_DIR}/src/utils/cmake_vars.h.in ${CMAKE_SOURCE_DIR}/src/utils/cmake_vars.h)


# main target
add_executable(DiRender src/main.cc ${SOURCE_HEADERS} )
target_include_directories(DiRender PRIVATE "src" )
target_link_libraries(DiRender PRIVATE Threads::Threads spdlog::spdlog)

if(DI_USE_GLM)
    if(NOT DI_BUILD_DEBUGGER)
    set(GLM_TEST_ENABLE OFF  CACHE BOOL "" FORCE)
    add_subdirectory(debugger/third_party/glm)
    endif()
    message(STATUS "DI_USE_GLM: ${GLM_VERSION}")
    target_compile_definitions(DiRender PRIVATE DI_USE_GLM)
    target_include_directories(DiRender PRIVATE "debugger/third_party/glm")
    target_link_libraries(DiRender PRIVATE glm)
endif(DI_USE_GLM)

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
