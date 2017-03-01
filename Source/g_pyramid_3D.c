#include "glsc3d_private.h"
void g_pyramid_3D_core(double center_x, double center_y, double center_z,                      //中心座標
                  double direction_x, double direction_y, double direction_z,             //方向
                  double radius,double head_size, double psi,                             //半径、高さ
                  int N, int DivideLevel, G_WIREFILL WireFill)
{
    
    int i;
    G_VECTOR top,r,center,r0,r1,r2;
    double dth;
    double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
    double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
    dth = 2*PI/N;
    center = g_vector3(center_x, center_y, center_z);
    top = g_vector3(head_size, 0, 0);
	r = g_vector3(0,radius,0);

	for(i=0;i<N;i++)
	{
		r0 = center;
		r1 = Heiko3Ds(Ry(Rz(Rx(Rx(r,psi),i*dth),phi),theta),center);
		r2 = Heiko3Ds(Ry(Rz(Rx(Rx(r,psi),(i+1)*dth),phi),theta),center);
		if(WireFill==1)
		{
			g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r2.x, r2.y, r2.z,
					DivideLevel, WireFill);
		}
		if(WireFill==0)
		{
			g_move_3D(r1.x,r1.y,r1.z);
			g_plot_3D(r2.x,r2.y,r2.z);
		}
		r0 = Heiko3Ds(Ry(Rz(top,phi),theta),center);
		r1 = Heiko3Ds(Ry(Rz(Rx(Rx(r,psi),i*dth),phi),theta),center);
		r2 = Heiko3Ds(Ry(Rz(Rx(Rx(r,psi),(i+1)*dth),phi),theta),center);
		if(WireFill==1)
		{
			g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r2.x, r2.y, r2.z,
					DivideLevel, WireFill);
		}
		if(WireFill==0)
		{
			g_move_3D(r0.x,r0.y,r0.z);
			g_plot_3D(r1.x,r1.y,r1.z);
		}
	}
}

void g_pyramid_3D(double center_x, double center_y, double center_z,                      //中心座標
		double direction_x, double direction_y, double direction_z,             //方向
		double radius,double head_size, double psi,                             //半径、高さ
		int N)
{
	g_pyramid_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, psi,  N, 0, G_FILL);
}
