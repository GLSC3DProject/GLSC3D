#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

#define LEN         (2.0)
#define XLEN        (LEN)
#define YLEN        (LEN)
#define ZLEN        (LEN)

#define SIZE        (40)
#define Imax        (SIZE)
#define Jmax        (2 * SIZE)

int main()
{
	g_init("Sample_g_contln.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	int i, j;
	double xx, yy, dx = 2 * M_PI / Imax, dy = 2 * M_PI / Jmax;

	double array[Imax*Jmax];
	double array2[Imax][Jmax];

	for (int i_time = 0;; i_time++)
	{
		for (i = 0; i < Imax; i++)
		{
			xx = dx * (i + 0.5) - M_PI;
			for (j = 0; j < Jmax; j++)
			{
				yy = dy * (j + 0.5) - M_PI;
				double tmp = sin(xx) * cos(2 * yy);
//				tmp = xx * xx + yy * yy - 2;
				array[i*Jmax + j] = array2[i][j] = tmp;
			}
		}

		g_cls();
		g_sel_scale(0);
		g_line_color(1.0, 0.0, 0.0, 1.0);
		g_contln_f_2D(-1.9, 1.9, -1.9, 1.9, Imax, Jmax, array, -0.5);
		g_line_color(0.0, 1.0, 0.0, 1.0);
		g_contln_2D(-1.9, 1.9, -1.9, 1.9, Imax, Jmax, array2, 0.5);

		g_finish();
		g_sleep(10);
	}
	return 0;
}
