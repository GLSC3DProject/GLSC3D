set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(2DFigures	2DFigures.c)
add_executable(3DFigures	3DFigures.c)
add_executable(Cone			Cone.c)
add_executable(Marker2D		Marker2D.c)
add_executable(Marker3D		Marker3D.cpp)

target_link_libraries(2DFigures	glsc3d_3)
target_link_libraries(3DFigures	glsc3d_3)
target_link_libraries(Cone		glsc3d_3)
target_link_libraries(Marker2D	glsc3d_3)
target_link_libraries(Marker3D	glsc3d_3)
