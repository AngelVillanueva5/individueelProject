cmake_minimum_required(VERSION 3.22.1)
PROJECT(cube VERSION 0.1.0)

add_executable(cube src/main.cpp)

target_link_libraries(cube glfw)
target_link_libraries(cube GL)
target_link_libraries(cube glut)
target_link_libraries(cube GLEW)
target_link_libraries(cube GLU)
