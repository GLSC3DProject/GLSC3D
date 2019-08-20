#include "glsc3d_3_private.h"

void g_rectangle_3D_core(
	double x, double y, double z,
	double direction_x, double direction_y, double direction_z,
	double width, double depth,
	double psi, int DivideLevel,
	G_BOOL WIRE, G_BOOL FILL)
{
	G_VECTOR center(x,y,z);
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));

	G_VECTOR p0(-width/2, 0, -depth/2);
	G_VECTOR p1(width/2, 0, -depth/2);
	G_VECTOR p2(width/2, 0, depth/2);
	G_VECTOR p3(-width/2, 0, depth/2);

	G_VECTOR r0 = center + Ry(Rz(Rx(p0, psi), phi), theta);
	G_VECTOR r1 = center + Ry(Rz(Rx(p1, psi), phi), theta);
	G_VECTOR r2 = center + Ry(Rz(Rx(p2, psi), phi), theta);
	G_VECTOR r3 = center + Ry(Rz(Rx(p3, psi), phi), theta);

	if(FILL)
	{
		g_triangle_3D_flat_worker(r0, r1, r2, DivideLevel);
		g_triangle_3D_flat_worker(r0, r2, r3, DivideLevel);
	}
	if(WIRE)
	{
		g_move_s(r0);
		g_plot_s(r1);
		g_plot_s(r2);
		g_plot_s(r3);
		g_plot_s(r0);
	}
}

void g_rectangle_3D(
	double x, double y, double z,
	double direction_x, double direction_y, double direction_z,
	double width, double depth,
	double psi,
	G_BOOL WIRE, G_BOOL FILL)
{
	g_rectangle_3D_core(x, y, z, direction_x, direction_y, direction_z, width, depth, psi, 0, WIRE, FILL);
}
