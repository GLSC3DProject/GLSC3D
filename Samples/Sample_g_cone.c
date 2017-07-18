#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_cone.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_3D_fix(0, -2, 2, -2, 2, -2, 2, 4, 4, 4, 0, 1, 0, 1, 0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
	g_def_scale_3D_fix(1, -2, 2, -2, 2, -2, 2, 4, 4, 4, 0, 1, 0, 1, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	int i_time;
	for(i_time = 0;;i_time++)
	{
		g_cls();

		g_sel_scale(0);
		g_cone_3D(0, -1, 0, 0, 1, 0, 1.5, 3, G_YES, G_NO);

		g_sel_scale(1);
		g_cone_3D(0, -1, 0, 0, 1, 0, 1.5, 3, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
