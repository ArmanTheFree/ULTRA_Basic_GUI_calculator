cmake_minimum_required(VERSION 3.14)
project(GUICal)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
add_executable(cal main.cpp)
target_link_libraries(cal OpenGL::GL glfw m GLEW)
