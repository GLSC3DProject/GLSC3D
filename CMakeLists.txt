cmake_minimum_required(VERSION 3.1)

project(GLSC3D)
project(2DFigures)
project(3DFigures)
project(Box)
project(Cone)
project(Sample_g_box)
project(Sample_g_font)

#set(CMAKE_C_FLAGS -Werror)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE 1)

include_directories(Include)

add_subdirectory(Source)

add_executable(2DFigures Test/2DFigures.c)
add_executable(3DFigures Test/3DFigures.c)
add_executable(Box Test/Box.c)
add_executable(Cone Test/Cone.c)
add_executable(Sample_g_box Test/Sample_g_box.c)
add_executable(Sample_g_font Test/Sample_g_font.c)

target_link_libraries(2DFigures GLSC3D)
target_link_libraries(3DFigures GLSC3D)
target_link_libraries(Box GLSC3D)
target_link_libraries(Cone GLSC3D)
target_link_libraries(Sample_g_box GLSC3D)
target_link_libraries(Sample_g_font GLSC3D)

#install(TARGETS GLSC3D RUNTIME DESTINATION bin)
