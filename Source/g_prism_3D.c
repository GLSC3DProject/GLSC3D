#include "glsc3d_private.h"
void g_prism_3D_core(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z,    //向き
		double radius,double height, double psi,
		int N, int DivideLevel, G_WIREFILL WireFill)
{
	int i;
	G_VECTOR top,center,r,n_x,r0,r1,r2,r3;
	double dth;
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));

	dth = 2*PI/N;
	center = g_vector3(center_x, center_y, center_z);
	top = g_vector3(height, 0, 0);
	r = g_vector3(0,radius,0);
	n_x = g_vector3(1,0,0);


	if(WireFill == 1)
	{	
		for(i=0;i<N;i++)
		{
			r0 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),0*dth),phi),theta),center);
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),i*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),(i+1)*dth),phi),theta),center);
			g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r2.x, r2.y, r2.z,
					DivideLevel,WireFill);

			r0 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),0*dth),phi),theta),center);
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),i*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),(i+1)*dth),phi),theta),center);
			g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r2.x, r2.y, r2.z,
					DivideLevel, WireFill);

			r0 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),i*dth),phi),theta),center);
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),(i+1)*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),(i+1)*dth),phi),theta),center);
			r3 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),i*dth),phi),theta),center);
			g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r3.x, r3.y, r3.z,
					DivideLevel,WireFill);
			g_triangle_3D_core(r1.x, r1.y, r1.z,
					r2.x, r2.y, r2.z,
					r3.x, r3.y, r3.z,
					DivideLevel,WireFill);
		}
	}

	if(WireFill == 0)
	{	
		for(i=0;i<N;i++)
		{
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),i*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),(i+1)*dth),phi),theta),center);
			g_move_3D(r1.x,r1.y,r1.z);
			g_plot_3D(r2.x,r2.y,r2.z);

			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),i*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),(i+1)*dth),phi),theta),center);
			g_move_3D(r1.x,r1.y,r1.z);
			g_plot_3D(r2.x,r2.y,r2.z);

			r0 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),i*dth),phi),theta),center);
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),(i+1)*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),(i+1)*dth),phi),theta),center);
			r3 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),i*dth),phi),theta),center);
			g_move_3D(r0.x,r0.y,r0.z);
			g_plot_3D(r1.x,r1.y,r1.z);
			g_move_3D(r2.x,r2.y,r2.z);
			g_plot_3D(r3.x,r3.y,r3.z);
		}
	}
}

void g_prism_3D(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z,    //向き
		double radius,double height, double psi, int N)                //半径、高さ、側面数
{
	g_prism_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, height, psi, N, 0, 1);
}
