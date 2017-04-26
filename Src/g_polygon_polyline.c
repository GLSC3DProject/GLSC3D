#include "glsc3d_3_private.h"
void g_polygon_2D(double *xx, double *yy, int n, G_BOOL WIRE, G_BOOL FILL)
{
	if (FILL)
	{
		g_begin_triangles();
		for (int i = 2; i < n; i++) {
			g_emit_vertex(g_vector2(xx[0], yy[0]));
			g_emit_vertex(g_vector2(xx[i-1], yy[i-1]));
			g_emit_vertex(g_vector2(xx[i], yy[i]));
		}
	}
	if (WIRE)
	{
		g_move_2D(xx[n-1], yy[n-1]);
		for (int i = 0; i < n; i++)
			g_plot_2D(xx[i], yy[i]);
	}
}

void g_polyline_2D(double *xx, double *yy, int n)
{
	g_move_2D(xx[0], yy[0]);
	for (int i = 1; i < n; i++)
		g_plot_2D(xx[i], yy[i]);
}

void g_polyline_3D(double *xx, double *yy, double *zz, int n)
{
	g_move_3D(xx[0], yy[0], zz[0]);
	for (int i = 1; i < n; i++)
		g_plot_3D(xx[i], yy[i], zz[i]);
}
