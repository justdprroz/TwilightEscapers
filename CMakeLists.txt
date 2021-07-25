cmake_minimum_required(VERSION 3.17)
project("Twilight Escapers")

set(CMAKE_CXX_STANDARD 17)



if (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)
    include_directories("include")
    add_executable(client source/main.cpp)
    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
    target_link_libraries(client sfml-graphics)
endif (UNIX)

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../build)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../build)
    include_directories("include")
    add_executable(client source/main.cpp)
    include_directories("C:\\SFML-2.5.1\\include")
    find_library(SFMLGRAPHICS NAMES sfml-graphics.lib PATHS "C:\\SFML-2.5.1\\lib")
    find_library(SFMLWINDOW NAMES sfml-window.lib PATHS "C:\\SFML-2.5.1\\lib")
    find_library(SFMLSYSTEM NAMES sfml-system.lib PATHS "C:\\SFML-2.5.1\\lib")
    target_link_libraries(client ${SFMLGRAPHICS} ${SFMLWINDOW} ${SFMLSYSTEM})
endif(MSVC)



