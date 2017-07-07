#include "glsc3d_3_private.h"

void g_box_2D(
	double x_left, double x_right,
	double y_bottom, double y_top,
	G_BOOL Wire, G_BOOL Fill)
{
	G_VECTOR r0(x_left, y_bottom);
	G_VECTOR r1(x_right, y_bottom);
	G_VECTOR r2(x_right, y_top);
	G_VECTOR r3(x_left, y_top);

	if (Fill)
	{
		g_begin_triangles();
		g_emit_triangle(r0, r1, r2);
		g_emit_triangle(r0, r2, r3);
	}
	if (Wire)
	{
		g_begin_lines();
		g_emit_line(r0, r1);
		g_emit_line(r1, r2);
		g_emit_line(r2, r3);
		g_emit_line(r3, r0);
	}
}

void g_box_3D_core(
	double x0, double x1,
	double y0, double y1,
	double z0, double z1,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	G_VECTOR r0(x0, y0, z0);
	G_VECTOR r1(x1, y0, z0);
	G_VECTOR r2(x0, y1, z0);
	G_VECTOR r3(x1, y1, z0);
	G_VECTOR r4(x0, y0, z1);
	G_VECTOR r5(x1, y0, z1);
	G_VECTOR r6(x0, y1, z1);
	G_VECTOR r7(x1, y1, z1);

	if (Fill)
	{
		g_triangle_3D_flat_worker(r0, r1, r4, DivideLevel);
		g_triangle_3D_flat_worker(r1, r5, r4, DivideLevel);

		g_triangle_3D_flat_worker(r6, r2, r4, DivideLevel);
		g_triangle_3D_flat_worker(r4, r2, r0, DivideLevel);

		g_triangle_3D_flat_worker(r2, r7, r3, DivideLevel);
		g_triangle_3D_flat_worker(r6, r7, r2, DivideLevel);

		g_triangle_3D_flat_worker(r1, r7, r5, DivideLevel);
		g_triangle_3D_flat_worker(r1, r3, r7, DivideLevel);

		g_triangle_3D_flat_worker(r0, r2, r1, DivideLevel);
		g_triangle_3D_flat_worker(r2, r3, r1, DivideLevel);

		g_triangle_3D_flat_worker(r6, r4, r7, DivideLevel);
		g_triangle_3D_flat_worker(r5, r7, r4, DivideLevel);
	}
	if (Wire)
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

		g_move_s(r0);
		g_plot_s(r4);

		g_move_s(r1);
		g_plot_s(r5);

		g_move_s(r2);
		g_plot_s(r6);

		g_move_s(r3);
		g_plot_s(r7);
	}
}

void g_box_3D(
	double x0, double x1,
	double y0, double y1,
	double z0, double z1,
	G_BOOL Wire, G_BOOL Fill)
{
	g_box_3D_core(x0, x1, y0, y1, z0, z1, 0, Wire, Fill);
}

void g_box_center_2D(
	double x, double y,
	double width, double height,
	G_BOOL WIRE, G_BOOL FILL)
{
	double dx = width / 2;
	double dy = height / 2;

	g_box_2D(x - dx, x + dx, y - dy, y + dy, WIRE, FILL);
}

void g_box_center_3D_core(
	double x, double y, double z,
	double width, double height, double depth,
	int DivideLevel, G_BOOL WIRE, G_BOOL FILL)
{
	double dx = width / 2;
	double dy = height / 2;
	double dz = depth / 2;

	g_box_3D_core(
		x - dx, x + dx, y - dy, y + dy, z - dz, z + dz,
		DivideLevel, WIRE, FILL);
}

void g_box_center_3D(
	double x, double y, double z,
	double width, double height, double depth,
	G_BOOL WIRE, G_BOOL FILL)
{
	g_box_center_3D_core(x, y, z, width, height, depth, 0, WIRE, FILL);
}
