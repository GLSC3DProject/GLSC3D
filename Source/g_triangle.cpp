#include "glsc3d_private.h"

// ---- internally called functions ----

void g_triangle_3D_smooth_worker(G_VECTOR r0, G_VECTOR r1, G_VECTOR r2, G_VECTOR n0, G_VECTOR n1, G_VECTOR n2, int DivideLevel)
{
	if (DivideLevel >= 1) {
		G_VECTOR r3 = (r0 + r1) / 2;
		G_VECTOR r4 = (r1 + r2) / 2;
		G_VECTOR r5 = (r2 + r0) / 2;

		G_VECTOR n3 = (n0 + n1) / 2;
		G_VECTOR n4 = (n1 + n2) / 2;
		G_VECTOR n5 = (n2 + n0) / 2;

		g_triangle_3D_smooth_worker(r0, r3, r5, n0, n3, n5, DivideLevel - 1);
		g_triangle_3D_smooth_worker(r1, r4, r3, n1, n4, n3, DivideLevel - 1);
		g_triangle_3D_smooth_worker(r2, r5, r4, n2, n5, n4, DivideLevel - 1);
		g_triangle_3D_smooth_worker(r3, r4, r5, n3, n4, n5, DivideLevel - 1);
	} else {
		G_VERTEX v0 = g_make_vertex(r0, n0);
		G_VERTEX v1 = g_make_vertex(r1, n1);
		G_VERTEX v2 = g_make_vertex(r2, n2);

		G_TRIANGLE t0 = g_make_triangle_core(v0, v1, v2);
		g_set_triangle(&t0);
	}
}

void g_triangle_3D_flat_worker(G_VECTOR r0, G_VECTOR r1, G_VECTOR r2, int DivideLevel)
{
	G_VECTOR n = g_calc_normal(r0, r1, r2);
	g_triangle_3D_smooth_worker(r0, r1, r2, n, n, n, DivideLevel);
}

void g_triangle_3D_wire_worker(G_VECTOR r0, G_VECTOR r1, G_VECTOR r2)
{
	g_begin_lines();

	g_emit_line(r0, r1);
	g_emit_line(r1, r2);
	g_emit_line(r2, r0);
}

// ---- struct argument functions ----

void g_triangle_3D_smooth_core_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	G_VECTOR n0, G_VECTOR n1, G_VECTOR n2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	if (Fill) {
		g_triangle_3D_smooth_worker(r0, r1, r2, n0, n1, n2, DivideLevel);
	}
	if (Wire) {
		g_triangle_3D_wire_worker(r0, r1, r2);
	}
}

void g_triangle_3D_smooth_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	G_VECTOR n0, G_VECTOR n1, G_VECTOR n2,
	G_BOOL Wire, G_BOOL Fill)
{
	g_triangle_3D_smooth_core_s(r0, r1, r2, n0, n1, n2, 0, Wire, Fill);
}

void g_triangle_3D_core_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	if (Fill) {
		G_VECTOR n = g_calc_normal(r0, r1, r2);
		g_triangle_3D_smooth_worker(r0, r1, r2, n, n, n, DivideLevel);
	}
	if (Wire) {
		g_triangle_3D_wire_worker(r0, r1, r2);
	}
}

void g_triangle_3D_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	G_BOOL Wire, G_BOOL Fill)
{
	g_triangle_3D_core_s(r0, r1, r2, 0, Wire, Fill);
}

// ---- non-struct argument functions ----

void g_triangle_3D_smooth_core(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	double nx0, double ny0, double nz0,
	double nx1, double ny1, double nz1,
	double nx2, double ny2, double nz2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	G_VECTOR r0(x0, y0, z0);
	G_VECTOR r1(x1, y1, z1);
	G_VECTOR r2(x2, y2, z2);

	G_VECTOR n0(nx0, ny0, nz0);
	G_VECTOR n1(nx1, ny1, nz1);
	G_VECTOR n2(nx2, ny2, nz2);

	g_triangle_3D_smooth_core_s(r0, r1, r2, n0, n1, n2, DivideLevel, Wire, Fill);
}

void g_triangle_3D_smooth(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	double nx0, double ny0, double nz0,
	double nx1, double ny1, double nz1,
	double nx2, double ny2, double nz2,
	G_BOOL Wire, G_BOOL Fill)
{
	g_triangle_3D_smooth_core(
		x0, y0, z0, x1, y1, z1, x2, y2, z2,
		nx0, ny0, nz0, nx1, ny1, nz1, nx2, ny2, nz2,
		0, Wire, Fill);
}

void g_triangle_3D_core(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	G_VECTOR r0(x0, y0, z0);
	G_VECTOR r1(x1, y1, z1);
	G_VECTOR r2(x2, y2, z2);

	g_triangle_3D_core_s(r0, r1, r2, DivideLevel, Wire, Fill);
}

void g_triangle_3D(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	G_BOOL Wire, G_BOOL Fill)
{
	g_triangle_3D_core(x0, y0, z0, x1, y1, z1, x2, y2, z2, 0, Wire, Fill);
}

// ---- 2D triangle functions ----

void g_triangle_2D_s(G_VECTOR r0, G_VECTOR r1, G_VECTOR r2, G_BOOL Wire, G_BOOL Fill)
{
	if (Wire) {
		g_begin_lines();
		g_emit_line(r0, r1);
		g_emit_line(r1, r2);
		g_emit_line(r2, r0);
	}
	if (Fill) {
		g_begin_triangles();
		g_emit_vertex(r0);
		g_emit_vertex(r1);
		g_emit_vertex(r2);
	}
}

void g_triangle_2D(
	double x0, double y0,
	double x1, double y1,
	double x2, double y2,
	G_BOOL Wire, G_BOOL Fill)
{
	G_VECTOR r0(x0, y0);
	G_VECTOR r1(x1, y1);
	G_VECTOR r2(x2, y2);

	g_triangle_2D_s(r0, r1, r2, Wire, Fill);
}

// ---- internally called functions ----

void g_set_triangle(const G_TRIANGLE *t)
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

void g_triangle_terminal(const G_TRIANGLE *t)
{
	g_begin_triangles();
	
	g_vertex_buffer_append(t->vertex[0]);
	g_vertex_buffer_append(t->vertex[1]);
	g_vertex_buffer_append(t->vertex[2]);
}
