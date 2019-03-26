cmake_minimum_required(VERSION 3.13)
project(Gradient_Pentagon)

set(CMAKE_CXX_STANDARD 14)

add_executable(Gradient_Pentagon main.cpp glad.c)

set(SOURCE_FILES glad.c main.cpp)
target_link_libraries(Gradient_Pentagon GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor assimp)