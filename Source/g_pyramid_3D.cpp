#include "glsc3d_private.h"

void g_pyramid_3D_core(double center_x, double center_y, double center_z,                      //中心座標
					   double direction_x, double direction_y, double direction_z,             //方向
					   double radius,double head_size, double psi,                             //半径、高さ
					   int N, int DivideLevel, G_BOOL WIRE, G_BOOL FILL)
{

//	int i;
//	G_VECTOR top,r,center,r0,r1,r2;
//	double dth;
//	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
//	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
//	dth = 2*PI/N;
//	center = g_vector3(center_x, center_y, center_z);
//	top = g_vector3(head_size, 0, 0);
//	r = g_vector3(0,radius,0);
//
//	for(i=0;i<N;i++)
//	{
//		r0 = center;
//		r1 = g_plus(Ry(Rz(Rx(Rx(r,psi),i*dth),phi),theta),center);
//		r2 = g_plus(Ry(Rz(Rx(Rx(r,psi),(i+1)*dth),phi),theta),center);
//		if(WireFill==1)
//		{
//			g_triangle_3D_core(r0.x, r0.y, r0.z,
//							   r1.x, r1.y, r1.z,
//							   r2.x, r2.y, r2.z,
//							   DivideLevel, WireFill);
//		}
//		if(WireFill==0)
//		{
//			g_move_3D(r1.x,r1.y,r1.z);
//			g_plot_3D(r2.x,r2.y,r2.z);
//		}
//		r0 = g_plus(Ry(Rz(top,phi),theta),center);
//		r1 = g_plus(Ry(Rz(Rx(Rx(r,psi),i*dth),phi),theta),center);
//		r2 = g_plus(Ry(Rz(Rx(Rx(r,psi),(i+1)*dth),phi),theta),center);
//		if(WireFill==1)
//		{
//			g_triangle_3D_core(r0.x, r0.y, r0.z,
//							   r2.x, r2.y, r2.z,
//							   r1.x, r1.y, r1.z,
//							   DivideLevel, WireFill);
//		}
//		if(WireFill==0)
//		{
//			g_move_3D(r0.x,r0.y,r0.z);
//			g_plot_3D(r1.x,r1.y,r1.z);
//		}
//	}

	int i;
	double dth = 2 * PI / N;
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
	G_VECTOR center = g_vector3(center_x, center_y, center_z);
	G_VECTOR top = g_vector3(head_size, 0, 0);
	G_VECTOR r = g_vector3(0,radius,0);

	G_VECTOR rotation_top = Ry(Rz(top,phi),theta) + center;

	int Data_Size_r_i = N;
	int Data_Size_n_i_lateral = N;
	int Data_Size = Data_Size_r_i * sizeof(G_VECTOR) + Data_Size_n_i_lateral * sizeof(G_VECTOR);
	GLSC3D_Data_Buffer = GLSC3D_Array_Buffer(Data_Size);
	G_VECTOR *r_i = (G_VECTOR *)GLSC3D_Data_Buffer;
	G_VECTOR *n_i_lateral = r_i + Data_Size_r_i;

	G_VECTOR center_normal;

	for(i=0; i<N; i++) {
		r_i[i] = Ry(Rz(Rx(Rx(r,psi),i*dth),phi),theta) + center;
	}
	for(i=0; i<N; i++) {
		n_i_lateral[i] = g_cross(r_i[i]-rotation_top, r_i[(i+1)%N]-rotation_top);
	}
	center_normal = g_normalize(center - rotation_top);

	for(i=0;i<N;i++)
	{
		if(FILL) {
			g_triangle_3D_smooth_worker(r_i[i], r_i[(i + 1) % N], rotation_top, n_i_lateral[i], n_i_lateral[i], n_i_lateral[i], DivideLevel);
			g_triangle_3D_smooth_worker(r_i[(i + 1) % N], r_i[i], center, center_normal, center_normal, center_normal, DivideLevel);
		}

		if(WIRE)
		{
			g_move_3D(rotation_top.x,rotation_top.y,rotation_top.z);
			g_plot_3D(r_i[i].x,r_i[i].y,r_i[i].z);

			g_move_3D(r_i[i].x,r_i[i].y,r_i[i].z);
			g_plot_3D(r_i[(i+1)%N].x,r_i[(i+1)%N].y,r_i[(i+1)%N].z);
		}
	}

}

void g_pyramid_3D(double center_x, double center_y, double center_z,                      //中心座標
				  double direction_x, double direction_y, double direction_z,             //方向
				  double radius,double head_size, double psi,                             //半径、高さ
				  int N)
{
	g_pyramid_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, psi,  N, 0, G_FALSE, G_FILL);
}
