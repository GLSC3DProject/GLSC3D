find_package(OpenGL)
find_package(SDL)
#include(FindSDL)

#if (UNIX)
#	set(SDL_LIBRARY_TEMP /usr/lib/x86_64-linux-gnu/libSDL2.so)
#endif (UNIX)

include_directories(${SDL_INCLUDE_DIR})
message(${SDL_LIBRARY})

add_library(Source
	g_area.c
	g_arrow.c
	g_box.c
	g_camera.cpp
	g_cls.c
	g_cone.c
	g_cylinder_3D.c
	g_debug_callback.cpp
	g_direction_private.c
	g_ellipse_3D.c
	g_fan.c
	g_finish.c
	g_glwrapper_private.c
	g_init.cpp
	g_line.c
	g_matrix_private.c
	g_move_plot.c
	g_prism_3D.c
	g_pyramid_3D.c
	g_rectangle.c
	g_scale.cpp
	g_sdl_wrapper.cpp
	g_shader_program.c
	g_sphere_3D.c
	g_triangle.c
	g_triangle_buffer.c
	g_vertex_buffer.cpp
)

target_link_libraries(Source libGL.so libSDL2.so)