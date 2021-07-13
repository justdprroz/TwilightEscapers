cmake_minimum_required(VERSION 3.17)
project("Twilight Escapers")

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories("include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)

add_executable(client source/main.cpp)

target_link_libraries(client sfml-graphics)