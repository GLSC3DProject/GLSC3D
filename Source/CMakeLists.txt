find_package(OpenGL)
find_package(freetype)
#find_library(FREETYPE_LIBRARIES freetype)
find_package(png)

if (MSVC)
	list(APPEND CMAKE_C_FLAGS	" /MP")
	list(APPEND CMAKE_CXX_FLAGS	" /MP")
	set(SDL_LIBRARY SDL2.lib SDL2main.lib)
	set(FREETYPE_LIBRARIES freetype271.lib)
	set(PNG_LIBRARY libpng16.lib zlib.lib)
#elseif (MINGW)
#	set(SDL_LIBRARY SDL2.a SDL2.dll.a SDL2main.a)
elseif (APPLE)
	include_directories(/opt/local/include)
	include_directories(/opt/local/include/freetype2)
	set(SDL_LIBRARY /opt/local/lib/libSDL2.dylib)
#	set(FREETYPE_LIBRARIES libfreetype.dylib)
elseif (UNIX)
	set(SDL_LIBRARY			libSDL2.so)
	set(FREETYPE_LIBRARIES	/usr/lib/x86_64-linux-gnu/libfreetype.so.6)
	set(PNG_LIBRARY			/usr/lib/x86_64-linux-gnu/libpng12.so)
endif ()

message(CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME})
message(CMAKE_C_FLAGS: ${CMAKE_C_FLAGS})
message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(OPENGL_gl_LIBRARY: ${OPENGL_gl_LIBRARY})
message(SDL_LIBRARY: ${SDL_LIBRARY})
message(SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR})
message(FREETYPE_LIBRARIES: ${FREETYPE_LIBRARIES})
message(FREETYPE_INCLUDE_DIRS: ${FREETYPE_INCLUDE_DIRS})
message(PNG_LIBRARY: ${PNG_LIBRARY})
message(PNG_INCLUDE_DIR: ${PNG_INCLUDE_DIR})

add_library(GLSC3D
		g_area.cpp
		g_arrow.c
		g_bird_view_3D.c
		g_box.c
		g_camera.cpp
		g_capture.c
		g_circle.c
		g_cone.c
		g_contln_2D.c
		g_cylinder_3D.cpp
		g_data_plot.c
		g_debug.cpp
		g_direction_private.c
		g_ellipse_3D.c
		g_fan.c
		g_cls_finish.c
		g_init.cpp
		g_input.c
		g_isosurface.c
		g_light.cpp
		g_line.cpp
		g_marker.cpp
		g_matrix_private.c
		g_move_plot.c
		g_off_screen.c
		g_polygon_polyline.c
		g_prism_3D.cpp
		g_pyramid_3D.c
		g_rectangle.c
		g_scale.cpp
		g_sdl_wrapper.cpp
		g_shader_program.cpp
		g_sphere_3D.c
		g_text.cpp
		g_triangle.c
		g_triangle_buffer.cpp
		g_vertex_buffer.cpp
		../Include/glsc3d.h
		../Include/glsc3d_math.h
		../Include/glsc3d_private.h
)

target_link_libraries(GLSC3D ${OPENGL_gl_LIBRARY} ${SDL_LIBRARY} ${FREETYPE_LIBRARIES} ${PNG_LIBRARY})
