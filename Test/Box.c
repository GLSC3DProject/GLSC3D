#include "glsc3d.h"

 #define WIRE_OR_FILL G_WIRE

int main()
{
	const int s = 320;
	
	g_init("GLSC3D", s*2, s*2);
	
	g_scr_color(1, 1, 1);
	
//	g_area_color_2D(1, 1, 0, 1);
	g_line_color(0, 0, 0, 1);

	g_def_scale_2D(0, -1.5, 1.5, -1.5, 1.5, 0, 0, s, s);
	g_def_scale_2D(1, -1.5, 1.5, -1.5, 1.5, s, 0, s, s);
	
	g_def_scale_3D(2, -1, 1, -1, 1, -1, 1, 0, s, s, s, 0, 1, 2, 8);
	g_def_scale_3D(3, -1, 1, -1, 1, -1, 1, s, s, s, s, 0, 1, 2, 8);

	for (double t = 0;; t += 1./128) {
		g_cls();
		
		g_sel_scale_2D(0);
		g_area_color_2D(1, 1, 0, 1);
		g_box_2D(0, 0, 2, 1, WIRE_OR_FILL);
		
//		g_sel_scale_2D(1);
//		g_area_color_2D(1, 0, 0, 1);
//		g_box_2D_vert(-1, 1, -0.5, 0.5, WIRE_OR_FILL);
		
		g_sel_scale_3D(2);
		g_area_color_3D(0.5, 0.75, 0.5, 1);
		g_box_3D_core(0, 0, 0, 2, 1, 1.5, 0, WIRE_OR_FILL);
		
//		g_sel_scale_3D(3);
//		g_box_3D_vert_core(-1, 1, -0.5, 0.5, -0.75, 0.75, 0, WIRE_OR_FILL);
		
		g_finish();
	}
	return 0;
}
