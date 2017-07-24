#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_ellipse.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	g_def_scale_3D(0, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	for (int i_time = 0;; i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_ellipse_3D(0, 0, 0, 2, 1, 1, 0, 1, 0, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
