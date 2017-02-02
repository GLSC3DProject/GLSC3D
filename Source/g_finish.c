#include "glsc3d_private.h"

void g_finish()
{
	glEnd();
	g_vertex_buffer_flush();
	g_triangle_buffer_draw();
	glutSwapBuffers();
	glutMainLoopEvent();
	g_get_input();
}
