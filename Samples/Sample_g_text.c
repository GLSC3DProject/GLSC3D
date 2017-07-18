#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (320)
#define WINDOW_SIZE_Y     (320)

int main()
{
	g_init("Sample_g_text.c",WINDOW_SIZE_X,WINDOW_SIZE_Y);

	g_def_scale_2D(0,-2, 2, -2, 2,0, 0,WINDOW_SIZE_X, WINDOW_SIZE_Y);

	char TEXT2[20];

	for(int i_time = 0;;i_time++)
	{
		g_cls();

		g_sel_scale(0);

		time_t time_val = time(0);
		strftime(TEXT2, sizeof(TEXT2), "%Y/%m/%d %H:%M:%S", localtime(&time_val));

		int x = 20, y = 0, dy = 25;
		g_text_size(24);

		// 1-byte characters
		g_text_standard(x, y += dy, "Hello, World!");

		// 3-byte characters
		g_text_standard(x, y += dy, "こんにちは，世界!");

		// 2-byte characters
		g_text_standard(x, y += dy, "∀∃∧∂∩∈∬∮≈");

		g_text_standard(x, y += dy, TEXT2);

		g_text_standard(x, y += dy, "Elapsed time = %d", i_time);

		y += dy;

		g_text_size(12);
		g_text_standard(x, y += 12, "12pt");

		g_text_size(18);
		g_text_standard(x, y += 18, "18pt");

		g_text_size(24);
		g_text_standard(x, y += 24, "24pt");

		g_text_size(32);
		g_text_standard(x, y += 32, "32pt");

		g_text_size(48);
		g_text_standard(x, y += 48, "48pt");

		g_finish();
		g_sleep(1.);
	}
	return 0;
}

