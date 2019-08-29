#include "glsc3d_3_private.h"

struct G_MARKER_APPEARANCE
{
	G_COLOR color;
	float size;
	G_UINT type;

	void select()
	{
		g_marker_color_s(color);
		g_marker_size(size);
		g_marker_type(type);
	}
};

G_MARKER_APPEARANCE glsc3D_g_def_marker[TotalDisplayNumber];

G_COLOR	g_current_marker_color;
float	g_current_marker_size;
G_UINT	g_current_marker_type;
G_UINT	g_current_marker_size_type;

void g_marker_color_s(G_COLOR color)
{
	g_current_marker_color = color;
}

void g_marker_color(float r, float g, float b, float a)
{
	g_current_marker_color = G_COLOR(r, g, b, a);
}

void g_marker_size(float size)
{
	if (g_current_marker_size_type != G_MARKER_SIZE_STANDARD) {
		g_vertex_buffer_flush();
		g_current_marker_size_type = G_MARKER_SIZE_STANDARD;
	}

	g_current_marker_size = size;
}

void g_marker_radius(float size)
{
	if (g_current_marker_size_type != G_MARKER_SIZE_VIRTUAL) {
		g_vertex_buffer_flush();
		g_current_marker_size_type = G_MARKER_SIZE_VIRTUAL;
	}

	g_current_marker_size = size;
}

void g_marker_type(G_UINT type)
{
	if (type >= G_NUM_MARKER_TYPES) {
		printf("Invalid marker type.\n");
		return;
	}

	if (g_current_marker_type != type) {
		g_vertex_buffer_flush();
		g_current_marker_type = type;
	}
}

void g_marker_s(G_VECTOR position)
{
	g_begin_points();
	g_emit_point(position);
}

void g_marker_3D(double x, double y, double z)
{
	g_marker_s(G_VECTOR(x, y, z));
}

void g_marker_2D(double x,double y)
{
	g_marker_s(G_VECTOR(x, y));
}

void g_def_marker(int id, float r, float g, float b, float a, G_UINT type, float size)
{
	glsc3D_g_def_marker[id].color = G_COLOR(r, g, b, a);
	glsc3D_g_def_marker[id].type = type;
	glsc3D_g_def_marker[id].size = size;
}

void g_sel_marker(int id)
{
	glsc3D_g_def_marker[id].select();
}
