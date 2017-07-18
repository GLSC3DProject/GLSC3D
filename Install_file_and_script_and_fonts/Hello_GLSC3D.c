#include<stdio.h>
#include<glsc3d_3.h>

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Hello GLS3D", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0, -2, 2, -2, 2, WINDOW_SIZE_X, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

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
