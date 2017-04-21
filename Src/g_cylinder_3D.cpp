#include "glsc3d_3_private.h"

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

			G_VECTOR n01, n23;

			n01 = g_transform_normal(rn * A, matrix);
			n23 = g_transform_normal(rn * B, matrix);

			g_triangle_3D_flat_worker(p, r0, r2, DivideLevel);
			g_triangle_3D_flat_worker(q, r3, r1, DivideLevel);
			g_triangle_3D_smooth_worker(
				r0, r1, r2,
				n01, n01, n23, DivideLevel);
			g_triangle_3D_smooth_worker(
				r1, r3, r2,
				n01, n23, n23, DivideLevel);
		}
	}
	if (Wire)
	{
		for (int i = 0; i < N; i++)
		{
			G_MATRIX A = G_MATRIX::RotationX(i * dth);
			G_MATRIX B = G_MATRIX::RotationX((i + 1) * dth);
			r0 = (r - top) * A * matrix;
			r1 = (r + top) * A * matrix;
			r2 = (r - top) * B * matrix;
			r3 = (r + top) * B * matrix;

			g_move_s(r2);
			g_plot_s(r0);
			g_plot_s(r1);
			g_plot_s(r3);
		}
	}
}

void g_cylinder_3D(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius, double height, double psi,                //半径、高さ、側面数
	G_BOOL Wire, G_BOOL Fill)
{
	g_cylinder_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, height, psi, 50, 0, Wire, Fill);
}
