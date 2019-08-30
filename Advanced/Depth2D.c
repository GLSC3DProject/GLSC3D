#include "glsc3d_3.h"
#include <stdlib.h>
#define PI (3.14159265358979)
int main() {
	const int Width = 1000, Height = 1000;
	g_enable_highdpi();
	g_set_antialiasing(4);
//	g_init("GLSC3D", Width, Height);
	g_init_core("GLSC3D",Width,Height,0,0,
		0,0,0,
		1,
		1 << 22,
		1 << 26);

	g_line_width(2);

	//g_def_scale_2D(0, -1.0, 1.0, -1.0, 1.0, 0, 0, Width, Height);
	g_def_scale_3D(0,
		-1.0,1.0,
		-1.0,1.0,
		-1.0,1.0,
		-1.0,1.0,
		-1.0,1.0,
		-1.0,1.0,
		0,0,
		Width, Height
		);

	for (int i_time = 0;; i_time++)
	{
		g_cls();
		g_sel_scale(0);
		g_line_color(1,1,1,1);
		g_boundary();

		srand(0);
		int Imax = 1 << 4;
		for(int i = 0;i < Imax;i ++)
		{

//			double theta = 2 * PI / Imax * i;
//			G_VECTOR b = {0,0,0};
//			G_VECTOR a = {0.5 * cos(theta),0.5 * sin(theta),1};
//			G_VECTOR c = {0.5 *sqrt(2) * cos(theta + PI / 4),0.5 *sqrt(2) * sin(theta+ PI / 4),0};

			G_COLOR color;

			srand(0);

			color.r = rand() / (RAND_MAX + 1.0);
			color.g = rand() / (RAND_MAX + 1.0);
			color.b = rand() / (RAND_MAX + 1.0);
			color.a = 1;

			g_area_color_s(color);
			g_line_color_s(color);
			//g_triangle_2D_s(a,b,c,1,1);
//			g_triangle_3D_s(a,b,c,1,1);
			//g_triangle_3D_core_s(a,b,c,4,1,1);
			g_sphere_3D_core(0,0,0,
				1,
				25,
				2,
				1,1);

//			color.r = rand() / (RAND_MAX + 1.0);
//			color.g = rand() / (RAND_MAX + 1.0);
//			color.b = rand() / (RAND_MAX + 1.0);
//			color.a = 1;
//
//			//g_marker_type(i % 2);
//			g_marker_type(0);
//			g_marker_color_s(color);
//			g_marker_size(10);
//			g_marker_s(a);
//			g_marker_s(b);
//			g_marker_s(c);
		}

		g_finish();
		g_sleep(0.1);
	}
//	const int Size = 128;
//	const int Width = 7*Size, Height = 7*Size;
//	const double radius = 1./16;
//
//	g_enable_highdpi();
//	g_set_antialiasing(2);
//	g_init("GLSC3D", Width, Height);
//
//	g_line_width(2);
//
//	g_def_scale_2D(0, -1.5, 1.5, -1.5, 1.5, 0, 0, Width, Height);
//
//	g_marker_color(1, 0, 0, 1);
//	g_marker_radius(radius);
//	for (int i_time = 0;;i_time ++)
//	{
//		g_cls();
//
//		g_sel_scale(0);
//		g_line_color(0, 0, 0, 1);
//		g_line_width(10);
//		int Imax = 1 << 12;
//		srand(0);
//		for(int i = 0;i < Imax;i ++)
//		{
//			G_COLOR color;
//
//			color.r = rand() / (RAND_MAX + 1.0);
//			color.g = rand() / (RAND_MAX + 1.0);
//			color.b = rand() / (RAND_MAX + 1.0);
//			color.a = 1;
//			g_area_color_s(color);
//			g_line_color_s(color);
//
//			double dr = 1.0 / Imax;
//			double r = 1 - dr * i;
//			double theta = 5 * 2 * PI * i / Imax;
//			double x = r * cos(theta);
//			double y = r * sin(theta);
//			g_circle_2D(x, y, 50 * dr, 0, 1);
//		}
//
//		g_finish();
//
//	}

//	for (;;) {
//		g_cls();
//		g_sel_scale(0);
//
//		int n = 0;
//		for (double s = 0; s < 1; s += 1./128) {
//			double x = 2*M_PI*s - M_PI, y = sin(2*M_PI*s);
//
//			switch (n % 4) {
//			case 0:
//				g_marker_type((n / 16) % 2);
//				g_marker_2D(x, y);
//				break;
//			case 1:
//				g_line_color(0, 0, 1, 1);
//				g_circle_2D(x, y, radius, G_YES, G_NO);
//				break;
//			case 2:
//				g_area_color(0, 1, 0, 1);
//				g_circle_2D(x, y, radius, G_NO, G_YES);
//				break;
//			case 3:
//				g_area_color(1, 1, 0, 1);
//				g_line_color(0, 0, 0, 1);
//				g_circle_2D(x, y, radius, G_YES, G_YES);
//				break;
//			}
//
//			n++;
//		}
//
//		g_finish();
//	}
	return 0;
}
