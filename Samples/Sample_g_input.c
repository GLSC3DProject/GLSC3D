#include <stdio.h>

#include "glsc3d_3.h"

int main()
{
	int down_x = 65536, down_y = 65536, up_x = 65536, up_y = 65536, x, y;
	int key = 0;
	int text_y = 20, text_x = 20, text_height = 25;
	int count = 0;
	const char* state_string[] = { "G_NONE", "G_DOWN", "G_UP", "G_REPEAT" };

	g_init("Sample_g_input", 640, 480);

	g_def_scale_2D(0, 0, 640, 480, 0, 0, 0, 640, 480);
	g_sel_scale(0);

	while (1)
	{
		g_cls();
		g_circle_2D(down_x, down_y, 20, G_YES,G_YES);
		g_circle_2D(up_x, up_y, 20, G_YES,G_YES);

		g_text_size(24);
		g_box_center_2D(545, 75, 150, 90, G_YES, G_NO);
		g_text_2D_virtual(500, 68, "Exit");
		g_text_2D_virtual(500, 93, "Program");

		for (int i = 0; i < 128; ++i)
			if (g_input_state(i, 0, 0) == G_DOWN)
				key = i;

		G_INPUT_STATE mouse_state = g_input_state(G_MOUSE_LEFT, &x, &y);
		if (mouse_state == G_DOWN) {
			down_x = x, down_y = y;
		} else if (mouse_state == G_UP) {
			up_x = x, up_y = y;
		}

		if (500 <= up_x && up_x <= 620 && 20 <= up_y && up_y <= 140)
			break;

		text_y = 40;
		if (count++ / 32 % 2 == 0)
			g_text_2D_virtual(text_x, text_y, "Please input any key or click");
		else
			g_text_2D_virtual(text_x, text_y, "Please input any key or click:");

		g_text_2D_virtual(text_x, text_y += text_height, "Input character  %s", g_key_code_string[key]);
		g_text_2D_virtual(text_x, text_y += text_height, "Key state : %s", state_string[g_key_state(key)]);
		g_text_2D_virtual(text_x, text_y += text_height, "Mouse down x:%d y:%d", down_x, down_y);
		g_text_2D_virtual(text_x, text_y += text_height, "Mouse up   x:%d y:%d", up_x, up_y);
		g_text_2D_virtual(text_x, text_y += text_height, "Mouse state : %s", state_string[mouse_state]);

//		g_sleep(0.1);
		g_finish();
		fflush(stdout);
	}
}
