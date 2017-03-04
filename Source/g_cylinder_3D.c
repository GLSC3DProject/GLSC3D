#include "glsc3d_private.h"
void g_cylinder_3D_core(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z,    //向き
		double radius,double height, double psi, int N, int DivideLevel, G_WIREFILL WireFill)                //半径、高さ、側面数
{
	int i;
	G_VECTOR top,center,r,r0,r1,r2,r3,n_x;
	double dth = 2*PI/N;
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));

	//G_MATRIX mat;

	center = g_vector3(center_x, center_y, center_z);
	top = g_vector3(height, 0, 0);
	r = g_vector3(0,radius,0);
	n_x = g_vector3(1,0,0);

	if(WireFill == 1)
	{
		for(i=0;i<N;i++)
		{
			r0 = Heiko3Ds(Ry(Rz(g_multi(-0.5,top),phi),theta),center);
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),i*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),(i+1)*dth),phi),theta),center);
			g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r2.x, r2.y, r2.z,
					DivideLevel, WireFill);
			r0 = Heiko3Ds(Ry(Rz(g_multi(0.5,top),phi),theta),center);
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),i*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),(i+1)*dth),phi),theta),center);
			g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r2.x, r2.y, r2.z,
					DivideLevel, WireFill);
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
		}
	}



	for(i=0;i<N;i++)
	{
		r0 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),i*dth),phi),theta),center);
		r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),i*dth),phi),theta),center);
		r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(-0.5,top)),(i+1)*dth),phi),theta),center);
		r3 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(Rx(r,psi),g_multi(0.5,top)),(i+1)*dth),phi),theta),center);

		if(WireFill == 1)
		{
			G_TRIANGLE t0,t1;
			G_VECTOR n0,n1,n2,n3;
			G_VERTEX   v0, v1, v2, v3;

			n0 = g_plus(Ry(Rz(Rx(Rx(r,psi),(i-0.5)*dth+PI/N),phi),theta),Ry(Rz(Rx(Rx(r,psi),(i+0.5)*dth+PI/N),phi),theta));
			n1 = g_plus(Ry(Rz(Rx(Rx(r,psi),(i-0.5)*dth+PI/N),phi),theta),Ry(Rz(Rx(Rx(r,psi),(i+0.5)*dth+PI/N),phi),theta));
			n2 = g_plus(Ry(Rz(Rx(Rx(r,psi),(i+0.5)*dth+PI/N),phi),theta),Ry(Rz(Rx(Rx(r,psi),(i+1.5)*dth+PI/N),phi),theta));
			n3 = g_plus(Ry(Rz(Rx(Rx(r,psi),(i+0.5)*dth+PI/N),phi),theta),Ry(Rz(Rx(Rx(r,psi),(i+1.5)*dth+PI/N),phi),theta));
			n0 = g_normalize(n0);
			n1 = g_normalize(n1);
			n2 = g_normalize(n2);
			n3 = g_normalize(n3);

			v0 = g_make_vertex(r0,n0);
			v1 = g_make_vertex(r1,n1);
			v2 = g_make_vertex(r2,n2);
			v3 = g_make_vertex(r3,n3);

			t0 = g_make_triangle_core(v0, v1, v2);
			t1 = g_make_triangle_core(v1, v3, v2);
			g_set_triangle(t0);
			g_set_triangle(t1);
		}
		if(WireFill == 0)
		{
			g_move_3D(r0.x,r0.y,r0.z);
			g_plot_3D(r1.x,r1.y,r1.z);
			g_move_3D(r2.x,r2.y,r2.z);
			g_plot_3D(r3.x,r3.y,r3.z);
		}
	}
}

void g_cylinder_3D(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z,    //向き
		double radius,double height, double psi)                //半径、高さ、側面数
{
	g_cylinder_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, height, psi, 50, 0, 1);
}
