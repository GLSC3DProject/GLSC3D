#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X (1024)
#define WINDOW_SIZE_Y (512)

#define LEN (4.0)

int main(void)
{
	g_init("Sample_g_clipping.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_2D(0, -LEN / 2.0, LEN / 2.0, -LEN / 2.0, LEN / 2.0, WINDOW_SIZE_X/8, WINDOW_SIZE_Y/4,
				   WINDOW_SIZE_X*(2.0/8), WINDOW_SIZE_Y*(2.0/4));

	g_def_scale_2D(1, -LEN / 2.0, LEN / 2.0, -LEN / 2.0, LEN / 2.0, WINDOW_SIZE_X*(5.0/8), WINDOW_SIZE_Y/4,
				   WINDOW_SIZE_X*(2.0/8), WINDOW_SIZE_Y*(2.0/4));

	g_def_scale_2D(2, -LEN / 2.0, LEN / 2.0, -LEN / 2.0, LEN / 2.0, 0.0, 0.0,
				   WINDOW_SIZE_X/2, WINDOW_SIZE_Y);

	g_def_scale_2D(3, -LEN / 2.0, LEN / 2.0, -LEN / 2.0, LEN / 2.0, WINDOW_SIZE_X/2, 0.0,
				   WINDOW_SIZE_X/2, WINDOW_SIZE_Y);

	double t = 0.0;
	double dt = 0.002;

	double r = 2.5;

	for(int i_time = 0; ; i_time++)
	{
		t = dt*i_time;

		g_cls();

		g_sel_scale(0);
		g_boundary();
		g_clipping(G_NO);

		g_sel_marker(0);
		g_area_color_2D(1, 0, 0, 1);
		g_circle_2D(r * cos(2*M_PI*t), r * sin(2*M_PI*t), 0.2, G_NO, G_YES);

		g_sel_marker(1);
		g_area_color_2D(0, 0, 1, 1);
		g_circle_2D(cos(2*M_PI*t), sin(2*M_PI*t), 0.2, G_NO, G_YES);


		g_sel_scale(1);
		g_boundary();
		g_clipping(G_YES);

		g_area_color_2D(1, 0, 0, 1);
		g_circle_2D(r * cos(2*M_PI*t), r * sin(2*M_PI*t), 0.2, G_NO, G_YES);

		g_sel_marker(1);
		g_area_color_2D(0, 0, 1, 1);
		g_circle_2D(cos(2*M_PI*t), sin(2*M_PI*t), 0.2, G_NO, G_YES);

		g_sel_scale(2);
		g_boundary();
		g_text_standard(WINDOW_SIZE_X*(0.18), WINDOW_SIZE_Y*(0.85), "Clipping OFF");

		g_sel_scale(3);
		g_boundary();
		g_text_standard(WINDOW_SIZE_X*(0.185) + WINDOW_SIZE_X*0.5, WINDOW_SIZE_Y*(0.85), "Clipping ON");

		g_finish();
	}
	return 0;
}
