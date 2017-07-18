#include <stdio.h>
#include "glsc3d_3.h"

#define PI M_PI

int main()
{
	double scale = 1.5;
	double t = 0, dt = 0.01;
	G_POSITION eye[4];
	double shu = 10;

	g_init_core("Sample_g_transparent.c", 1280, 960, 0, 60, 1, 1, 1, G_TRUE, 0, 0);

	while(t < shu * 2 * PI)
	{
		t += dt;
		for(int id = 0; id < 4; id ++)
		{
			eye[id].x = cos(t);
			eye[id].y = sin(t);
			eye[id].z = cos(t + id / 4.0 * 2 * PI);
		}
		g_def_scale_3D_fix(0, -scale, scale, -scale, scale, -scale, scale,
			4 * scale * eye[0].x, 4 * scale * eye[0].y, 4 * scale * eye[0].z, 0, 0, 1, 1, 0, 0, 640, 480);
		g_def_scale_3D_fix(1, -scale, scale, -scale, scale, -scale, scale,
			4 * scale * eye[1].x, 4 * scale * eye[1].y, 4 * scale * eye[1].z, 0, 0, 1, 1, 640, 0, 640, 480);
		g_def_scale_3D_fix(2, -scale, scale, -scale, scale, -scale, scale,
			4 * scale * eye[2].x, 4 * scale * eye[2].y, 4 * scale * eye[2].z, 0, 0, 1, 1, 0, 480, 640, 480);
		g_def_scale_3D_fix(3, -scale, scale, -scale, scale, -scale, scale,
			4 * scale * eye[3].x, 4 * scale * eye[3].y, 4 * scale * eye[3].z, 0, 0, 1, 1, 640, 480, 640, 480);

		g_init_light_core(0,1,1,1,1); //透明化処理のため, デフォルトから初期値を変更
		g_init_light_core(1,-1,-1,-1,1); //透明化処理のため, デフォルトから初期値を変更

		g_area_color_3D(1, 0, 0, 0.5); //透明化処理のため, デフォルトから初期値を変更

		g_cls();
		for(int id = 0;id < 4;id ++)
		{
			g_sel_scale(id);

			g_arrow_3D(-2*scale, 0, 0, 1, 0, 0, 4*scale, 0.1*scale, G_NO, G_YES);

			g_arrow_3D(0, -2*scale, 0, 0, 1, 0, 4*scale, 0.1*scale, G_NO, G_YES);

			g_arrow_3D(0, 0, -2*scale, 0, 0, 1, 4*scale, 0.1*scale, G_NO, G_YES);

			g_cone_3D_core(0.5, 1, 0.5, 1, 1, 1, 1, 2, 100, 0, G_NO, G_YES);
		}
		g_finish();
	}
}
