#include<stdlib.h>
#include<stdio.h>

#include "glsc3d.h"

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

#define LEN         (2.0)
#define XLEN        (LEN)
#define YLEN        (LEN)
#define ZLEN        (LEN)

#define SIZE        (40)
#define Imax        (SIZE)
#define Jmax        (SIZE*2)
#define Kmax        (SIZE)

int main()
{
	g_init("GRAPH", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	g_scr_color(1, 1, 1);
	g_line_width(4);
	
	g_def_scale_2D(0,-2, 2, -2, 2,0, 0,WINDOW_SIZE_X, WINDOW_SIZE_Y);
	
	int i_time;
	for(i_time = 0;;i_time++)
	{
		int i,j;
		double xx,yy,dx=4.0/Imax,dy=4.0/Jmax;
		double array[Imax*Jmax];
		double array2[Imax][Jmax];
		for (i = 0; i < Imax; i++)
		{
			xx = dx * i;
			for (j = 0; j < Jmax; j++)
			{
				yy = dy * j;
				//printf("x = %f, y = %f\n", xx, yy);
				array[i*Jmax + j] = sin(2*xx - 1) * cos(3*yy - 0.5);
				//array[i*Jmax + j] = xx*xx + yy*yy;
				array2[i][j] = sin(2*xx - 1) * cos(3*yy - 0.5);
			}
		}
		
		g_cls();
		g_sel_scale_2D(0);
		g_line_color(1,0,0,1);
		g_contln_f_2D(-1.9, 1.9, -1.9, 1.9, Imax, Jmax, array, 0.05);
		//g_contln_2D(-1.9, 1.9, -1.9, 1.9, Imax, Jmax, array2, 0.05);
		
		g_finish();
	}
	return 0;
}
