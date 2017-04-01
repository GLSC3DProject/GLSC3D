#include "glsc3d_private.h"
void g_pyramid_3D_core(double center_x, double center_y, double center_z,                      //中心座標
					   double direction_x, double direction_y, double direction_z,             //方向
					   double radius,double head_size, double psi,                             //半径、高さ
					   int N, int DivideLevel, G_WIREFILL WireFill)
{

	int i;
	double dth = 2 * PI / N;
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
	G_VECTOR center = g_vector3(center_x, center_y, center_z);
	G_VECTOR top = g_vector3(head_size, 0, 0);
	G_VECTOR r = g_vector3(0,radius,0);

	G_VECTOR rotation_top = Ry(Rz(top,phi),theta) + center;
	G_VECTOR r_i[N], n_i_lateral[N], n_i[N];
	G_VECTOR top_normal, center_normal;

	for(i=0; i<N; i++) {
		r_i[i] = Ry(Rz(Rx(Rx(r,psi),i*dth),phi),theta) + center;
	}
	for(i=0; i<N; i++) {
		n_i_lateral[i] = g_cross(r_i[i]-rotation_top, r_i[(i+1)%N]-rotation_top);
	}
	top_normal = g_normalize(rotation_top - center);
	center_normal = g_normalize(center - rotation_top);

	for(i=0; i<N; i++) {
		n_i[i] = g_normalize(g_normalize(n_i_lateral[i]) + g_normalize(n_i_lateral[(i+N-1)%N]) + center_normal);
	}

	for(i=0;i<N;i++)
	{
		if(WireFill == 1) {
			// 点法線
			g_triangle_3D_core_smooth(r_i[i], r_i[(i + 1) % N], rotation_top, n_i[i], n_i[(i + 1) % N], top_normal, DivideLevel);
			g_triangle_3D_core_smooth(r_i[(i + 1) % N], r_i[i], center, n_i[(i + 1) % N], n_i[i], center_normal, DivideLevel);
			// 面法線
//			g_triangle_3D_core_smooth(r_i[i], r_i[(i + 1) % N], rotation_top, n_i_lateral[i], n_i_lateral[i], n_i_lateral[i], DivideLevel);
//			g_triangle_3D_core_smooth(r_i[(i + 1) % N], r_i[i], center, center_normal, center_normal, center_normal, DivideLevel);
		}

		if(WireFill == 0)
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
				  int N, G_WIREFILL WireFill)
{
	g_pyramid_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, psi,  N, 0, WireFill);
}