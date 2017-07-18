#include<stdlib.h>
#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_arrow.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
	g_def_scale_3D_fix(1, -4, 4, -4, 4, -4, 4, 10, 10, 10, 0, 1, 0, 1, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	int i_time;
	for (i_time = 0;; i_time++)
	{
		g_cls();

		g_sel_scale(0);
		g_arrow_2D(-2, 0, 2, 0, 4, 0.5, 0);
		g_arrow_2D(-1.4, -1.4, 1, 1, 4, 0.5, 2);
		g_arrow_2D(0, -2, 0, 2, 4, 0.5, 1);

		g_sel_scale(1);
		g_arrow_3D(-4, 0, 0, 4, 0, 0, 8, 1, G_NO, G_YES);
		g_arrow_3D(0, -4, 0, 0, 4, 0, 8, 1, G_NO, G_YES);
		g_arrow_3D(0, 0, -4, 0, 0, 4, 8, 1, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
