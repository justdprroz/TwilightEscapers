cmake_minimum_required(VERSION 3.17)

# Project definition
project(
    "Twilight Escapers"
    VERSION 0.1
)

set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set variables and compiler flags (Only these compilers supported)
if (UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
else()
    message(FATAL_ERROR "Your compiler is unsupported. Use supported one (GCC or MinGW) or modify source code")
endif (UNIX OR MINGW)

# Find SFML and include it
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
include_directories(${SFML_DIR}/include)

# Include and build engine
add_subdirectory(${CMAKE_SOURCE_DIR}/sequoia-engine)
include_directories(${CMAKE_SOURCE_DIR}/sequoia-engine/src)
target_link_libraries(sequoia sfml-graphics sfml-window sfml-system)

# Include and build executable
include_directories("src")
set(
    TWE_SOURCES
    src/main.cpp
)
add_executable(twe ${TWE_SOURCES})
target_link_libraries(twe sfml-graphics sfml-window sfml-system sequoia)