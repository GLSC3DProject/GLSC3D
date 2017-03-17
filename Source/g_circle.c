#include "glsc3d_private.h"
void g_circle_2D(double center_x, double center_y,
		double radius, G_WIREFILL WireFill)
{
	int i,STEP=100;
	double dtheta=2*PI/STEP;

	if(WireFill == 1)
	{
		g_begin_triangle_fan();
		for(i=0; i<=STEP; i++)
		{
			G_VECTOR pos = g_vector2(center_x + radius*cos(i*dtheta),center_y + radius*sin(i*dtheta));
			g_emit_vertex(pos);
		}
	}
	if(WireFill == 0)
	{
		g_begin_line_strip();
		for(i=0; i<=STEP; i++)
		{
			G_VECTOR pos = g_vector2(center_x + radius*cos(i*dtheta),center_y + radius*sin(i*dtheta));
			g_emit_vertex(pos);
		}
	}
}

void g_circle_3D_core(double center_x, double center_y, double center_z,
		double radius,
		double theta, double phi,
		int N, int DivideLevel, G_WIREFILL WireFill)
{
	int i,STEP=N;
	double dtheta=2*PI/STEP;
	G_VECTOR r = g_vector3(radius,0,0);
	G_VECTOR center = g_vector3(center_x,center_y,center_z), r0, r1, r2;

	r0 = g_plus(center,Rz(Rx(r,theta),phi));
	if(WireFill==1)
	{
		for(i=0; i<STEP; i++)
		{
			r1 = g_plus(center,Rz(Rx(Ry(r,i*dtheta),theta),phi));
			r2 = g_plus(center,Rz(Rx(Ry(r,(i+1)*dtheta),theta),phi));
			g_triangle_3D_core(center.x,center.y,center.z,
					r1.x,r1.y,r1.z,
					r2.x,r2.y,r2.z,
					DivideLevel, WireFill);
		}
	}
	if(WireFill==0)
	{
		g_begin_line_strip();
		for(i=0; i<=STEP; i++)
		{
			g_emit_vertex(g_plus(center, Rz(Rx(Ry(r, i * dtheta), theta), phi)));
		}
	}
}

void g_circle_3D(double center_x, double center_y, double center_z,
		double radius,
		double theta, double phi,
		G_WIREFILL WireFill)
{
	g_circle_3D_core(center_x, center_y, center_z, radius, theta, phi, 100, 0, WireFill);
}

