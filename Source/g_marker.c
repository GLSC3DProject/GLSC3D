#include "glsc3d_private.h"

struct
{
	double r,g,b,a,size;
	int type;
}           glsc3D_g_def_marker[TotalDisplayNumber];

void g_marker_size(double size)
{
    glPointSize(size);
}

void g_marker_type(int type)
{
    glEnd();
    if(type == 0)
    {
        glEnable(GL_POINT_SMOOTH);
    }
    if(type == 1)
    {
        glDisable(GL_POINT_SMOOTH);
    }
}

void g_marker_3D(double x,double y,double z)
{
	glEnd();
	glDisable(GL_LIGHTING);
	g_points();
	glColor4fv(&g_current_marker_color.r);
	glVertex3d(x, y, z);
	glEnd();
}
void g_marker_2D(double x,double y)
{
	glDisable(GL_LIGHTING);
	g_points();
	glColor4fv(&g_current_marker_color.r);
	glVertex2d(x, y);
	glEnd();
}

void g_def_marker(int id, double r, double g, double b, double a, int type, double size)
{
	glsc3D_g_def_marker[id].r = r;
	glsc3D_g_def_marker[id].g = g;
	glsc3D_g_def_marker[id].b = b;
	glsc3D_g_def_marker[id].a = a;
	glsc3D_g_def_marker[id].type = type;
	glsc3D_g_def_marker[id].size = size;
}

void g_sel_marker(int id)
{
	double r, g, b, a, size;
	int type;
	r = glsc3D_g_def_marker[id].r;
	g = glsc3D_g_def_marker[id].g;
	b = glsc3D_g_def_marker[id].b;
	a = glsc3D_g_def_marker[id].a;
	type = glsc3D_g_def_marker[id].type;
	size = glsc3D_g_def_marker[id].size;

	g_marker_color(r, g, b, a);
	g_marker_type(type);
	g_marker_size(size);
}
