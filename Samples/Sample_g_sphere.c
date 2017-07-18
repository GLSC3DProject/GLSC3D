#include "glsc3d_3.h"

int main(void)
{
	g_init("Sample_g_sphere.c", 640, 480);

	double scale = 1;
	g_def_scale_3D_fix(0, -scale, scale, -scale, scale, -scale, scale, 4, 4, 4, 0, 1, 0, scale, 0, 0, 320, 480);
	g_def_scale_3D_fix(1, -scale, scale, -scale, scale, -scale, scale, 4, 4, 4, 0, 1, 0, scale, 320, 0, 320, 480);

	while(1)
	{
		g_cls();

		g_sel_scale(0);
		g_sphere_3D(0, 0, 0, scale, G_YES, G_NO);

		g_sel_scale(1);
		g_sphere_3D_core(0, 0, 0, scale, 10, 0, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
