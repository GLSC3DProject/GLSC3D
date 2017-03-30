#include "glsc3d_private.h"

void g_cylinder_3D_core(
		double center_x, double center_y, double center_z,              //中心座標
		double direction_x, double direction_y, double direction_z,     //向き
		double radius, double height, double psi, int N,                //半径、高さ、側面数
		int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	G_VECTOR r0, r1, r2, r3;
	float dth = 2 * (float)PI / N;
	G_VECTOR direction(direction_x, direction_y, direction_z);
	double phi = g_direction_phi(direction);
	double theta = g_direction_theta(direction);

	G_VECTOR center(center_x, center_y, center_z);
	G_VECTOR top(0.5 * height, 0, 0);
	G_VECTOR rn(0, 0, 1);
	G_VECTOR r = radius * rn;

	G_MATRIX matrix = G_MATRIX::RotationX(psi) * G_MATRIX::RotationZ(phi) * G_MATRIX::RotationY(-theta) * G_MATRIX::Translation(center);

	if (Fill)
	{
		G_VECTOR p = -top * matrix;
		G_VECTOR q = top * matrix;

		for (int i = 0; i < N; i++)
		{
			G_MATRIX A = G_MATRIX::RotationX(i * dth);
			G_MATRIX B = G_MATRIX::RotationX((i + 1) * dth);
			r0 = (r - top) * A * matrix;
			r1 = (r + top) * A * matrix;
			r2 = (r - top) * B * matrix;
			r3 = (r + top) * B * matrix;

			G_TRIANGLE t0, t1;
			G_VECTOR n01, n23;
			G_VERTEX v0, v1, v2, v3;

			n01 = g_transform_normal(rn * A, matrix);
			n23 = g_transform_normal(rn * B, matrix);

			v0 = g_make_vertex(r0, n01);
			v1 = g_make_vertex(r1, n01);
			v2 = g_make_vertex(r2, n23);
			v3 = g_make_vertex(r3, n23);

			t0 = g_make_triangle_core(v0, v1, v2);
			t1 = g_make_triangle_core(v1, v3, v2);
			g_set_triangle(t0);
			g_set_triangle(t1);

			g_triangle_3D_core_worker(p, r0, r2, DivideLevel);
			g_triangle_3D_core_worker(q, r3, r1, DivideLevel);
		}
	}
	if (Wire)
	{
		g_begin_lines();

		for (int i = 0; i < N; i++)
		{
			G_MATRIX A = G_MATRIX::RotationX(i * dth);
			G_MATRIX B = G_MATRIX::RotationX((i + 1) * dth);
			r0 = (r - top) * A * matrix;
			r1 = (r + top) * A * matrix;
			r2 = (r - top) * B * matrix;
			r3 = (r + top) * B * matrix;

			g_emit_line(r0, r1);
			g_emit_line(r0, r2);
			g_emit_line(r1, r3);
		}
	}
}

void g_cylinder_3D(double center_x, double center_y, double center_z, double direction_x, double direction_y,
				   double direction_z, double radius, double height, double psi, G_BOOL Wire, G_BOOL Fill)                //半径、高さ、側面数
{
	g_cylinder_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, height, psi, 50, 0, Wire, Fill);
}
