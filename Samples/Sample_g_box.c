#include<stdio.h>
#include "glsc3d_3.h"

#define GRID_SIZE 500

int main()
{
	g_init("Sample_g_box.c", GRID_SIZE * 2, GRID_SIZE * 2);

	g_def_scale_2D(0, -2, 2, -2, 2, 0, 0, GRID_SIZE, GRID_SIZE);
	g_def_scale_3D(1, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, GRID_SIZE, 0, GRID_SIZE, GRID_SIZE);

	g_def_scale_2D(2, -2, 2, -2, 2, 0, GRID_SIZE, GRID_SIZE, GRID_SIZE);
	g_def_scale_3D(3, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, GRID_SIZE, GRID_SIZE, GRID_SIZE, GRID_SIZE);

	int i_time;
	for(i_time = 0;;i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_box_center_2D(1, 1, 1, 1, G_NO, G_YES);
		g_box_center_2D(-1, -1, 1, 1, G_YES, G_NO);

		g_sel_scale(1);
		g_box_center_3D(0, 0, 0, 3, 3, 3, G_YES, G_YES);

		g_sel_scale(2);
		g_box_2D(0.5, 1.5, 0.5, 1.5, G_NO, G_YES);
		g_box_2D(-1.5, -0.5, -1.5, -0.5, G_YES, G_NO);

		g_sel_scale(3);
		g_box_3D(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5, G_YES, G_YES);

		g_finish();
	}
	return 0;
}
