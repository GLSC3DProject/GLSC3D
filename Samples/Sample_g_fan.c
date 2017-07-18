#include<stdio.h>
#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_fan.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);

	g_def_scale_3D(1, -2, 2, -2, 2, -2, 2, WINDOW_SIZE_X / 2, 0, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y, 4, 4, 4, 1);

	double dt = 0.1;
	double theta;
	for (int i_time = 0;; i_time++)
	{
		g_cls();
		theta = 0.1*i_time*dt;
		g_sel_scale(0);
		g_fan_2D(-1, -1, cos(theta), sin(theta), 2, M_PI / 6, G_YES, G_NO);
		g_fan_2D(0, 0, cos(theta), sin(theta), 1, M_PI / 6, G_NO, G_YES);

		g_sel_scale(1);
		g_fan_3D(
			0, 0, 0,
			0, -1, 0,
			3, M_PI / 6, 0.01*i_time,
			G_YES, G_NO);
		g_fan_3D(0, 0, 0, 0, 1, 0, 3, M_PI / 6, 0.01*i_time, G_NO, G_YES);

		g_finish();

	}
	return 0;
}
