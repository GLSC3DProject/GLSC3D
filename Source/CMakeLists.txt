find_package(OpenGL)
#find_package(SDL)

if (WINDOWS)
#	set(OPENGL_LIBRARY OpenGL32.lib)
#	set (SDL_LIBRARY SDL2.lib SDL2main.lib)
else ()
	#message(${SDL_LIBRARY})
	set(SDL_LIBRARY SDL2.a SDL2.dll.a SDL2main.a)
endif ()

#include_directories(${SDL_INCLUDE_DIR})

add_library(GLSC3D
		g_arrow.c
		g_box.c
		g_camera.cpp
		g_cone.c
		g_contln_2D.c
		g_cylinder_3D.c
		g_debug.cpp
		g_direction_private.c
		g_ellipse_3D.c
		g_fan.c
		g_fundamentals.c
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

target_link_libraries(GLSC3D ${OPENGL_gl_LIBRARY} ${SDL_LIBRARY})
