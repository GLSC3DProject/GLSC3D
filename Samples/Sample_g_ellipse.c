#include<stdio.h>

#include "glsc3d_3.h"
#include "glsc3d_3_private.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_ellipse.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	g_def_scale_3D(0,
		-2, 2, -2, 2, -2, 2,
		-2, 2, -2, 2, -2, 2,
		0, 0, WINDOW_SIZE_X*0.5, WINDOW_SIZE_Y);
	g_def_scale_3D(1,
		-2, 2, -2, 2, -2, 2,
		-2, 2, -2, 2, -2, 2,
		WINDOW_SIZE_X*0.5, 0, WINDOW_SIZE_X*0.5, WINDOW_SIZE_Y);

	for (int i_time = 0;; i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_box_center_3D(0,0,0, 4,4,4, G_YES, G_NO);
		g_ellipse_3D(
			0, 0, 0,
			2, 1, 1,
			1, 0, 0,
			G_NO, G_YES);

		/* 試験中 g_ellipse_3D_Euler (core仕様) */
		g_sel_scale(1);
		g_box_center_3D(0,0,0, 4,4,4, G_YES, G_NO);
		g_ellipse_3D_Euler(
			0, 0, 0,
			2, 1, 1,
			2.7646, 2.33734, 0,
			6, 0,
			G_YES, G_YES);

		G_VECTOR r0 = {0,0,1};
		G_VECTOR r1 = Rz(Rx(Rz(r0, 2.7646),2.33734), 0);
		g_arrow_3D(
			0,0,0,
			r1.x, r1.y, r1.z,
			2, 0.2, G_NO, G_YES);

		g_finish();
	}
	return 0;
}
