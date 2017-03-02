cmake_minimum_required(VERSION 3.1)
project(GLSC3D)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_VERBOSE_MAKEFILE 1)

include_directories(Include)
add_subdirectory(Source)

add_executable(GLSC3D Test/3DFigures.c)
target_link_libraries(GLSC3D Source)

#install(TARGETS GLSC3D RUNTIME DESTINATION bin)
