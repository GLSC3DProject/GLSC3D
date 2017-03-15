#include "glsc3d.h"

// #define WIRE_OR_FILL G_FILL

int main()
{
	const int Size = 320;

	g_init("GLSC3D", Size, Size);

//	g_scr_color(1, 1, 1);
//	g_line_color(0, 0, 0, 1);
	g_line_width(4);

	g_def_marker(0, 1, 0, 0, 1, 0, 32);

	g_def_scale_2D(0, -1.5, 1.5, -1.5, 1.5, 0, 0, Size, Size);
	
	for (double t = 0;; t += 1./128) {
		double x = cos(t), y = sin(t);

		g_cls();
		g_sel_scale_2D(0);

		g_box_2D(0, 0, 2, 2, G_WIRE);
		
		g_move_2D(+1, +1);
		g_plot_2D(-1, -1);
		g_move_2D(+1, -1);
		g_plot_2D(-1, +1);

		g_sel_marker(0);
		g_marker_2D(0, 0);

		g_marker_color(0, 1, 0, 1);
		g_marker_type(1);
		g_marker_size(16);
		g_marker_2D(+x, +y);
		g_marker_2D(-x, -y);

		g_finish();
	}
	return 0;
}
