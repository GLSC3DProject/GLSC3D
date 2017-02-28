#include "glsc3d_private.h"

void g_scr_color(double r, double g, double b)
{
	glClearColor(r, g, b, 1);
}

void g_cls()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
