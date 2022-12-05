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
message(NOTICE "Build type is: " ${CMAKE_BUILD_TYPE})

# Set variables and compiler flags (Only these compilers supported)
if (UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/dev)
    if (MINGW)
        set(
            STATIC_LIBS
            -static-libstdc++
            -static-libgcc
        )
    endif(MINGW)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/dev)
    message(WARNING "Your compiler may be unsupported. Use supported one (GCC or MinGW) or modify source code")
endif (UNIX OR MINGW)

# Include and build engine
add_subdirectory(${CMAKE_SOURCE_DIR}/sequoia-engine)

# Include and build executable
include_directories("src")

# Game files

set(
    TWE_SOURCES
    src/main.cpp
)

add_executable(
    twe
    ${TWE_SOURCES}
)

target_link_libraries(
    twe
    sequoia
    ${STATIC_LIBS}
)

target_include_directories(
    twe
    PRIVATE "src"
)

# Test files

add_executable(noise src/noised.cpp)
target_link_libraries(
    noise
    sequoia
    ${STATIC_LIBS}
)
target_include_directories(
    noise
    PRIVATE "src"
)

add_executable(hash src/hashed.cpp)
target_link_libraries(
    hash
    sequoia
    ${STATIC_LIBS}
)
target_include_directories(
    hash
    PRIVATE "src"
)

add_executable(particles src/particles.cpp)
target_link_libraries(
    particles
    sequoia
    ${STATIC_LIBS}
)
target_include_directories(
    particles
    PRIVATE "src"
)