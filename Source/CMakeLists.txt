#cmake_minimum_required(VERSION 2.6)
#project(glsc3d)

add_library(Source
	g_camera.cpp
	g_init.cpp
	g_matrix_private.c
	g_pyramid_3D.c
	g_scale.cpp
	g_sdl_wrapper.c
	g_vertex_buffer.cpp
)