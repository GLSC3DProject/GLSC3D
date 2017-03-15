#include "glsc3d_private.h"
#define     data(i, j)      data[(j)*N_y+(i)]

void g_data_plot_f_3D(double x0, double x1,
		double y0, double y1,
		int N_x, int N_y,
		double *data)
{
	int i, j;
	double dx=(x1-x0)/N_x, dy=(y1-y0)/N_x;

	g_begin_points();

	for (j = 0; j < N_y; ++j)
		for (i = 0; i < N_x; ++i)
			g_emit_vertex(g_vector3(x0 + dx*i, y0 + dy*j, data(i, j)));
}
void g_data_plot_2D(double x_left, double x_right,
		double *yy, int n)
{
	double dx=(x_right-x_left)/n;
	int i;

	g_begin_line_strip();

	for(i=0;i<n;i++)
		g_emit_vertex(g_vector2(x_left+i*dx, yy[i]));

	for(i=0;i<n;i++)
		g_marker_2D(x_left+i*dx, yy[i]);
}
