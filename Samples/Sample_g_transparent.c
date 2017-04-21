#include <stdio.h>
#include "glsc3d_3.h"

#define PI M_PI

int main()
{
	double scale = 1;
	double t = 0, dt = 0.01;
	G_POSITION eye[4];
	double shu = 10;

	g_init_core("Sample_g_transparent.c", 1280, 960, 50, 50, 1, 1, 1, G_TRUE, 0, 0);

	while(t < shu * 2 * PI)
	{
		t += dt;
		for(int id = 0; id < 4; id ++)
		{
			eye[id].x = cos(t);
			eye[id].y = sin(t);
			eye[id].z = cos(t + id / 4.0 * 2 * PI);
		}
		g_def_scale_3D_core(0, -scale, scale, -scale, scale, -scale, scale,   0,   0, 640, 480, eye[0].x, eye[0].y, eye[0].z, scale*20, 0, 0, 1);
		g_def_scale_3D_core(1, -scale, scale, -scale, scale, -scale, scale, 640,   0, 640, 480, eye[1].x, eye[1].y, eye[1].z, scale*20, 0, 0, 1);
		g_def_scale_3D_core(2, -scale, scale, -scale, scale, -scale, scale,   0, 480, 640, 480, eye[2].x, eye[2].y, eye[2].z, scale*20, 0, 0, 1);
		g_def_scale_3D_core(3, -scale, scale, -scale, scale, -scale, scale, 640, 480, 640, 480, eye[3].x, eye[3].y, eye[3].z, scale*20, 0, 0, 1);

		g_init_light_core(0,1,1,1,1); //透明化処理のため, デフォルトから初期値を変更
		g_init_light_core(1,-1,-1,-1,1); //透明化処理のため, デフォルトから初期値を変更

		g_area_color_3D(1, 0, 0, 0.5); //透明化処理のため, デフォルトから初期値を変更

		g_cls();
		for(int id = 0;id < 4;id ++)
		{
			g_sel_scale_3D(id);

			g_arrow_3D(-2*scale, 0, 0, 1, 0, 0, 4*scale, 0.1*scale, G_NO, G_YES);

			g_arrow_3D(0, -2*scale, 0, 0, 1, 0, 4*scale, 0.1*scale, G_NO, G_YES);

			g_arrow_3D(0, 0, -2*scale, 0, 0, 1, 4*scale, 0.1*scale, G_NO, G_YES);

			g_cone_3D_core(0.5, 1, 0.5, 1, 1, 1, 1, 2, 100, 0, G_NO, G_YES);
		}
		g_finish();
	}
}
