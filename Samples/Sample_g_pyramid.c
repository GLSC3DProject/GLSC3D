#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main(void)
{
	g_init("Sample_g_pyramid.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_3D(0, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
	g_def_scale_3D(1, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	for (int i_time = 0;; i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_pyramid_3D(1, -1, 0, 0, 0, 1, 2, 4, M_PI / 8, 10, G_YES, G_NO);

		g_sel_scale(1);
		g_pyramid_3D_core(1, -1, 0, 0, 0, 1, 2, 4, M_PI / 8, 10, 4, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
