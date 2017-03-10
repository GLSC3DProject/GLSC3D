find_package(OpenGL)
find_package(SDL2)
find_package(Freetype)

if (MSVC)
	set(SDL_LIBRARY SDL2.lib SDL2main.lib)
	set(FREETYPE_LIBRARIES freetype271.lib)
#elseif (MINGW)
#	set(SDL_LIBRARY SDL2.a SDL2.dll.a SDL2main.a)
elseif(MACOS)
	include_directories(/opt/local/include)
elseif (UNIX)
	set(SDL_LIBRARY libSDL2.so)
	set(FREETYPE_LIBRARIES /usr/lib/x86_64-linux-gnu/libfreetype.so.6)
endif ()

message(OPENGL_gl_LIBRARY = ${OPENGL_gl_LIBRARY})
message(SDL_LIBRARY = ${SDL_LIBRARY})
message(SDL_INCLUDE_DIR = ${SDL_INCLUDE_DIR})
message(FREETYPE_LIBRARIES = ${FREETYPE_LIBRARIES})
message(FREETYPE_INCLUDE_DIRS = ${FREETYPE_INCLUDE_DIRS})

include_directories(Include)

add_library(GLSC3D
		g_arrow.c
		g_box.c
		g_camera.cpp
		g_circle.cpp
		g_color.cpp
		g_cone.c
		g_contln_2D.c
		g_cylinder_3D.c
		g_debug.cpp
		g_direction_private.c
		g_ellipse_3D.c
		g_fan.cpp
		g_fundamentals.c
		g_init.cpp
		g_line.c
		g_matrix_private.c
		g_move_plot.c
		g_polygon_polyline.c
		g_prism_3D.c
		g_pyramid_3D.c
		g_rectangle.c
		g_scale.cpp
		g_sdl_wrapper.cpp
		g_shader_program.cpp
		g_sphere_3D.c
		g_text.cpp
		g_triangle.c
		g_triangle_buffer.c
		g_vertex_buffer.cpp
		../Include/glsc3d.h
		../Include/glsc3d_math.h
		../Include/glsc3d_private.h
		../Include/glsc3d_private_ext.h
)

target_link_libraries(GLSC3D ${OPENGL_gl_LIBRARY} ${SDL_LIBRARY} ${FREETYPE_LIBRARIES})
