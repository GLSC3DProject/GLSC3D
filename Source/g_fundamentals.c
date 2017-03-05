#include "glsc3d_private.h"

void g_scr_color(double r, double g, double b)
{
	glClearColor(r, g, b, 1);
}

void g_cls()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void g_finish()
{
	g_vertex_buffer_flush();
	g_triangle_buffer_draw();

	g_swap_buffers();
	g_poll_events();
//	g_get_input();
}
