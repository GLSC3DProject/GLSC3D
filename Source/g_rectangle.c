#include "glsc3d_private.h"
#include <stdio.h>
void g_rectangle_3D_core(double x, double y, double z,
		double direction_x, double direction_y, double direction_z,
		double width, double depth,
		double psi,
		int DivideLevel, G_WIREFILL WireFill)
{
	G_VECTOR r[4], center, r0, r1, r2, r3;
	center = g_vector3(x,y,z);
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));

	r[0] = g_vector3(-width/2, 0, -depth/2);
	r[1] = g_vector3(width/2, 0, -depth/2);
	r[2] = g_vector3(width/2, 0, depth/2);
	r[3] = g_vector3(-width/2, 0, depth/2);

	r0 = g_plus(center,Ry(Rz(Rx(r[0],psi),phi),theta));
	r1 = g_plus(center,Ry(Rz(Rx(r[1],psi),phi),theta));
	r2 = g_plus(center,Ry(Rz(Rx(r[2],psi),phi),theta));
	r3 = g_plus(center,Ry(Rz(Rx(r[3],psi),phi),theta));

	if(WireFill == G_FILL)
	{
		g_triangle_3D_core_worker(r0, r1, r2, DivideLevel);
		g_triangle_3D_core_worker(r0, r2, r3, DivideLevel);
	}
	else
	{
		g_begin_line_loop();
		for(int i=0;i<4;i++)
		{
			g_emit_vertex(g_plus(center,Ry(Rz(Rx(r[i],psi),phi),theta)));
		}
	}
}

void g_rectangle_3D(double x, double y, double z,
		double direction_x, double direction_y, double direction_z,
		double width, double depth,
		double psi,
		G_WIREFILL WireFill)
{
	g_rectangle_3D_core(x, y, z, direction_x, direction_y, direction_z, width, depth, psi, 0, WireFill);
}
