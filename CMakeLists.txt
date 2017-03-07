cmake_minimum_required(VERSION 3.1)
project(3DFigures)
project(Cone)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_VERBOSE_MAKEFILE 1)

include_directories(Include)
add_subdirectory(Source)

add_executable(3DFigures Test/3DFigures.c)
target_link_libraries(3DFigures GLSC3D)

add_executable(Cone Test/Cone.c)
target_link_libraries(Cone GLSC3D)

#install(TARGETS GLSC3D RUNTIME DESTINATION bin)
