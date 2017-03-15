#include "glsc3d_private.h"

struct G_MARKER_APPEARANCE
{
	G_COLOR color;
	float size;
	int type;
};

G_MARKER_APPEARANCE glsc3D_g_def_marker[TotalDisplayNumber];

G_COLOR	g_current_marker_color(1, 1, 1, 1);
float	g_current_marker_size;
int		g_current_marker_type;

void g_marker_color_s(G_COLOR color)
{
	g_current_marker_color = color;
}

void g_marker_color(double r,double g,double b,double a)
{
	g_current_marker_color = G_COLOR(r, g, b, a);
}

void g_marker_size(double size)
{
#ifdef G_USE_CORE_PROFILE
	g_current_marker_size = (float)size * g_screen_scale_factor;
#else
    glPointSize(size);
#endif
}

void g_marker_type(int type)
{
#ifdef G_USE_CORE_PROFILE
	if (type < 0 || type >= G_NUM_MARKER_TYPES) {
		printf("Invalid marker type.\n");
	}

	g_current_marker_type = type;
#else
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

void g_marker_s(G_VECTOR position)
{
	g_begin_points();
	g_emit_vertex(position);
}

void g_marker_3D(double x, double y, double z)
{
	g_marker_s(G_VECTOR(x, y, z));
}

void g_marker_2D(double x,double y)
{
	g_marker_s(G_VECTOR(x, y));
}

void g_def_marker(int id, double r, double g, double b, double a, int type, double size)
{
	glsc3D_g_def_marker[id].color = G_COLOR(r, g, b, a);
	glsc3D_g_def_marker[id].type = type;
	glsc3D_g_def_marker[id].size = size;
}

void g_sel_marker(int id)
{
	G_MARKER_APPEARANCE& marker = glsc3D_g_def_marker[id];

	g_current_marker_color = marker.color;
	g_marker_type(marker.type);
	g_marker_size(marker.size);
}
