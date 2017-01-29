#include "glsc3d_private.h"
void g_move_3D(double x,double y,double z)
{
	glEnd();
	glDisable(GL_LIGHTING);
	g_line_strip();
	glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
	glVertex3d(x,y,z);
}
void g_move_2D(double x,double y)
{
	glEnd();
	glDisable(GL_LIGHTING);
	g_line_strip();
	glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
	glVertex2d(x,y);
}
void g_moves(G_VECTOR u)
{
	glEnd();
	glDisable(GL_LIGHTING);
	g_line_strip();
	glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
	glVertex4d(u.x, u.y, u.z, u.w);
}
