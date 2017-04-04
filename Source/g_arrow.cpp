#include "glsc3d_private.h"

#define SQRT3 1.7320508f

void g_arrow_2D(double base_x, double base_y,                 //根元の座標
                double direction_x, double direction_y,       //方向
                double arrow_size, double head_size,          //全体の長さ、頭の長さ
                int type)
{
    G_VECTOR base = G_VECTOR(base_x, base_y);
    G_VECTOR X = g_normalize(G_VECTOR(direction_x, direction_y));
	G_VECTOR Y = G_VECTOR(-X.y, X.x);

	G_VECTOR center = base + (arrow_size - head_size) * X;
	float delta = head_size / SQRT3;

	G_VECTOR A = center - delta * Y;
	G_VECTOR B = base + arrow_size * X;
	G_VECTOR C = center + delta * Y;

    g_move_s(base);
    g_plot_s(B);

	switch (type) {
	case 0:
		g_begin_line_loop(); break;
	case 1:
		g_begin_triangles(); break;
	case 2:
		g_begin_line_strip(); break;
	}

	g_emit_vertex(A);
	g_emit_vertex(B);
	g_emit_vertex(C);
}

void g_arrow_3D_core(double base_x, double base_y, double base_z,                //根元の座標
                     double direction_x, double direction_y, double direction_z, //方向
                     double arrow_size, double head_size,
                     int N, int DivideLevel, G_WIREFILL WireFill)
{
    G_VECTOR base = g_vector3(base_x,base_y,base_z);
    G_VECTOR direction = g_vector3(direction_x,direction_y,direction_z);
    G_VECTOR n = g_multi(1/g_norm(direction),direction);
    G_VECTOR center = g_plus(base,g_multi(arrow_size-head_size,n));
    g_cone_3D_core(center.x, center.y, center.z,
                   n.x, n.y, n.z,
                   head_size / SQRT3, head_size,
                   N, DivideLevel, WireFill);
    g_move_s(base);
    g_plot_s(center);
}

void g_arrow_3D(double base_x, double base_y, double base_z,                //根元の座標
                double direction_x, double direction_y, double direction_z, //方向
                double arrow_size, double head_size)                        //全体の長さ、頭の長さ
{
    g_arrow_3D_core(base_x, base_y, base_z, direction_x, direction_y, direction_z, arrow_size, head_size, 100, 0, G_FILL);
}