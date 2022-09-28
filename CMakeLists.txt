cmake_minimum_required(VERSION 3.17)
project("Twilight Escapers")

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else()
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
endif (UNIX)

# NOT MAINTAINED CURRENTLY
# if (MSVC)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../build)
# endif(MSVC)

include_directories("src")

set(
    TWE_SOURCES
    src/Render.cpp
    src/Renderer.cpp
    src/World.cpp
    src/main.cpp
)

message("TWE sources are: " ${TWE_SOURCES})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(twe ${TWE_SOURCES})
target_link_libraries(twe sfml-graphics sfml-window sfml-system)