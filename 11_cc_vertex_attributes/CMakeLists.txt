cmake_minimum_required (VERSION 2.6)

project (GLFW_WINDOW)

set (CMAKE_CXX_FLAGS "-Wall -ansi -pedantic")

include_directories (../include)

add_executable (main main.cc glad.c)
target_link_libraries(main glfw GL X11 pthread Xrandr Xi dl)
