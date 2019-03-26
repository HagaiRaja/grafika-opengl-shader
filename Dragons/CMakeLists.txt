cmake_minimum_required(VERSION 3.13)
project(Dragons)

set(CMAKE_CXX_STANDARD 14)

add_executable(Dragons main.cpp glad.c)
add_executable(gradient_sample background_gradients.cpp glad.c)

set(SOURCE_FILES glad.c main.cpp)
target_link_libraries(Dragons GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor assimp)