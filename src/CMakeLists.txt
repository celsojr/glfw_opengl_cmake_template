cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(glfw_opengl_cmake_template_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_WASM "Build for WebAssembly" OFF)

file(TO_NATIVE_PATH "${STAGED_INSTALL_PREFIX}/lib/cmake/glew" GLEW_DIR)

message(STATUS "GLEW_DIR is... ${GLEW_DIR}")

set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED CONFIG)

set(SRCS main.cpp)

if(NOT DEFINED INCLUDE_DIR)
    message(FATAL_ERROR "INCLUDE_DIR is not defined")
endif()

message(STATUS "INCLUDE_DIR is... ${INCLUDE_DIR}")

if(NOT DEFINED RESOURCES_DIR)
    message(FATAL_ERROR "RESOURCES_DIR is not defined")
endif()

message(STATUS "RESOURCES_DIR is... ${RESOURCES_DIR}")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${INCLUDE_DIR}/*.h)

if (BUILD_WASM)
    # Ensure toolchain is set
    if (NOT CMAKE_TOOLCHAIN_FILE)
        message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE is not set. Ensure you pass -DCMAKE_TOOLCHAIN_FILE=<path> when building for WASM.")
    endif()

    # Create the WebAssembly target
    add_executable(main ${SRCS} ${HEADERS})

    # Set the output properties for the WASM target
    set_target_properties(main PROPERTIES
        OUTPUT_NAME "index"                    # Output file name will be "index"
        SUFFIX ".html"                         # Output file will have ".html" suffix
        # LINK_FLAGS "-Os -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s OFFSCREEN_FRAMEBUFFER=1 -g -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 --no-minify --use-preload-plugins --preload-file ${WASM_OUTPUT_DIR}@/res -s FETCH=1 --embed-file ${RESOURCES_DIR}"
        LINK_FLAGS "-Os -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s OFFSCREEN_FRAMEBUFFER=1 -g -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 --no-minify --embed-file ${WASM_OUTPUT_DIR}/wasm/res -s GL_DEBUG=1"
        RUNTIME_OUTPUT_DIRECTORY ${WASM_OUTPUT_DIR}/wasm
    )

    # target_compile_options(main PRIVATE
    #     "-Os"                                  # Optimize for size
    #     "-s USE_WEBGL2=1"                      # Enable WebGL 2.0
    #     "-s FULL_ES3=1"                        # Full ES3 support
    #     "-s USE_GLFW=3"                        # Use GLFW
    #     "-s WASM=1"                            # Output WebAssembly
    #     "-s ALLOW_MEMORY_GROWTH=1"             # Allow memory growth
    #     "-s OFFSCREEN_FRAMEBUFFER=1"           # Use offscreen framebuffer
    #     "-g"                                   # Enable debug information
    #     "-s ASSERTIONS=1"                      # Enable runtime assertions
    #     "-s DEMANGLE_SUPPORT=1"                # Demangle C++ names in errors
    #     "-s SAFE_HEAP=1"                       # Check memory safety
    #     "--no-minify"                          # Avoid minified JS for easier debugging
    # )

    # target_link_options(main PRIVATE
    #     "-s ASSERTIONS=0"                        # Disable assertions
    #     "--preload-file ${WASM_OUTPUT_DIR}@/res" # Preload resources
    # )

    # Add include directories
    target_include_directories(main PRIVATE ${INCLUDE_DIR})

    message(STATUS "Before GLFW")

    # Link dependencies
    # target_link_libraries(main PRIVATE GLEW::glew_s glfw)
    # target_link_libraries(main PRIVATE GLEW::glew_s)
    # target_link_libraries(main PRIVATE GLEW::glew_s)
    target_link_libraries(main glfw)

    message(STATUS "After GLFW")

    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1")

    # find_package(glfw3)

    # Copy resources for WASM
    file(COPY ${RESOURCES_DIR} DESTINATION ${WASM_OUTPUT_DIR}/wasm)
    target_compile_definitions(main PRIVATE RESOURCES_DIR="${WASM_OUTPUT_DIR}/wasm/res")
else()
    # Native build
    if(APPLE)
        add_executable(main MACOSX_BUNDLE ${SRCS} ${HEADERS})
        # Set a custom plist file for the app bundle (needed for Mac OS Retina display)
        set_target_properties(main PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        )
    else()
        # No WIN32! We don't want WinMain() with GLFW.
        add_executable(main ${SRCS} ${HEADERS} main.exe.manifest)
    endif()

    file(TO_NATIVE_PATH "${STAGED_INSTALL_PREFIX}/lib/cmake/glfw3" glfw3_DIR)

    message(STATUS "glfw3_DIR is... ${glfw3_DIR}")

    find_package(glfw3 REQUIRED CONFIG)

    target_include_directories(main PRIVATE ${INCLUDE_DIR})
    target_link_libraries(main PRIVATE GLEW::glew_s glfw)
    target_compile_definitions(main PRIVATE RESOURCES_DIR="${RESOURCES_DIR}")
endif()
