#include "glsc3d.h"

#define ARROW_TYPE	1

int main()
{
	const int Size = 320;
	const int CountX = 3, CountY = 2;

	double p[5], q[5];
	double u[6], v[6];

	for (int i = 0; i < 5; i++)
		p[i] = sin(2*M_PI/5 * i), q[i] = cos(2*M_PI/5 * i);

	for (int i = 0; i <= 5; i++)
		u[i] = sin(4*M_PI/5 * i), v[i] = cos(4*M_PI/5 * i);

	g_init("Grid", Size * CountX, Size * CountY);

	// g_scr_color(1, 1, 1);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	g_area_color_2D(0.5, 0.75, 0.5, 1);
	g_line_color(1, 1, 1, 1);
	g_line_width(2);
	//g_line_type(1);

	//g_text_color(1, 1, 1, 1);

	int id = 0;
	for (int i = 0; i < CountX; i++)
	for (int j = 0; j < CountY; j++)
		g_def_scale_2D(id++, -1.5, 1.5, -1.5, 1.5, Size * i, Size * j, Size, Size);

	for (double t = 0;; t += 1./128) {
		double c = cos(t), s = sin(t);

		g_cls();

		g_sel_scale_2D_boundary(0);
		g_box_2D(0, 0, 2, 1, G_YES, G_NO);

		g_sel_scale_2D_boundary(1);
		g_circle_2D(0, 0, 1, G_YES, G_NO);

		g_sel_scale_2D(2);
		g_text_size(24);
		g_arrow_2D(0, 0, c, s, 1, 0.25, ARROW_TYPE);
		g_arrow_2D(0, 0,-s, c, 1, 0.25, ARROW_TYPE);
		g_text_2D_virtual( 1.1*c, 1.1*s, "x");
		g_text_2D_virtual(-1.1*s, 1.1*c, "y");

		g_sel_scale_2D(3);
		g_fan_2D(0, 0, c, s, 1, 2.0, G_YES, G_NO);

		g_sel_scale_2D(4);
		g_polygon_2D(p, q, 5, G_NO, G_YES);

		g_sel_scale_2D(5);
		g_polyline_2D(u, v, 6);

		g_finish();
	}
	return 0;
}