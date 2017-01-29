#include "glsc3d_private.h"
void g_plot_3D(double x,double y,double z)
{
	glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
	glVertex3d(x,y,z);
}
void g_plot_2D(double x,double y)
{
	glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
	glVertex2d(x,y);
}
void g_plots(G_VECTOR u)
{
	glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
	glVertex4d(u.x, u.y, u.z, u.w);
}
