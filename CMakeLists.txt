cmake_minimum_required(VERSION 3.17)
project(
    "Twilight Escapers"
    VERSION 0.1
)
set(CMAKE_CXX_STANDARD 17)

# Set variables and compiler flags
if (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif (UNIX)
if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
endif(MSVC)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Include and add engine
add_subdirectory("sequoia-engine")
include_directories("sequoia-engine/src")

# Inlcude and add sources
include_directories("src")
set(
    TWE_SOURCES
    src/main.cpp
)

# add libs and link
add_executable(twe ${TWE_SOURCES})
target_link_libraries(twe sfml-graphics sfml-window sfml-system sequoia)