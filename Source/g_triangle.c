#include "glsc3d_private.h"

void g_triangle_3D_core_worker(G_POSITION r0, G_POSITION r1, G_POSITION r2, int DivideLevel)
{
	G_POSITION r3, r4, r5;

	if(DivideLevel >= 1)
	{
		r3 = g_multi(0.5,g_plus(r0,r1));
		r4 = g_multi(0.5,g_plus(r1,r2));
		r5 = g_multi(0.5,g_plus(r2,r0));

		g_triangle_3D_core_worker(r0, r3, r5, DivideLevel-1);
		g_triangle_3D_core_worker(r1, r4, r3, DivideLevel-1);
		g_triangle_3D_core_worker(r2, r5, r4, DivideLevel-1);
		g_triangle_3D_core_worker(r3, r5, r4, DivideLevel-1);
	}else{
		G_TRIANGLE t0;
		G_VERTEX   v0, v1, v2;

		G_VECTOR n = g_calc_normal(r0, r1, r2);

		v0 = g_make_vertex(r0, n);
		v1 = g_make_vertex(r1, n);
		v2 = g_make_vertex(r2, n);
		t0 = g_make_triangle_core(v0, v1, v2);
		g_set_triangle(t0);
	}
}

void g_triangle_3D_core(double x0, double y0, double z0,
		double x1, double y1, double z1,
		double x2, double y2, double z2,
		int DivideLevel, G_WIREFILL WireFill)
{
	if(WireFill == G_FILL)
	{
		g_triangle_3D_core_worker(
				g_vector(x0, y0, z0),
				g_vector(x1, y1, z1),
				g_vector(x2, y2, z2),
				DivideLevel);
	}
	else
	{
		G_VECTOR r0, r1, r2;
		r0 = g_vector3(x0,y0,z0);
		r1 = g_vector3(x1,y1,z1);
		r2 = g_vector3(x2,y2,z2);

		g_begin_lines();
		
		g_emit_vertex(r0);
		g_emit_vertex(r1);

		g_emit_vertex(r1);
		g_emit_vertex(r2);

		g_emit_vertex(r2);
		g_emit_vertex(r0);
	}
}

void g_triangle_3D(double x0, double y0, double z0,
		double x1, double y1, double z1,
		double x2, double y2, double z2,
		G_WIREFILL WireFill)
{
	g_triangle_3D_core(x0, y0, z0,
			x1, y1, z1,
			x2, y2, z2,
			0, WireFill);
}

void g_set_triangle(G_TRIANGLE t)
{
	if(g_enable_transparent)
	{
		g_triangle_buffer_append(t);
	}
	else
	{
		g_triangle_terminal(t);
	}
}

void g_triangle_terminal(G_TRIANGLE t)
{
	g_begin_triangles();
	
	g_vertex_buffer_append(t.vertex[0]);
	g_vertex_buffer_append(t.vertex[1]);
	g_vertex_buffer_append(t.vertex[2]);
}

void g_triangle_2D(double x0, double y0,
				   double x1, double y1,
				   double x2, double y2,
				   G_WIREFILL WireFill)
{
	G_VECTOR r0,r1,r2;
	
	r0 = g_vector2(x0,y0);
	r1 = g_vector2(x1,y1);
	r2 = g_vector2(x2,y2);
	
	if(WireFill == G_FILL)
	{
		g_begin_triangles();

		g_emit_vertex(r0);
		g_emit_vertex(r1);
		g_emit_vertex(r2);
	}
	else
	{
		g_begin_lines();

		g_emit_vertex(r0);
		g_emit_vertex(r1);

		g_emit_vertex(r1);
		g_emit_vertex(r2);

		g_emit_vertex(r2);
		g_emit_vertex(r0);
	}
}
