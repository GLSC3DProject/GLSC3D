#include "glsc3d_3_private.h"

void g_fan_2D(
	double center_x, double center_y,       //中心座標
	double direction_x, double direction_y, //方向
	double radius,                          //半径
	double angle,                           //開く角度
	G_BOOL Wire, G_BOOL Fill)
{
	int i,STEP=8;
	double dtheta;
	G_VECTOR direction = g_vector2(direction_x, direction_y);
	direction = g_normalize(direction);
	G_VECTOR center = g_vector2(center_x, center_y);
	if (Fill)
	{
		g_begin_triangle_fan();
		g_emit_vertex(center);
		for (i = 0; i <= STEP; i++) {
			dtheta = 0.5 * angle / STEP;
			g_emit_vertex(center + Rx2D(radius * direction, (2 * i - STEP) * dtheta));
		}
	}
	if (Wire)
	{
		g_begin_line_strip();
		for (i = 0; i <= STEP; i++)
		{
			dtheta = 0.5*angle / STEP;
			g_emit_vertex(center + Rx2D(radius * direction, (2 * i - STEP)*dtheta));
		}
		g_emit_vertex(center);
		g_emit_vertex(center + Rx2D(radius * direction, (-STEP)*dtheta));
	}
}

void g_fan_3D_core(
	double center_x, double center_y, double center_z,          //中心座標
	double direction_x, double direction_y, double direction_z, //方向
	double radius,                                              //半径
	double angle, double psi,                                   //開く角度、回転角
	int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	int i,STEP=FaceNumberLevel;
	double dtheta = 0.5*angle / STEP;
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
	G_VECTOR r0(center_x, center_y, center_z);
	G_VECTOR nx(1,0,0);
	if (Fill)
	{
		for (i = 0; i < STEP; i++)
		{
			G_VECTOR r1 = r0 + Ry(Rz(Rx(Ry(radius * nx, (2 * i - STEP)*dtheta), psi), phi), theta);
			G_VECTOR r2 = r0 + Ry(Rz(Rx(Ry(radius * nx, (2 * (i + 1) - STEP)*dtheta), psi), phi), theta);
			g_triangle_3D_flat_worker(r0, r1, r2, DivideLevel);
		}
	}

	if (Wire)
	{
		g_move_s(r0);
		for (i = 0; i <= STEP; i++)
		{
			g_plot_s(r0 + Ry(Rz(Rx(Ry(radius * nx, (2 * i - STEP)*dtheta), psi), phi), theta));
		}
		g_plot_s(r0);
	}
}

void g_fan_3D(
	double center_x, double center_y, double center_z,          //中心座標
	double direction_x, double direction_y, double direction_z, //方向
	double radius,                                              //半径
	double angle, double psi,                                   //開く角度、回転角
	G_BOOL Wire, G_BOOL Fill)
{
	g_fan_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, angle, psi, 10, 0, Wire, Fill);
}
