#include "glsc3d_3_private.h"

int glsc3D_width;
int glsc3D_height;

float g_screen_scale_factor;
float g_retina_scale_factor;

int g_enable_transparent, TRIANGLE_BUFFER_SIZE, TEMPORARY_TRIANGLE_BUFFER_SIZE;

void g_init_core(
	const char *WindowName, int width, int height,
	int pos_x, int pos_y,
	float r, float g, float b,
	int enable_transparent,
	int temporary_triangle_buffer_size,
	int triangle_buffer_size
)
{
	g_sdl_init(WindowName, pos_x, pos_y, width, height);

	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

//	glEnable(GL_CULL_FACE);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	g_vertex_buffer_init();

	g_shader_program_init();
	g_text_init();

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	glEnable(GL_POINT_SPRITE);

	glEnable(GL_NORMALIZE);

	g_marker_size(1);
	g_line_width(2);
	g_line_type(0);

	g_init_light(0, 0, 0, 1);

	g_scr_color(r, g, b);

	g_marker_color(1, 0, 0, 1);
	g_line_color(0, 0, 0, 1);
	g_area_color_2D(1, 0, 0, 1);
	g_area_color_3D(1, 0, 0, 1);
	g_text_color(0, 0, 0, 1);

	g_enable_transparent = enable_transparent;

	if(g_enable_transparent)
	{
		if(temporary_triangle_buffer_size == 0)
		{
			TEMPORARY_TRIANGLE_BUFFER_SIZE = 1 << 20;
		}
		else
		{
			TEMPORARY_TRIANGLE_BUFFER_SIZE = temporary_triangle_buffer_size;
		}
		if(triangle_buffer_size == 0)
		{
			TRIANGLE_BUFFER_SIZE = 1 << 20;
		}
		else
		{
			TRIANGLE_BUFFER_SIZE = triangle_buffer_size;
		}

		g_triangle_buffer_init();
	}
}

void g_init(const char *WindowName, int width, int height)
{
	g_init_core(WindowName, width, height, 0, 60, 0.95, 0.95, 0.95, 0, 0, 0);
}
