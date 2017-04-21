#include "glsc3d_3_private.h"
void g_polygon_2D(double *xx, double *yy, int n, G_BOOL WIRE, G_BOOL FILL)
{
	if (FILL)
	{
		g_begin_triangle_fan();
		int i;
		for (i = 0; i < n; i++)
			g_emit_vertex(g_vector2(xx[i], yy[i]));
	}
	if (WIRE)
	{
		g_begin_line_loop();
		int i;
		for (i = 0; i < n; i++)
			g_emit_vertex(g_vector2(xx[i], yy[i]));
	}
}

void g_polyline_2D(double *xx, double *yy, int n)
{
	g_begin_line_strip();
	int i;
	for (i = 0; i < n; i++)
		g_emit_vertex(g_vector2(xx[i], yy[i]));
}

void g_polyline_3D(double *xx, double *yy, double *zz, int n)
{
	g_begin_line_strip();
	int i;
	for (i = 0; i < n; i++)
		g_emit_vertex(g_vector3(xx[i], yy[i], zz[i]));
}
