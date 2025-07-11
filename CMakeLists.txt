cmake_minimum_required(VERSION 3.14)
project(GUICal)
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable(cal main.cpp)
target_link_libraries(cal Qt6::Widgets)

