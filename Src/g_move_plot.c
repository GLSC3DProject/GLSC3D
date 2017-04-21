#include "glsc3d_3_private.h"

G_VECTOR current_position;

void g_move_s(G_VECTOR position)
{
	g_begin_lines();
	current_position = position;
}

void g_move_3D(double x, double y, double z)
{
	g_move_s(g_vector3(x, y, z));
}

void g_move_2D(double x, double y)
{
	g_move_3D(x, y, 0);
}

void g_plot_s(G_VECTOR position)
{
	g_emit_vertex(current_position);
	g_emit_vertex(position);

	current_position = position;
}

void g_plot_3D(double x, double y, double z)
{
	g_plot_s(g_vector3(x, y, z));
}

void g_plot_2D(double x, double y)
{
	g_plot_s(g_vector2(x, y));
}
