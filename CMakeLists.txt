cmake_minimum_required(VERSION 3.17)
project("Twilight Escapers")

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)

    include_directories("source")

    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

    add_executable(twe source/main.cpp source/Render.cpp)
    # add_executable(light source/lightexample.cpp)
    # add_executable(particle source/particles.cpp)
    
    target_link_libraries(twe sfml-graphics sfml-window sfml-system)
    # target_link_libraries(light sfml-graphics sfml-window sfml-system)
    # target_link_libraries(particle sfml-graphics sfml-window sfml-system)
endif (UNIX)

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../build)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../build)

    include_directories("source")
    include_directories("C:\\SFML-2.5.1\\include")

    find_library(SFMLGRAPHICS NAMES sfml-graphics.lib PATHS "C:\\SFML-2.5.1\\lib")
    find_library(SFMLWINDOW NAMES sfml-window.lib PATHS "C:\\SFML-2.5.1\\lib")
    find_library(SFMLSYSTEM NAMES sfml-system.lib PATHS "C:\\SFML-2.5.1\\lib")

    add_executable(twe source/main.cpp source/Render.cpp)
    add_executable(light source/lightexample.cpp)
    # add_executable(particle source/particles.cpp)

    target_link_libraries(twe ${SFMLGRAPHICS} ${SFMLWINDOW} ${SFMLSYSTEM})
    target_link_libraries(light ${SFMLGRAPHICS} ${SFMLWINDOW} ${SFMLSYSTEM})
    # target_link_libraries(particle ${SFMLGRAPHICS} ${SFMLWINDOW} ${SFMLSYSTEM})
endif(MSVC)