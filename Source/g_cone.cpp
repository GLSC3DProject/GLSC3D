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
					int DivideLevel, G_WIREFILL WireFill)
{
	g_pyramid_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, 0,  100, DivideLevel, G_FILL);
}

void g_cone_3D(double center_x, double center_y, double center_z,          //中心座標
			   double direction_x, double direction_y, double direction_z, //方向
			   double radius,double head_size)                             //半径、高さ
{
	g_cone_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, 0, G_FILL);
}

//void g_cone_3D_core(double center_x, double center_y, double center_z,          //中心座標
//		double direction_x, double direction_y, double direction_z, //方向
//		double radius, double head_size,
//		int FaceNumberLevel_X, int FaceNumberLevel_Y, int DivideLevel, G_WIREFILL WireFill)
//{
//	int i,j,n = FaceNumberLevel_X,stack = FaceNumberLevel_Y;
//	G_VECTOR top,r,r0,r1,r2,r3,center;
//	double alpha,dth,stack_size=1/(double)stack;
//	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
//	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
//	dth = 2*PI/n;
//	center = g_vector3(center_x,center_y,center_z);
//	alpha = -atan(radius/head_size);
//	top = g_vector3(head_size, 0, 0);
//	r = g_vector3(0,radius,0);
//
//	//r0 = g_plus(Ry(Rz(r,phi),theta),center);
//	r0 = center;
//	for(i=0;i<n;i++)
//	{
//		r1 = g_plus(Ry(Rz(Rx(r,i*dth),phi),theta),center);
//		r2 = g_plus(Ry(Rz(Rx(r,(i+1)*dth),phi),theta),center);
//		if(WireFill==1)
//		{
//			g_triangle_3D_core(r0.x,r0.y,r0.z,
//					r1.x,r1.y,r1.z,
//					r2.x,r2.y,r2.z,
//					DivideLevel,WireFill);
//		}
//		if(WireFill==0)
//		{
//			g_move_3D(r1.x,r1.y,r1.z);
//			g_plot_3D(r2.x,r2.y,r2.z);
//		}
//	}
//
//	for(j=0;j<stack;j++)
//	{
//		for(i=0;i<n;i++)
//		{
//			r0 = g_plus(Ry(Rz(Rx(g_plus(g_multi(j*stack_size,r),g_multi(1-j*stack_size,top)),i*dth),phi),theta),center);
//			r1 = g_plus(Ry(Rz(Rx(g_plus(g_multi((j+1)*stack_size,r),g_multi(1-(j+1)*stack_size,top)),i*dth),phi),theta),center);
//			r2 = g_plus(Ry(Rz(Rx(g_plus(g_multi(j*stack_size,r),g_multi(1-j*stack_size,top)),(i+1)*dth),phi),theta),center);
//			r3 = g_plus(Ry(Rz(Rx(g_plus(g_multi((j+1)*stack_size,r),g_multi(1-(j+1)*stack_size,top)),(i+1)*dth),phi),theta),center);
//
//			if(WireFill==1)
//			{
//				G_VECTOR   n0,n1,n2,n3;
//				G_VERTEX   v0, v1, v2, v3;
//				G_TRIANGLE t0, t1;
//
//				n0 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i-0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta));
//				n0 = g_multi(1/g_norm(n0),n0);
//				n1 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i-0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta));
//				n1 = g_multi(1/g_norm(n1),n1);
//				n2 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+1.5)*dth),phi),theta));
//				n2 = g_multi(1/g_norm(n2),n2);
//				n3 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+1.5)*dth),phi),theta));
//				n3 = g_multi(1/g_norm(n3),n3);
//
//				v0 = g_make_vertex(r0,n0);
//				v1 = g_make_vertex(r1,n1);
//				v2 = g_make_vertex(r2,n2);
//				v3 = g_make_vertex(r3,n3);
//
//				t0 = g_make_triangle_core(v0, v1, v2);
//				t1 = g_make_triangle_core(v1, v3, v2);
//				g_set_triangle(t0);
//				g_set_triangle(t1);
//			}
//			if(WireFill==0)
//			{
//				g_move_3D(r0.x,r0.y,r0.z);
//				g_plot_3D(r1.x,r1.y,r1.z);
//				g_move_3D(r2.x,r2.y,r2.z);
//				g_plot_3D(r3.x,r3.y,r3.z);
//			}
//		}
//	}
//}
//
//void g_cone_3D(double center_x, double center_y, double center_z,          //中心座標
//		double direction_x, double direction_y, double direction_z, //方向
//		double radius,double head_size)                             //半径、高さ
//{
//	g_cone_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, 20, 10, 0, G_FILL);
//}