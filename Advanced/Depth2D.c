#include "glsc3d_3.h"

int main()
{
	const int Size = 128;
	const int Width = 7*Size, Height = 3*Size;
	const double radius = 1./16;

	g_enable_highdpi();
	g_set_antialiasing(2);
	g_init("GLSC3D", Width, Height);

//	g_scr_color(1, 1, 1);
//	g_line_color(0, 0, 0, 1);
	g_line_width(2);

	g_def_scale_2D(0, -3.5, 3.5, -1.5, 1.5, 0, 0, Width, Height);

	g_marker_color(1, 0, 0, 1);
	g_marker_radius(radius);

	for (;;) {
		g_cls();
		g_sel_scale(0);

		int n = 0;
		for (double s = 0; s < 1; s += 1./128) {
			double x = 2*M_PI*s - M_PI, y = sin(2*M_PI*s);

			switch (n % 4) {
			case 0:
				g_marker_type((n / 16) % 2);
				g_marker_2D(x, y);
				break;
			case 1:
				g_line_color(0, 0, 1, 1);
				g_circle_2D(x, y, radius, G_YES, G_NO);
				break;
			case 2:
				g_area_color(0, 1, 0, 1);
				g_circle_2D(x, y, radius, G_NO, G_YES);
				break;
			case 3:
				g_area_color(1, 1, 0, 1);
				g_line_color(0, 0, 0, 1);
				g_circle_2D(x, y, radius, G_YES, G_YES);
				break;
			}

			n++;
		}

		g_finish();
	}
	return 0;
}
