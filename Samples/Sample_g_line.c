#include<stdio.h>

#include "glsc3d.h"

#define INTV              (1)
#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_line.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0,
				   -2, 2, -2, 2,
				   0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_line(0, 1, 0, 0, 1, 4, 0);
	g_def_line(1, 0, 1, 0, 1, 4, 1);
	g_def_line(2, 0, 0, 1, 1, 4, 2);
	g_def_line(3, 0, 1, 0, 1, 4, 3);
	g_def_line(4, 1, 0, 0, 1, 4, 4);
	g_def_line(5, 0, 1, 0, 1, 4, 5);
	g_def_line(6, 0, 0, 1, 1, 4, 6);
	g_def_line(7, 0, 1, 0, 1, 4, 7);
	g_def_line(8, 1, 0, 0, 1, 4, 8);

	for(int i_time = 0;;i_time++)
	{
		if(i_time%INTV == 0)
		{
			g_cls();
			g_sel_scale_2D(0);
			g_sel_line(0);
			g_move_2D(-2,-1.9);
			g_plot_2D(2,-1.9);

			g_sel_line(1);
			g_move_2D(-2,-1.5);
			g_plot_2D(2,-1.5);

			g_sel_line(2);
			g_move_2D(-2,-1);
			g_plot_2D(2,-1);

			g_sel_line(3);
			g_move_2D(-2,-0.5);
			g_plot_2D(2,-0.5);

			g_sel_line(4);
			g_move_2D(-2,0);
			g_plot_2D(2,0);

			g_sel_line(5);
			g_move_2D(-2,0.5);
			g_plot_2D(2,0.5);

			g_sel_line(6);
			g_move_2D(-2,1);
			g_plot_2D(2,1);

			g_sel_line(7);
			g_move_2D(-2,1.5);
			g_plot_2D(2,1.5);

			g_sel_line(8);
			g_move_2D(-2,1.9);
			g_plot_2D(2,1.9);

			g_finish();
			g_sleep(0.05);
		}
	}
	return 0;
}
