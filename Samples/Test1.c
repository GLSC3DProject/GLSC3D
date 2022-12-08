#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (1000)

#define ENABLE_TRANSPARENT

int main()
{
#ifdef ENABLE_TRANSPARENT
	g_init_core("Test1.c", WINDOW_SIZE_X, WINDOW_SIZE_Y, 0, 60, 0.95, 0.95, 0.95, 1, 1 << 16, 1 << 16);
	const float alpha = 0.5f;
#else
	g_init("Test1.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	const float alpha = 1;
#endif

	g_def_scale_3D(0, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	int i_time;
	for(i_time = 0;;i_time++)
	{
		g_cls();

		g_sel_scale(0);
		double t = i_time / 64.;
		double u = cos(t), v = sin(t);

		g_area_color(1, 0, 0, alpha);
		g_sphere_3D(u, v, 0, 0.95, G_NO, G_YES);

		g_area_color(0, 1, 0, alpha);
		g_sphere_3D(-u, -v, 0, 0.95, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
