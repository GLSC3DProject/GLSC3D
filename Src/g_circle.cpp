#include "glsc3d_3_private.h"

void g_circle_2D(
	double center_x, double center_y,
	double radius, G_BOOL Wire, G_BOOL Fill)
{
	int i,STEP=64;
	double dtheta=2*PI/STEP;
	G_VECTOR p0(center_x + radius, center_y); //i = 0

	if (Fill)
	{
		g_begin_triangles();
		G_VECTOR p1(center_x + radius*cos(dtheta), center_y + radius*sin(dtheta)); // i = 1
		for(i=2; i<STEP; i++)
		{
			G_VECTOR p2(center_x + radius*cos(i*dtheta), center_y + radius*sin(i*dtheta));
			g_emit_triangle(p0, p1, p2);
			p1 = p2;
		}
	}
	if (Wire)
	{
		g_move_s(p0);
		for(i=1; i<STEP; i++)
		{
			G_VECTOR pos(center_x + radius*cos(i*dtheta), center_y + radius*sin(i*dtheta));
			g_plot_s(pos);
		}
		g_plot_s(p0);
	}
}

void g_circle_3D_core(
	double center_x, double center_y, double center_z,
	double radius,
	double theta, double phi,
	int N, int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	int i,STEP=N;
	double dtheta=2*PI/STEP;
	G_VECTOR r(radius,0,0);
	G_VECTOR center(center_x, center_y, center_z);
	G_VECTOR r1, r2;

	if (Fill)
	{
		for(i=0; i<STEP; i++)
		{
			r1 = Rz(Rx(Ry(r,i*dtheta),theta),phi) + center;
			r2 = Rz(Rx(Ry(r,(i+1)*dtheta),theta),phi) + center;
			g_triangle_3D_flat_worker(center, r1, r2, DivideLevel);
		}
	}
	if (Wire)
	{
		G_VECTOR p0 = Rz(Rx(r, theta), phi);
		g_move_s(p0);
		for(i=1; i<STEP; i++)
		{
			g_plot_s(Rz(Rx(Ry(r, i * dtheta), theta), phi) + center);
		}
		g_plot_s(p0);
	}
}

void g_circle_3D(
	double center_x, double center_y, double center_z,
	double radius,
	double theta, double phi,
	G_BOOL Wire, G_BOOL Fill)
{
	g_circle_3D_core(center_x, center_y, center_z, radius, theta, phi, 64, 0, Wire, Fill);
}
