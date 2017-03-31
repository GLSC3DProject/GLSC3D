#include "glsc3d.h"

#define WIRE_OR_FILL G_FILL
#define WIRE G_TRUE
#define FILL G_TRUE

int main(int argc, char *argv[])
{
	const int Width = 240, Height = 240;
	const int CountX = 5, CountY = 2;

	g_enable_highdpi();
	g_init("GLSC3D", Width * CountX, Height * CountY);

//	g_init_light(0, 0, 1, 2);
	g_init_light(0, 1, -1, 1);
//	g_init_light_core(1, 1, 1, 1, 0.25f);
	g_init_light_core(2, 0, 1, 2, 0.5f);

	//g_scr_color(1, 1, 1);
	//g_scr_color(0.5, 0.5, 0.5);

	g_area_color_3D(0.5, 0.75, 0.5, 1);
	//g_line_color(0, 0, 0, 1);

	g_line_width(2);
	g_line_type(1);

	g_text_color(0.5f, 0.5f, 0, 1);

	int id = 0;
	for (int i = 0; i < CountX; i++)
	for (int j = 0; j < CountY; j++)
		g_def_scale_3D_core(id++, -1, 1, -1, 1, -1, 1, Width * i, Height * j, Width, Height, 0, 1, 1, 8, 0, 0, 1);

	for (double t = 0;; t += 1./128) {
		double c = cos(t), s = sin(t);

		g_cls();

		g_sel_scale_3D_boundary(0);
		g_pyramid_3D_core (0, 0,-1, 0, 0, 1, 1, 2, t, 6, 0, WIRE_OR_FILL);

		g_sel_scale_3D(1);
		g_cone_3D_core    (0, 0,-1, 0, 0, 1, 1, 2,24, 6, 0, WIRE_OR_FILL);

		g_sel_scale_3D(2);
		g_prism_3D_core   (0, 0, 0, 0, 0, 1, 1, 2, t, 6, 0, WIRE, FILL);

		g_sel_scale_3D_boundary(3);
		g_cylinder_3D_core(0, 0, 0, 0, 0, 1, 1, 2, t,24, 0, WIRE, FILL);

		g_sel_scale_3D(4);
		g_ellipse_3D_core(0, 0, 0, 1.5, 1, 0.75, c, s, 0, 8, 0, WIRE_OR_FILL);

		g_sel_scale_3D(5);
		g_box_3D_core(0, 0, 0, 3, 2, 1.5, 0, WIRE_OR_FILL);

		g_sel_scale_3D(6);
		g_sphere_3D_core(0, 0, 0, 1, 8, 0, WIRE_OR_FILL);

		g_sel_scale_3D(7);
		g_rectangle_3D_core(0, 0, 0, c, s, 0, 3, 2, 0, 0, WIRE_OR_FILL);

		g_sel_scale_3D(8);
		g_text_size(24);
		g_arrow_3D_core(0, 0, 0, c, s, 0, 1, 0.25, 16, 1, 0, WIRE_OR_FILL);
		g_arrow_3D_core(0, 0, 0,-s, c, 0, 1, 0.25, 16, 1, 0, WIRE_OR_FILL);
		g_arrow_3D_core(0, 0, 0, 0, 0, 1, 1, 0.25, 16, 1, 0, WIRE_OR_FILL);
		g_text_3D_virtual( 1.1*c, 1.1*s, 0, "x");
		g_text_3D_virtual(-1.1*s, 1.1*c, 0, "y");
		g_text_3D_virtual(0, 0, 1, "z");

		g_sel_scale_3D(9);
		g_fan_3D_core(0, 0, 0, c, s, 0, 1, 2.0, 0, 4, 0, WIRE_OR_FILL);

		g_finish();
	}
	return 0;
}
