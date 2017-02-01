#include<stdlib.h>
#include<stdio.h>

#include "glsc3d.h"

//#define g_isosurface_3D(x0, x1, y0, y1, z0, z1, number_x, number_y, number_z, u, height) (g_isosurface_3D((x0), (x1), (y0), (y1), (z0), (z1), (number_x), (number_y), (number_z),(double(*)[(number_y)][(number_z)])(u), height))


#define INTV              (1)
#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

#define LEN         (2.0)
#define XLEN        (LEN)
#define YLEN        (LEN)
#define ZLEN        (LEN)

#define SIZE        (40)
#define Imax        (SIZE)
#define Jmax        (SIZE)
#define Kmax        (SIZE)

#define SQ(i)       ((i) * (i))
#define v(i,j,k)    (v[(k) * (Jmax) * (Imax) + (j) * (Imax) + (i)])

double f(double x,double y,double z,double x0,double y0,double z0)
{
	double ans = 0;
	
	ans += pow((x - x0),2.0);
	ans += pow((y - y0),2.0);
	ans += pow((z - z0),2.0);
	ans = exp (-ans / 0.2);
	
	return ans;
}

int main()
{
	g_init("GRAPH", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	
	double v[Imax * Jmax * Kmax],v2[Imax][Jmax][Kmax];
	double d_x, d_y, d_z;
	double r_x, r_y, r_z;
	double r0_x, r0_y, r0_z;

	int i,j,k;
	
	d_x = XLEN / Imax;
	d_y = YLEN / Jmax;
	d_z = ZLEN / Kmax;

	r0_x = 0.0;
	r0_y = 0.0;
	r0_z = 0.0;

	g_def_scale_3D(0,
					-1, 1, -1, 1, -1, 1,
					0, 0,
					WINDOW_SIZE_X, WINDOW_SIZE_Y,
					1, 1, 1,
					1);
	g_init_light_core(0,1,1,1,1);
	int i_time;
	double t,dt = 0.1;
	for(i_time = 0;;i_time++)
	{
		t = i_time * dt;
		
		r0_x = sin(t) / 2;
		r0_y = cos(t) / 2;
		r0_z = sin(t) / 2;
		for(k = 0;k < Kmax;k ++)
		{
			r_z = (k + 0.5) * d_z - ZLEN / 2;
			for (j = 0; j < Jmax; j ++)
			{
				r_y = (j + 0.5) * d_y - YLEN / 2;
				for(i = 0;i < Imax;i ++)
				{
					r_x = (i + 0.5) * d_x - XLEN / 2;
					v(i, j, k) = f(r_x,r_y,r_z,r0_x,r0_y,r0_z)
					+ f(r_x,r_y,r_z,-r0_x,r0_y,r0_z)
					+ f(r_x,r_y,r_z,r0_x,-r0_y,r0_z)
					+ f(r_x,r_y,r_z,r0_x,r0_y,-r0_z);
					v2[i][j][k] = f(r_x,r_y,r_z,r0_x,r0_y,r0_z)
					+ f(r_x,r_y,r_z,-r0_x,r0_y,r0_z)
					+ f(r_x,r_y,r_z,r0_x,-r0_y,r0_z)
					+ f(r_x,r_y,r_z,r0_x,r0_y,-r0_z);
				}
			}
		}
		
		g_scr_color(0, 0, 0);
		g_cls();
		g_sel_scale_3D(0);
		g_area_color_3D(1, 0, 0, 1);
		
		g_isosurface_f_3D(-XLEN / 2 + 0.5 * d_x, XLEN / 2 - 0.5 * d_x,
						-YLEN / 2 + 0.5 * d_y, YLEN / 2 - 0.5 * d_y,
						-ZLEN / 2 + 0.5 * d_z, ZLEN / 2 - 0.5 * d_z,
						Imax, Jmax, Kmax,v, 0.5);

		g_isosurface_3D(-XLEN / 2 + 0.5 * d_x, XLEN / 2 - 0.5 * d_x,
						-YLEN / 2 + 0.5 * d_y, YLEN / 2 - 0.5 * d_y,
						-ZLEN / 2 + 0.5 * d_z, ZLEN / 2 - 0.5 * d_z,
						Imax, Jmax, Kmax,v2, 0.5);
		 
		
		g_finish();
	}
	return 0;
}
