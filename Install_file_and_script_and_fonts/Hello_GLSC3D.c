#include<stdio.h>
#include<glsc3d_3.h>

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Hello GLS3D", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0, -2, 2, -2, 2, -2, 2, WINDOW_SIZE_X, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);
//	void g_def_scale_2D(
//		int id,                                                         //id
//		double x_left, double x_right, double y_bottom, double y_top,   //仮想座標系
//		double x_left_std, double y_top_std,                            //ウィンドウの位置
//		double width_std, double height_std)

	for (int i_time = 0;; i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_text_size(32);
		g_text_color(0, 0, 0, 1);
		g_text_standard(130, 250, "Hello GLSC3D!!");

		g_finish();
	}
	return 0;
}
