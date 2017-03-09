#include "glsc3d_private.h"
void g_polygon_2D(double *xx, double *yy, int n, G_WIREFILL WireFill)
{
	if(WireFill == 1)
	{
		g_begin_triangle_fan();
		int i;
		for(i=0;i<n;i++)
		{
			g_emit_vertex(g_vector2(xx[i], yy[i]));
		}
	}
	if(WireFill == 0)
	{
		g_begin_line_loop();
		int i;
		for(i=0;i<n;i++)
		{
			g_emit_vertex(g_vector2(xx[i], yy[i]));
		}
	}
}

