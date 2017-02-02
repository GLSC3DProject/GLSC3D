#include "glsc3d_private.h"

G_VECTOR current_position;

void g_move_3D(double x,double y,double z)
{
	g_begin_lines();
	current_position = g_vector3(x, y, z);
}

void g_move_2D(double x,double y)
{
	g_move_3D(x, y, 0);
}

void g_move_s(G_VECTOR u)
{
	g_move_3D(u.x, u.y, u.z);
}

void g_plot_3D(double x,double y,double z)
{
	G_VECTOR position = g_vector3(x, y, z);
	
	g_vertex_buffer_append_line(current_position, position);
	
	current_position = position;
}

void g_plot_2D(double x,double y)
{
	g_plot_3D(x, y, 0);
}

void g_plot_s(G_VECTOR u)
{
	g_plot_3D(u.x, u.y, u.z);
}
