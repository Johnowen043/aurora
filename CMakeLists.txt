cmake_minimum_required(VERSION 3.16)
project(AuroraUI VERSION 1.0.0 LANGUAGES CXX C)

# C++17 is minimum, C++20 preferred
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(AURORA_BUILD_EXAMPLES "Build example applications" ON)
option(AURORA_BUILD_TESTS "Build unit tests" OFF)
option(AURORA_USE_WAYLAND "Enable Wayland support" OFF)
option(AURORA_USE_VULKAN "Enable Vulkan renderer (experimental)" OFF)

# Find dependencies
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)

# Platform detection
if(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    add_definitions(-DAURORA_PLATFORM_FREEBSD)
endif()

# Include paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter
        -march=native
        -fPIC
    )
    
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        add_compile_options(-g -O0 -DAURORA_DEBUG)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
endif()

# Aurora library
add_library(aurora SHARED)
target_sources(aurora PRIVATE
    # Add source files here
)

target_link_libraries(aurora
    PUBLIC
        OpenGL::GL
        ${X11_LIBRARIES}
        Threads::Threads
    PRIVATE
        ${CMAKE_DL_LIBS}
        m
)

# Examples
if(AURORA_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(AURORA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS aurora
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/aurora
    DESTINATION include
)

# Package config
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AuroraConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/AuroraConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Aurora
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/AuroraConfig.cmake"
    DESTINATION lib/cmake/Aurora
)