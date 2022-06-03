cmake_minimum_required(VERSION 3.17)
project("Twilight Escapers")

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)

    include_directories("source")
    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

    add_executable(twe source/main.cpp source/Render.cpp source/World.cpp)
    target_link_libraries(twe sfml-graphics sfml-window sfml-system)
endif (UNIX)

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../build)
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")

    include_directories("source")
    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

    add_executable(twe source/main.cpp source/Render.cpp source/World.cpp)
    target_link_libraries(twe sfml-graphics sfml-window sfml-system)
endif(MSVC)