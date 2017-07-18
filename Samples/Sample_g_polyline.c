#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_polyline.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
	g_def_scale_3D_fix(1, -1, 1, -1, 1, -1, 1, 4, 4, 4, 0, 1, 0, 1, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	int i_time;
	for (i_time = 0;; i_time++)
	{
		double x[10], y[10], z[10];
		for (int i = 0; i < 10; i++)
		{
			x[i] = sin(2 * M_PI*i / 10);
			y[i] = cos(2 * M_PI*i / 10);
			z[i] = sin(2 * M_PI*i / 10)*cos(2 * M_PI*i / 10);
		}

		g_cls();
		g_sel_scale(0);
		g_polyline_2D(x, y, 10);

		g_sel_scale(1);
		g_polyline_3D(x, y, z, 10);

		g_finish();
	}
	return 0;
}
