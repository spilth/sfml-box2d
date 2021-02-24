cmake_minimum_required(VERSION 3.17)
project(sfml-box2d)

set(CMAKE_CXX_STANDARD 14)

add_executable(sfml-box2d main.cpp)

set(SFML_INCLUDE_DIR "/usr/local/Cellar/sfml/2.5.1/include")
set(SFML_LIBRARY_DIR "/usr/local/Cellar/sfml/2.5.1/lib")

set(BOX2D_INCLUDE_DIR "/usr/local/Cellar/box2d/2.4.1/include")
set(BOX2D_LIBRARY_DIR "/usr/local/Cellar/box2d/2.4.1/lib")

include_directories(SFML_INCLUDE_DIR)
include_directories(BOX2D_INCLUDE_DIR)

link_directories(SFML_LIBRARY_DIR)
link_directories(BOX2D_LIBRARY_DIR)

find_package(SFML REQUIRED graphics window system)
find_package(Box2D REQUIRED)

target_link_libraries(sfml-box2d sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(sfml-box2d /usr/local/Cellar/box2d/2.4.1/lib/libbox2d.a)
