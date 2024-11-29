cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(glfw_opengl_cmake_template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ExternalProject base
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)

set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
set(EP_SOURCE_DIR_PREFIX ${CMAKE_BINARY_DIR}/subprojects/Source)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

add_subdirectory(thirdparty)

include(ExternalProject)

option(BUILD_WASM "Build for WebAssembly" OFF)

if (BUILD_WASM)
    message(STATUS "Configuring for WebAssembly...")
    set(CMAKE_TOOLCHAIN_FILE $ENV{EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake)
endif()

ExternalProject_Add(${PROJECT_NAME}_core
  DEPENDS GLFW_external GLEW_external glm_external
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DSTAGED_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
    -DINCLUDE_DIR=${INCLUDE_DIR}
    -DRESOURCES_DIR=${RESOURCES_DIR}
    -DBUILD_WASM=${BUILD_WASM}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
  CMAKE_CACHE_ARGS
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
  BUILD_ALWAYS ON
  INSTALL_COMMAND ""
)
