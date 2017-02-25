#include "glsc3d_private.h"

void g_finish()
{
	glEnd();
	g_vertex_buffer_flush();
	g_triangle_buffer_draw();
	
	g_swap_buffers();
	g_poll_events();
//	g_get_input();
}
