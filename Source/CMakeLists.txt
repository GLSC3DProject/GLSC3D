find_package(OpenGL)

if (MSVC)
	list(APPEND CMAKE_C_FLAGS	" /MP")
	list(APPEND CMAKE_CXX_FLAGS	" /MP")
	set(SDL_LIBRARY SDL2.lib)
	set(PNG_LIBRARY libpng16.lib zlib.lib)
	set(FREETYPE_LIBRARIES freetype271.lib)
elseif (APPLE)
	include_directories(/opt/local/include)
	include_directories(/opt/local/include/freetype2)
	set(SDL_LIBRARY /opt/local/lib/libSDL2.dylib)
	find_package(png)
	find_package(freetype)
elseif (UNIX)
	set(SDL_LIBRARY SDL2)
	set(PNG_LIBRARY png)
	set(FREETYPE_LIBRARIES freetype)
endif ()

message(CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME})
message(CMAKE_C_FLAGS: ${CMAKE_C_FLAGS})
message(CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})
message(OPENGL_gl_LIBRARY: ${OPENGL_gl_LIBRARY})
message(SDL_LIBRARY: ${SDL_LIBRARY})
message(FREETYPE_LIBRARIES: ${FREETYPE_LIBRARIES})
message(PNG_LIBRARY: ${PNG_LIBRARY})

add_library(GLSC3D
	g_area.cpp
	g_arrow.c
	g_bird_view_3D.cpp
	g_box.c
	g_camera.cpp
	g_capture.c
	g_circle.cpp
	g_cone.cpp
	g_contln_2D.c
	g_cylinder_3D.cpp
	g_data_plot.c
	g_debug.cpp
	g_ellipse_3D.cpp
	g_fan.cpp
	g_cls_finish.c
	g_init.cpp
	g_input.c
	g_isosurface.c
	g_light.cpp
	g_line.cpp
	g_marker.cpp
	g_move_plot.c
	g_off_screen.c
	g_polygon_polyline.c
	g_prism_3D.cpp
	g_private.cpp
	g_pyramid_3D.cpp
	g_rectangle.cpp
	g_scale.cpp
	g_sdl_wrapper.cpp
	g_shader_program.cpp
	g_sphere_3D.cpp
	g_text.cpp
	g_triangle.cpp
	g_triangle_buffer.cpp
	g_vertex_buffer.cpp
	../Include/glsc3d.h
	../Include/glsc3d_math.h
	../Include/glsc3d_private.h
)

target_link_libraries(GLSC3D
	${OPENGL_gl_LIBRARY}
	${SDL_LIBRARY}
	${PNG_LIBRARY}
	${FREETYPE_LIBRARIES}
)
