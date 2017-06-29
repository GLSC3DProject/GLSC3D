#include<stdio.h>

#include "glsc3d_3.h"

#define INTV              (1)
#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)
#define WINDOW_POS_X      (50)
#define WINDOW_POS_Y      (50)

int main()
{
	g_init("Sample_g_area.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	g_init_light(0, 1, 1, 1); // 立方体を確認するためにライトの位置を初期位置から変更

	g_def_scale_3D(0, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2, 4, 4, 4, 1);
	g_def_scale_3D(1, -2, 2, -2, 2, -2, 2, WINDOW_SIZE_X / 4, 0, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2, 4, 4, 4, 1);
	g_def_scale_3D(2, -2, 2, -2, 2, -2, 2, 2 * WINDOW_SIZE_X / 4, 0, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2, 4, 4, 4, 1);
	g_def_scale_3D(3, -2, 2, -2, 2, -2, 2, 3 * WINDOW_SIZE_X / 4, 0, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2, 4, 4, 4, 1);

	g_def_scale_2D(4, -2, 2, -2, 2, 0, WINDOW_SIZE_Y / 2, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2);
	g_def_scale_2D(5, -2, 2, -2, 2, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2);
	g_def_scale_2D(6, -2, 2, -2, 2, 2 * WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2);
	g_def_scale_2D(7, -2, 2, -2, 2, 3 * WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2, WINDOW_SIZE_X / 4, WINDOW_SIZE_Y / 2);

	g_def_area_3D(0, 1, 0, 0, 1);
	g_def_area_3D(1, 0, 1, 0, 1);
	g_def_area_3D(2, 0, 0, 1, 1);
	g_def_area_3D(3, 1, 1, 1, 1);

	g_def_area_2D(0, 1, 0, 0, 1);
	g_def_area_2D(1, 0, 1, 0, 1);
	g_def_area_2D(2, 0, 0, 1, 1);
	g_def_area_2D(3, 1, 1, 1, 1);

	for (int i_time = 0;; i_time++)
	{
		if (i_time%INTV == 0)
		{
			g_cls();

			g_sel_scale_2D(4);
			g_sel_area_2D(0);

			g_box_center_2D(0, 0, 1, 1, G_NO, G_YES);

			g_sel_scale_2D(5);
			g_sel_area_2D(1);
			g_box_center_2D(0, 0, 1, 1, G_NO, G_YES);

			g_sel_scale_2D(6);
			g_sel_area_2D(2);
			g_box_center_2D(0, 0, 1, 1, G_NO, G_YES);

			g_sel_scale_2D(7);
			g_sel_area_2D(3);
			g_box_center_2D(0, 0, 1, 1, G_NO, G_YES);

			g_sel_scale_3D(0);
			g_sel_area_3D(0);
			g_box_center_3D(0, 0, 0, 2, 2, 2, G_NO, G_YES);

			g_sel_scale_3D(1);
			g_sel_area_3D(1);
			g_box_center_3D(0, 0, 0, 2, 2, 2, G_NO, G_YES);

			g_sel_scale_3D(2);
			g_sel_area_3D(2);
			g_box_center_3D(0, 0, 0, 2, 2, 2, G_NO, G_YES);

			g_sel_scale_3D(3);
			g_sel_area_3D(3);
			g_box_center_3D(0, 0, 0, 2, 2, 2, G_NO, G_YES);

			g_finish();
			g_sleep(0.05);
		}
	}
	return 0;
}
