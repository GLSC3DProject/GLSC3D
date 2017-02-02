#include "glsc3d_private.h"
void g_box_2D_vert(double left, double right, double bottom, double top, G_WIREFILL WireFill)
{
	G_VECTOR r0 = g_vector2(left, bottom);
	G_VECTOR r1 = g_vector2(right, bottom);
	G_VECTOR r2 = g_vector2(right, top);
	G_VECTOR r3 = g_vector2(left, top);
	
	if (WireFill == G_FILL)
	{
		g_begin_triangles();
		
		g_vertex_buffer_append_triangle_2D(r0, r1, r2);
		g_vertex_buffer_append_triangle_2D(r0, r2, r3);
	}
	else
	{
		g_begin_lines();
		
		g_vertex_buffer_append_line(r0, r1);
		g_vertex_buffer_append_line(r1, r2);
		g_vertex_buffer_append_line(r2, r3);
		g_vertex_buffer_append_line(r3, r0);
	}
}

void g_box_3D_vert_core(double x_min, double x_max,
						double y_min, double y_max,
						double z_min, double z_max,
						int DivideLevel, G_WIREFILL WireFill)
{
	G_VECTOR r0, r1, r2, r3, r4, r5, r6, r7;
	
	r0 = g_vector3(x_min, y_min, z_min);
	r1 = g_vector3(x_max, y_min, z_min);
	r2 = g_vector3(x_min, y_max, z_min);
	r3 = g_vector3(x_max, y_max, z_min);
	r4 = g_vector3(x_min, y_min, z_max);
	r5 = g_vector3(x_max, y_min, z_max);
	r6 = g_vector3(x_min, y_max, z_max);
	r7 = g_vector3(x_max, y_max, z_max);
	
	
	if (WireFill == G_FILL)
	{
		g_triangle_3D_core_worker(r0, r1, r4, DivideLevel);
		g_triangle_3D_core_worker(r1, r5, r4, DivideLevel);
		
		g_triangle_3D_core_worker(r6, r2, r4, DivideLevel);
		g_triangle_3D_core_worker(r4, r2, r0, DivideLevel);
		
		g_triangle_3D_core_worker(r2, r7, r3, DivideLevel);
		g_triangle_3D_core_worker(r6, r7, r2, DivideLevel);
		
		g_triangle_3D_core_worker(r1, r7, r5, DivideLevel);
		g_triangle_3D_core_worker(r1, r3, r7, DivideLevel);
		
		g_triangle_3D_core_worker(r0, r2, r1, DivideLevel);
		g_triangle_3D_core_worker(r2, r3, r1, DivideLevel);
		
		g_triangle_3D_core_worker(r6, r4, r7, DivideLevel);
		g_triangle_3D_core_worker(r5, r7, r4, DivideLevel);
	}
	else
	{
		g_move_s(r0);
		g_plot_s(r1);
		g_plot_s(r3);
		g_plot_s(r2);
		g_plot_s(r0);
		
		g_move_s(r4);
		g_plot_s(r5);
		g_plot_s(r7);
		g_plot_s(r6);
		g_plot_s(r4);
		
		g_move_s(r2);
		g_plot_s(r6);
		
		g_move_s(r3);
		g_plot_s(r7);
		
		g_move_s(r5);
		g_plot_s(r1);
		
		g_move_s(r0);
		g_plot_s(r4);
	}
}
void g_box_3D_vert(double x_min, double x_max,
				   double y_min, double y_max,
				   double z_min, double z_max)
{
	g_box_3D_vert_core(x_min, x_max, y_min, y_max, z_min, z_max, 0, G_FILL);
}
