#include "glsc3d.h"

 #define WIRE_OR_FILL G_FILL

int main()
{
	const int Size = 320;
	
	g_init("Marker", Size*2, Size*2);
	
	g_init_light(0, 0, 0, -1);
	
//	g_scr_color(1, 1, 1);
	g_line_color(0, 0, 0, 1);
	g_line_width(4);
	
	g_def_scale_2D(0, -1.5, 1.5, -1.5, 1.5, 0, 0, Size, Size);
	g_def_scale_2D(1, -1.5, 1.5, -1.5, 1.5, Size, 0, Size, Size);
	
	g_def_scale_3D(2, -1, 1, -1, 1, -1, 1, 0, Size, Size, Size, 0, 0, 1, 1);
	
	for (double t = 0;; t += 1./128) {
//		double x = cos(t), y = sin(t);
		
		g_cls();
		
		g_sel_scale_2D(0);
		g_box_2D(0, 0, 2, 1, WIRE_OR_FILL);
		
		g_sel_scale_2D(1);
		g_box_2D_vert(-1, 1, -0.5, 0.5, WIRE_OR_FILL);
		
		g_sel_scale_3D(2);
		g_box_3D(0, 0, 0, 2, 1, 1.5);
		
		g_finish();
	}
	return 0;
}
