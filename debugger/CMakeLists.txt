# Project definition
cmake_minimum_required(VERSION 3.5)
project(DiDebugger LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
# Source files
set(SOURCES
    main.cc
    glsupport.h
    glsupport.cc
    )
include(../source_list.cmake)

file(GLOB IMGUI_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/*")

#find_package(OpenGL REQUIRED COMPONENTS OpenGL)

#deps
add_subdirectory(third_party/glad/)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF  CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glm)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCE} )
    #${SOURCE_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "../src")
target_include_directories(${PROJECT_NAME} PRIVATE "third_party/")
target_compile_definitions(${PROJECT_NAME} PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(${PROJECT_NAME} PRIVATE
  glad
  glfw
  #  ${OPENGL_LIBRARIES}
  ${CMAKE_DL_LIBS}
  )
