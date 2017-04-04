#include "glsc3d_private.h"
void g_cone_2D(double center_x, double center_y,       //底面の中心座標
			   double direction_x, double direction_y, //方向
			   double radius, double head_size,        //半径、高さ
			   int type)
{
	G_VECTOR n = g_normalize(g_vector2(direction_x, direction_y));
	G_VECTOR center = g_vector2(center_x, center_y);
	G_VECTOR r_1 = g_vector2(0, radius);
	G_VECTOR r_2 = g_vector2(0, -radius);
	double side = sqrt(head_size*head_size + radius*radius);
	if(type == 1)
	{
		g_begin_triangles();
		g_emit_vertex(n * head_size + center);
		g_emit_vertex(Rx2D(r_1, atan2(direction_y, direction_x)) + center);
		g_emit_vertex(Rx2D(r_2, atan2(direction_y, direction_x)) + center);
	}
	if(type == 0)
	{
		g_begin_line_loop();
		g_emit_vertex(n * head_size + center);
		g_emit_vertex(Rx2D(r_1, atan2(direction_y, direction_x)) + center);
		g_emit_vertex(Rx2D(r_2, atan2(direction_y, direction_x)) + center);
	}
	//	if(type == 2)
	//	{
	//		g_begin_line_strip();
	//		g_emit_vertex(g_minus(g_plus(center, g_multi(head_size, n)), Rx2D(g_multi(side, n), atan(radius / head_size))));
	//		g_emit_vertex(g_plus(center, g_multi(head_size, n)));
	//		g_emit_vertex(g_minus(g_plus(center, g_multi(head_size, n)), Rx2D(g_multi(side, n), -atan(radius / head_size))));
	//	}
}

void g_cone_3D_core(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z, //方向
		double radius, double head_size,
		int N, int DivideLevel, G_WIREFILL WireFill)
{
	int i;

	G_VECTOR top,r;
	G_VECTOR r0,r1,r2,r3;
	G_VECTOR center;

	double alpha, dth = 2*PI/N;

	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));

	G_VECTOR n0, n1, n2, n3;

	center = g_vector3(center_x,center_y,center_z);
	alpha = -atan(radius/head_size);

	top = g_vector3(head_size, 0, 0);
	r = g_vector3(0,radius,0);
	r0 = center;

	G_VECTOR n_center = Ry(Rz(g_normalize(top),phi),theta);

	for(i=0; i<N; i++)
	{
		r1 = Ry(Rz(Rx(r,i*dth),phi),theta) + center;
		r2 = Ry(Rz(Rx(r,(i+1)*dth),phi),theta) + center;

		if(WireFill==1)
		{
			g_triangle_3D_core_smooth(r0, r1, r2, n_center, n_center, n_center, DivideLevel);
		}
		if(WireFill==0)
		{
			g_move_3D(r1.x,r1.y,r1.z);
			g_plot_3D(r2.x,r2.y,r2.z);
		}
	}

	for(i=0;i<N;i++)
	{
		r0 = Ry(Rz(Rx(top,i*dth),phi),theta) + center;
		r1 = Ry(Rz(Rx(r,i*dth),phi),theta) + center;
		r2 = Ry(Rz(Rx(top,(i+1)*dth),phi),theta) + center;
		r3 = Ry(Rz(Rx(r,(i+1)*dth),phi),theta) + center;

		if(WireFill==1)
		{
			n0 = Ry(Rz(Rx(Rz(r,alpha),(i-0.5)*dth),phi),theta) + Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta);
			n0 = g_multi(1/g_norm(n0),n0);
			n1 = Ry(Rz(Rx(Rz(r,alpha),(i-0.5)*dth),phi),theta) + Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta);
			n1 = g_multi(1/g_norm(n1),n1);
			n2 = Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta) + Ry(Rz(Rx(Rz(r,alpha),(i+1.5)*dth),phi),theta);
			n2 = g_multi(1/g_norm(n2),n2);
			n3 = Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta) + Ry(Rz(Rx(Rz(r,alpha),(i+1.5)*dth),phi),theta);
			n3 = g_multi(1/g_norm(n3),n3);

			g_triangle_3D_core_smooth(r0, r1, r2, n0, n1, n2, DivideLevel);
			g_triangle_3D_core_smooth(r1, r3, r2, n1, n3, n2, DivideLevel);

		}
		if(WireFill==0)
		{
			g_move_3D(r0.x,r0.y,r0.z);
			g_plot_3D(r1.x,r1.y,r1.z);
			g_move_3D(r2.x,r2.y,r2.z);
			g_plot_3D(r3.x,r3.y,r3.z);
		}
	}
}

void g_cone_3D(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z, //方向
		double radius,double head_size)                             //半径、高さ
{
	g_cone_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, 100, 0, G_WIRE);
}
