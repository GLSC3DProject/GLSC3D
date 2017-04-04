#include "glsc3d_private.h"

void g_circle_2D(
	double center_x, double center_y,
	double radius, G_WIREFILL WireFill)
{
	int i,STEP=64;
	double dtheta=2*PI/STEP;

	if(WireFill == 1)
	{
		g_begin_triangle_fan();
		for(i=0; i<=STEP; i++)
		{
			G_VECTOR pos(center_x + radius*cos(i*dtheta), center_y + radius*sin(i*dtheta));
			g_emit_vertex(pos);
		}
	}
	if(WireFill == 0)
	{
		g_begin_line_strip();
		for(i=0; i<=STEP; i++)
		{
			G_VECTOR pos(center_x + radius*cos(i*dtheta), center_y + radius*sin(i*dtheta));
			g_emit_vertex(pos);
		}
	}
}

void g_circle_3D_core(
	double center_x, double center_y, double center_z,
	double radius,
	double theta, double phi,
	int N, int DivideLevel, G_WIREFILL WireFill)
{
	int i,STEP=N;
	double dtheta=2*PI/STEP;
	G_VECTOR r(radius,0,0);
	G_VECTOR center(center_x, center_y, center_z);
	G_VECTOR r0, r1, r2;

	//r0 = Rz(Rx(r,theta),phi) + center;
	if (WireFill==1)
	{
		for(i=0; i<STEP; i++)
		{
			r1 = Rz(Rx(Ry(r,i*dtheta),theta),phi) + center;
			r2 = Rz(Rx(Ry(r,(i+1)*dtheta),theta),phi) + center;
			g_triangle_3D_flat_worker(center, r1, r2, DivideLevel);
		}
	}
	if (WireFill==0)
	{
		g_begin_line_strip();
		for(i=0; i<=STEP; i++)
		{
			g_emit_vertex(Rz(Rx(Ry(r, i * dtheta), theta), phi) + center);
		}
	}
}

void g_circle_3D(
	double center_x, double center_y, double center_z,
	double radius,
	double theta, double phi,
	G_WIREFILL WireFill)
{
	g_circle_3D_core(center_x, center_y, center_z, radius, theta, phi, 64, 0, WireFill);
}
