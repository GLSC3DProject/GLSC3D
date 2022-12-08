#include "glsc3d_3_private.h"

int glsc3D_width;
int glsc3D_height;

float g_screen_scale_factor;
float g_retina_scale_factor;

int g_enable_transparent, TRIANGLE_BUFFER_SIZE, TEMPORARY_TRIANGLE_BUFFER_SIZE;

#ifndef G_USE_RELATIVE_RUNTIME_PATH
std::string g_runtime_dir;
#endif

void g_init_core(
	const char *WindowName, int width, int height,
	int pos_x, int pos_y,
	float r, float g, float b,
	int enable_transparent,
	int temporary_triangle_buffer_size,
	int triangle_buffer_size
)
{
#ifndef G_USE_RELATIVE_RUNTIME_PATH
#ifdef _WIN32
	g_runtime_dir = getenv("HOMEDRIVE");
	g_runtime_dir += getenv("HOMEPATH");
#else
	g_runtime_dir = getenv("HOME");
#endif
	g_runtime_dir += "/glsc3d_runtime";
#endif

	g_sdl_init(WindowName, pos_x, pos_y, width, height);

	g_general_init();
#ifdef G_USE_VERTEX_BUFFERS
	g_vertex_buffer_init();
#endif
	g_shader_program_init();
	g_text_init();

	g_marker_size(1);
	g_line_width(2);
	g_line_type(0);
	g_text_size(24);

	g_init_light(0, 0, 0, 1);

	g_scr_color(r, g, b);

	g_marker_color(1, 0, 0, 1);
	g_line_color(0, 0, 0, 1);
	g_area_color(1, 0, 0, 1);
	g_text_color(0, 0, 0, 1);

	g_enable_transparent = enable_transparent;

	if (g_enable_transparent) {
		if (temporary_triangle_buffer_size == 0)
			TEMPORARY_TRIANGLE_BUFFER_SIZE = 1 << 20;
		else
			TEMPORARY_TRIANGLE_BUFFER_SIZE = temporary_triangle_buffer_size;

		if (triangle_buffer_size == 0)
			TRIANGLE_BUFFER_SIZE = 1 << 20;
		else
			TRIANGLE_BUFFER_SIZE = triangle_buffer_size;

		g_triangle_buffer_init();
	}
}

void g_init(const char *WindowName, int width, int height)
{
	g_init_core(WindowName, width, height, 0, 60, 0.95, 0.95, 0.95, 0, 0, 0);
}
