#include "glsc3d_3.h"

#define WIDTH 640
#define HEIGHT 320

#define x_division 64
#define y_division 32

static const double h = -0.5;

int main()
{
	g_enable_highdpi();
	g_set_antialiasing(2);
	g_init("GLSC3D", WIDTH, HEIGHT*2);
	
	g_init_light(0, 1, 0, 1);
	g_init_light(1,-1, 0, 1);
	g_scr_color(1, 1, 1);

	g_def_scale_3D_fix(0, -2, +2, -2, +2, -1, +1, 0, 8, 8, 0, 1, 0, 1, 0, 0, WIDTH, HEIGHT);
	g_def_scale_2D(1, -2, +2, -1, +1, 0, HEIGHT, WIDTH, HEIGHT);
	
	for (double t = 0;; t += 1./128) {
		double c = cos(t), s = sin(t), a = c*c - s*s, b = 4*c*s;
		double data[x_division][y_division];
		
		for (int i = 0; i < x_division; i++) {
			for (int j = 0; j < y_division; j++) {
				double x = (4./x_division) * i - 2;
				double y = (2./y_division) * j - 1;
				
				data[i][j] = a * (x*x - h*h) + b * (x*h) - y*y;
			}
		}
		
		g_cls();

		g_sel_scale(0);
		
		g_area_color_3D(0.5, 0.5, 0.75, 1);
		g_rectangle_3D(0, h, 0, 1, 0, 0, 5, 4, 0, G_NO, G_YES);

		g_area_color_3D(0.5, 0.75, 0.5, 1);
		g_cone_3D_core(-2*c, -2*s, 0, c, s, 0, 2, 2, 100, 0, G_NO, G_YES);
		g_cone_3D_core(2*c, 2*s, 0, -c, -s, 0, 2, 2, 100, 0, G_NO, G_YES);

		g_line_color(0, 0, 0, 1);
		g_sel_scale(1);
		g_line_color(0, 0, 0, 1);
		g_contln_f_2D(-2, 2, -1, 1, x_division, y_division, (double *)data, 0);
		
		g_finish();
	}
	
	return 0;
}
