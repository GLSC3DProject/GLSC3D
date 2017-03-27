#include "glsc3d.h"
#include <stdio.h>

int main()
{
	int down_x = 65536, down_y = 65536, up_x = 65536, up_y = 65536, x, y;
	int key = 0;
	int text_y = 20, text_x = 20, text_height = 25;
	int count = 0;

	g_init("Sample_g_input", 640, 480);

	g_def_scale_2D(0, 0, 640, 480, 0, 0, 0, 640, 480);
	g_sel_scale_2D(0);
	g_def_text(0, 1, 1, 1, 1, 0, 20);
	g_def_text(1, 0, 0, 0, 1, 0, 20);

	while (1)
	{
		g_sleep(0.01);
		g_cls();
		g_circle_2D(down_x, down_y, 20, 1);
		g_circle_2D(up_x, up_y, 20, 1);
		g_sel_text(1);
		g_text_size(24);
		g_area_color_2D(1, 1, 1, 1);
		g_box_2D(545, 75, 150, 90, 1);
		g_text_2D_virtual(500, 68, "Exit");
		g_text_2D_virtual(500, 93, "Program");

		for (int i = 0; i < 128; ++i)
			if(g_input_state(i, 0, 0) == G_DOWN)
				key = i;

		G_INPUT_STATE mouse_state = g_input_state(G_MOUSE_LEFT, &x, &y);
		if (mouse_state == G_DOWN) {
			down_x = x, down_y = y;
		} else if (mouse_state == G_UP) {
			up_x = x, up_y = y;
		}

		if (500 <= up_x && up_x <= 620 && 20 <= up_y && up_y <= 140)
			break;

		g_sel_text(0);
		text_y = 40;
		if (count++ / 32 % 2 == 0)
			g_text_2D_virtual(text_x, text_y, "Please input any key or click");
		else
			g_text_2D_virtual(text_x, text_y, "Please input any key or click:");

		g_text_2D_virtual(text_x, text_y += text_height, "Input character  %s", g_key_code_string[key]);
		g_text_2D_virtual(text_x, text_y += text_height, "Mouse down x:%d y:%d", down_x, down_y);
		g_text_2D_virtual(text_x, text_y += text_height, "Mouse up   x:%d y:%d", up_x, up_y);
		g_finish();
		fflush(stdout);
	}
}
