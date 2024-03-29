#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

#define SIZE        (200)
#define Imax        (SIZE)
#define Jmax        (SIZE)

int main()
{
	g_init("Sample_g_data_plot.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	g_def_scale_2D(0, -2, 2, -2, 2, 0, 0,WINDOW_SIZE_X/2, WINDOW_SIZE_Y);
	g_def_scale_3D(1, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	int i,j;
	double xx,yy,dx=4.0/Imax,dy=4.0/Jmax;
//	double array[Imax*Jmax];
	double array2[Imax][Jmax];
	double array3[10];

	for(int i_time = 0;;i_time++)
	{
		for (i = 0; i < Imax; i++)
		{
			xx = dx * i;
			for (j = 0; j < Jmax; j++)
			{
				yy = dy * j;
//				array[i*Jmax+j] = sin(2*xx - 1) * cos(3*yy - 0.5);
				array2[i][j] = sin(2*xx - 1) * cos(3*yy - 0.5);
			}
		}
		for (i = 0; i < 10; i++)
		{
			xx = 0.4 * i;
			array3[i] = sin(2*xx - 1);
		}
		g_cls();

		g_sel_scale(0);
		g_marker_size(5); //Markerを大きく表示するため, デフォルト値から変更している
		g_data_plot_2D(-2, 2, array3, 10);

		g_sel_scale(1);
		g_marker_size(0.5);
		g_data_plot_3D(-2, 2, -2, 2, Imax, Jmax, array2);
		//g_data_plot_f_3D(-2, 2, -2, 2, Imax, Jmax, array);

		g_finish();
	}
	return 0;
}
