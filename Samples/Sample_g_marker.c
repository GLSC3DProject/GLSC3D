#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_marker.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0,
		-2, 2, -2, 2,
		0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	g_def_scale_3D_fix(1, -2, 2, -2, 2, -2, 2, 2, 3, 4, 0, 1, 0, 1, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	g_def_marker(0, 1, 0, 0, 1, 0, 10);
	g_def_marker(1, 0, 1, 0, 1, 1, 15);
	g_def_marker(2, 0, 0, 1, 1, 0, 20);
	g_def_marker(3, 1, 1, 0, 1, 1, 25);
	g_def_marker(4, 1, 0, 1, 1, 0, 30);
	g_def_marker(5, 0, 1, 1, 1, 1, 35);
	g_def_marker(6, 1, 1, 1, 1, 0, 40);

	g_def_marker(7, 1, 0, 0, 1, 1, 45);
	g_def_marker(8, 0, 1, 0, 1, 0, 50);
	g_def_marker(9, 0, 0, 1, 1, 2, 55);

	for (int i_time = 0;; i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_sel_marker(0);
		g_marker_2D(0, -1.5);

		g_sel_marker(1);
		g_marker_2D(0, -1);

		g_sel_marker(2);
		g_marker_2D(0, -0.5);

		g_sel_marker(3);
		g_marker_2D(0, 0);

		g_sel_marker(4);
		g_marker_2D(0, 0.5);

		g_sel_marker(5);
		g_marker_2D(0, 1);

		g_sel_marker(6);
		g_marker_2D(0, 1.5);

		g_sel_scale(1);
		g_line_width(2);
		g_line_type(3);
		g_box_center_3D(0, 0, 0, 3.8, 3.8, 3.8, G_YES, G_NO);

		g_sel_marker(7);
		g_marker_3D(-1, 0, -1);

		g_sel_marker(8);
		g_marker_3D(-1.5, 0, 1.5);

		g_sel_marker(9);
		g_marker_3D(1, 0, 1);

		g_finish();
	}
	return 0;
}
