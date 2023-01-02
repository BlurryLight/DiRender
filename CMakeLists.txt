cmake_minimum_required(VERSION 3.5)
project(DiRender LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
#spdlog
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/spdlog)
#include
include(${CMAKE_SOURCE_DIR}/source_list.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cmake_filesystem.cmake)

#options 
option(DI_BUILD_TESTS "Build test programs" OFF)
# due to the change of the Render, the debugger now needs much  more work to catch up the progress of the main project
# so I change the default option to off.
option(DI_BUILD_DEBUGGER "Build a real-time debugger" OFF)
option(DI_USE_GLM "USE GLM for matrix. DO NOT USE. BROKEN" OFF)

# options build
if (DI_BUILD_TESTS)
    add_subdirectory(test)
    enable_testing()
    add_test(
            NAME runTests
            COMMAND runTests
    )
endif (DI_BUILD_TESTS)

if (DI_BUILD_DEBUGGER)
    add_subdirectory(debugger)
endif (DI_BUILD_DEBUGGER)

# generate header
set(ROOT_DIR_VAR ${CMAKE_SOURCE_DIR})
if (DEFINED CMAKE_BUILD_TYPE)
    set(BUILD_TYPE_VAR ${CMAKE_BUILD_TYPE})
else ()
    # In Linux the default is empty.
    set(BUILD_TYPE_VAR "Debug")
endif ()
set(BUILD_SYSTEM_VERSION_VAR ${CMAKE_SYSTEM_VERSION})
set(BUILD_SYSTEM_NAME_VAR ${CMAKE_SYSTEM_NAME})
string(TIMESTAMP BUILD_UTC_TIMESTAMP_VAR UTC)
configure_file(${CMAKE_SOURCE_DIR}/src/utils/cmake_vars.h.in ${CMAKE_SOURCE_DIR}/src/utils/cmake_vars.h)


# main target
add_executable(DiRender ${SOURCE_HEADERS} src/main.cc src/sampler/sampler.cc src/sampler/sampler.hh)
target_include_directories(DiRender PRIVATE "src")
target_link_libraries(DiRender PRIVATE Threads::Threads spdlog::spdlog)

if (DI_USE_GLM)
    if (NOT DI_BUILD_DEBUGGER)
        set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
        add_subdirectory(debugger/third_party/glm)
    endif ()
    message(STATUS "DI_USE_GLM: ${GLM_VERSION}")
    target_compile_definitions(DiRender PRIVATE DI_USE_GLM)
    target_include_directories(DiRender PRIVATE "debugger/third_party/glm")
    target_link_libraries(DiRender PRIVATE glm)
endif (DI_USE_GLM)

# Flags are added only with the main target
if (MSVC)
    target_compile_options(DiRender PRIVATE /W4)
    # ignore stupid conversion warnings
    target_compile_options(DiRender PRIVATE /wd4305 /wd4244 /wd4287 /wd4838)
    # special case: nameless struct/union is nonstandard but major compilers support it so I use it
    # disable the warning about it
    target_compile_options(DiRender PRIVATE /wd4201)
    # stupid msvc only accepts UTF-8 with Bom unless we force it to accept utf-8
    target_compile_options(DiRender PRIVATE /utf-8)
    #clang
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(DiRender PRIVATE -Wall -Wextra -pedantic -Wno-unknown-warning-option)
    #gcc
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(DiRender PRIVATE -Wall -Wextra -pedantic)
endif ()