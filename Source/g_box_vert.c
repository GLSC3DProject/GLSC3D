#include "glsc3d_private.h"
void g_box_2D_vert(double left, double right, double bottom, double top, G_WIREFILL WireFill)
{
	G_VECTOR_F r0 = g_vector2f(left, bottom);
	G_VECTOR_F r1 = g_vector2f(right, bottom);
	G_VECTOR_F r2 = g_vector2f(right, top);
	G_VECTOR_F r3 = g_vector2f(left, top);
	
	if (WireFill == G_FILL)
	{
		g_begin_triangles();
		
		g_vertex_buffer_append(r0);
		g_vertex_buffer_append(r1);
		g_vertex_buffer_append(r2);
		
		g_vertex_buffer_append(r0);
		g_vertex_buffer_append(r2);
		g_vertex_buffer_append(r3);
	}
	else
	{
		g_begin_lines();
		
		g_vertex_buffer_append(r0);
		g_vertex_buffer_append(r1);
		
		g_vertex_buffer_append(r1);
		g_vertex_buffer_append(r2);
		
		g_vertex_buffer_append(r2);
		g_vertex_buffer_append(r3);
		
		g_vertex_buffer_append(r3);
		g_vertex_buffer_append(r0);
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
	
	
	if(WireFill == 1)
	{
		g_triangle_3D_core(r0.x, r0.y, r0.z,
						   r1.x, r1.y, r1.z,
						   r4.x, r4.y, r4.z,
						   DivideLevel, WireFill);
		g_triangle_3D_core(r1.x, r1.y, r1.z,
						   r5.x, r5.y, r5.z,
						   r4.x, r4.y, r4.z,
						   DivideLevel, WireFill);
		
		g_triangle_3D_core(r6.x, r6.y, r6.z,
						   r2.x, r2.y, r2.z,
						   r4.x, r4.y, r4.z,
						   DivideLevel, WireFill);
		g_triangle_3D_core(r4.x, r4.y, r4.z,
						   r2.x, r2.y, r2.z,
						   r0.x, r0.y, r0.z,
						   DivideLevel, WireFill);
		
		g_triangle_3D_core(r2.x, r2.y, r2.z,
						   r7.x, r7.y, r7.z,
						   r3.x, r3.y, r3.z,
						   DivideLevel, WireFill);
		g_triangle_3D_core(r6.x, r6.y, r6.z,
						   r7.x, r7.y, r7.z,
						   r2.x, r2.y, r2.z,
						   DivideLevel, WireFill);
		
		g_triangle_3D_core(r1.x, r1.y, r1.z,
						   r7.x, r7.y, r7.z,
						   r5.x, r5.y, r5.z,
						   DivideLevel, WireFill);
		g_triangle_3D_core(r1.x, r1.y, r1.z,
						   r3.x, r3.y, r3.z,
						   r7.x, r7.y, r7.z,
						   DivideLevel, WireFill);
		
		g_triangle_3D_core(r0.x, r0.y, r0.z,
						   r2.x, r2.y, r2.z,
						   r1.x, r1.y, r1.z,
						   DivideLevel, WireFill);
		g_triangle_3D_core(r2.x, r2.y, r2.z,
						   r3.x, r3.y, r3.z,
						   r1.x, r1.y, r1.z,
						   DivideLevel, WireFill);
		
		g_triangle_3D_core(r6.x, r6.y, r6.z,
						   r4.x, r4.y, r4.z,
						   r7.x, r7.y, r7.z,
						   DivideLevel, WireFill);
		g_triangle_3D_core(r5.x, r5.y, r5.z,
						   r7.x, r7.y, r7.z,
						   r4.x, r4.y, r4.z,
						   DivideLevel, WireFill);
	}
	if(WireFill == 0)
	{
		g_move_3D(r0.x,r0.y,r0.z);
		g_plot_3D(r1.x,r1.y,r1.z);
		g_plot_3D(r3.x,r3.y,r3.z);
		g_plot_3D(r2.x,r2.y,r2.z);
		g_plot_3D(r0.x,r0.y,r0.z);
		
		g_move_3D(r4.x,r4.y,r4.z);
		g_plot_3D(r5.x,r5.y,r5.z);
		g_plot_3D(r7.x,r7.y,r7.z);
		g_plot_3D(r6.x,r6.y,r6.z);
		g_plot_3D(r4.x,r4.y,r4.z);
		
		g_move_3D(r2.x,r2.y,r2.z);
		g_plot_3D(r6.x,r6.y,r6.z);
		
		g_move_3D(r3.x,r3.y,r3.z);
		g_plot_3D(r7.x,r7.y,r7.z);
		
		g_move_3D(r5.x,r5.y,r5.z);
		g_plot_3D(r1.x,r1.y,r1.z);
		
		g_move_3D(r0.x,r0.y,r0.z);
		g_plot_3D(r4.x,r4.y,r4.z);
	}
}
void g_box_3D_vert(double x_min, double x_max,
				   double y_min, double y_max,
				   double z_min, double z_max)
{
	g_box_3D_vert_core(x_min, x_max, y_min, y_max, z_min, z_max, 0, G_FILL);
}
