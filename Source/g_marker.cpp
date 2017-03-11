#include "glsc3d_private.h"

struct G_MARKER
{
	G_COLOR color;
	float size;
	int type;
};

G_MARKER glsc3D_g_def_marker[TotalDisplayNumber];

void g_marker_size(double size)
{
    glPointSize(size);
}

void g_marker_type(int type)
{
#ifndef G_USE_CORE_PROFILE
	if(type == 0)
    {
        glEnable(GL_POINT_SMOOTH);
    }
    if(type == 1)
    {
        glDisable(GL_POINT_SMOOTH);
    }
#endif
}

void g_marker_3D(double x,double y,double z)
{
	g_begin_points();
	g_current_color = g_current_marker_color;
	g_emit_vertex(G_VECTOR(x, y, z));
}

void g_marker_2D(double x,double y)
{
	g_begin_points();
	g_current_color = g_current_marker_color;
	g_emit_vertex(G_VECTOR(x, y));
}

void g_def_marker(int id, double r, double g, double b, double a, int type, double size)
{
	glsc3D_g_def_marker[id].color = G_COLOR(r, g, b, a);
	glsc3D_g_def_marker[id].type = type;
	glsc3D_g_def_marker[id].size = size;
}

void g_sel_marker(int id)
{
	G_MARKER& marker = glsc3D_g_def_marker[id];

	g_current_marker_color = marker.color;
	g_marker_type(marker.type);
	g_marker_size(marker.size);
}
