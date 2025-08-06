cmake_minimum_required(VERSION 3.14)
project(ULTRA_BASIC_CALCULATOR)


if(WIN32)
    set(EXTRA_LIBS opengl32 glfw3 freeglut)
elseif(UNIX)
    set(EXTRA_LIBS GL GLEW glfw)
endif()

add_executable(u_b_calculator ../main.cpp)

target_link_libraries(u_b_calculator ${EXTRA_LIBS})
