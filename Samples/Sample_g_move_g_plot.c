#include<stdlib.h>
#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_move_g_plot.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_3D(0, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
	g_vision(0, 4, 4, 4, 0, 1, 0, 1);
	g_def_scale_2D(1, -2, 2, -2, 2, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	for (int i_time = 0;; i_time++)
	{
		g_cls();

		g_sel_scale(0);
		g_move_3D(-1, 2, -1);
		g_plot_3D(1, -2, 2);

		g_sel_scale(1);
		g_move_2D(-1, 1);
		g_plot_2D(1, -1);

		g_finish();
	}
	return 0;
}
