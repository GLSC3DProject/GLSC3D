#include<stdio.h>
#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_rectangle.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_3D(0, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
	g_def_scale_3D(1, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	for(int i_time = 0;;i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_rectangle_3D(0, 0, 0, 1, 1, 0, 2, 4, 0.01*i_time, G_YES, G_NO);

		g_sel_scale(1);
		g_rectangle_3D_core(0, 0, 0, 1, 1, 0, 2, 4, 0.01*i_time, 0, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
