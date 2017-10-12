#include "glsc3d_3_private.h"

void g_ellipse_3D_core(
	double x, double y, double z,                                 //中心座標
	double Sx, double Sy, double Sz,                              //x,y,z方向への拡大率
	double direction_x, double direction_y, double direction_z,
	int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	int theta, phi;
	int Nt = FaceNumberLevel * 2, Np = 2 * Nt;
	double dth = M_PI / Nt;
	double dp = 2.0 * M_PI / Np;
	double a = 1, b = 0, c = 1, d = 0, P, Q, R, S;
	double atmp, btmp, ctmp, dtmp;
	double A = cos(dth), B = sin(dth), C = cos(dp), D = sin(dp);
	double beta = g_direction_phi(G_VECTOR(direction_x, direction_y, direction_z));
	double alpha = g_direction_theta(G_VECTOR(direction_x, direction_y, direction_z));
	G_VECTOR r0, r1, r2, r3;
//	X = G_VECTOR( x, y, z );
	G_VECTOR center = G_VECTOR(x, y, z);
	G_VECTOR SCALE = G_VECTOR(Sx, Sy, Sz);
	G_VECTOR n0, n1, n2, n3;
	G_VECTOR n_SCALE = G_VECTOR(1 / Sx, 1 / Sy, 1 / Sz);

	for (theta = 0; theta < Nt; theta++)
	{
		for (phi = 0; phi < Np; phi++)
		{
			P = (b * A + a * B); Q = (a * A - b * B);
			R = (c * C - d * D); S = (d * C + c * D);

			r0 = G_VECTOR(b * c, b * d, a);
			r0 = center + Ry(Rz(Scaling3Ds(r0, SCALE), beta), alpha);
			r1 = G_VECTOR(P * c, P * d, Q);
			r1 = center + Ry(Rz(Scaling3Ds(r1, SCALE), beta), alpha);
			r2 = G_VECTOR(b * R, b * S, a);
			r2 = center + Ry(Rz(Scaling3Ds(r2, SCALE), beta), alpha);
			r3 = G_VECTOR(P * R, P * S, Q);
			r3 = center + Ry(Rz(Scaling3Ds(r3, SCALE), beta), alpha);

			n0 = Scaling3Ds(r0, n_SCALE);
			n1 = Scaling3Ds(r1, n_SCALE);
			n2 = Scaling3Ds(r2, n_SCALE);
			n3 = Scaling3Ds(r3, n_SCALE);

			g_triangle_3D_smooth_core_s(
					r0,r1,r3,
					n0, n1, n3,
					DivideLevel, Wire, Fill
			);
			g_triangle_3D_smooth_core_s(
					r0,r3,r2,
					n0, n3, n2,
					DivideLevel, Wire, Fill
			);
			ctmp = c;
			dtmp = d;
			c = ctmp * C - dtmp * D;
			d = dtmp * C + ctmp * D;
		}
		atmp = a;
		btmp = b;
		a = atmp * A - btmp * B;
		b = btmp * A + atmp * B;
	}
}

void g_ellipse_3D(
	double x, double y, double z,                                 //中心座標
	double Sx, double Sy, double Sz,                              //x,y,z方向への拡大率
	double direction_x, double direction_y, double direction_z,   //方向
	G_BOOL Wire, G_BOOL Fill)
{
	g_ellipse_3D_core(x, y, z, Sx, Sy, Sz, direction_x, direction_y, direction_z, 12, 0, Wire, Fill);
}

void g_ellipse_3D_Euler(
	double x, double y, double z,                                 //中心座標
	double Sx, double Sy, double Sz,                              //x,y,z方向への拡大率
	double angle_alpha, double angle_beta, double angle_gamma,    //Euler角
	int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	int theta, phi;
	int Nt = FaceNumberLevel * 2, Np = 2 * Nt;
	double dth = M_PI / Nt;
	double dp = 2.0 * M_PI / Np;
	double a = 1, b = 0, c = 1, d = 0, P, Q, R, S;
	double atmp, btmp, ctmp, dtmp;
	double A = cos(dth), B = sin(dth), C = cos(dp), D = sin(dp);

	G_VECTOR r0, r1, r2, r3;
//	X = G_VECTOR( x, y, z );
	G_VECTOR center = G_VECTOR(x, y, z);
	G_VECTOR SCALE = G_VECTOR(Sx, Sy, Sz);
	G_VECTOR n0, n1, n2, n3;
	G_VECTOR n_SCALE = G_VECTOR(1 / Sx, 1 / Sy, 1 / Sz);

	for (theta = 0; theta < Nt; theta++)
	{
		for (phi = 0; phi < Np; phi++)
		{
			P = (b * A + a * B); Q = (a * A - b * B);
			R = (c * C - d * D); S = (d * C + c * D);

			r0 = G_VECTOR(b * c, b * d, a);
			r0 = center + Rz(Rx(Rz(Scaling3Ds(r0, SCALE), angle_alpha), angle_beta), angle_gamma);
			r1 = G_VECTOR(P * c, P * d, Q);
			r1 = center + Rz(Rx(Rz(Scaling3Ds(r1, SCALE), angle_alpha), angle_beta), angle_gamma);
			r2 = G_VECTOR(b * R, b * S, a);
			r2 = center + Rz(Rx(Rz(Scaling3Ds(r2, SCALE), angle_alpha), angle_beta), angle_gamma);
			r3 = G_VECTOR(P * R, P * S, Q);
			r3 = center + Rz(Rx(Rz(Scaling3Ds(r3, SCALE), angle_alpha), angle_beta), angle_gamma);

			n0 = Scaling3Ds(r0, n_SCALE);
			n1 = Scaling3Ds(r1, n_SCALE);
			n2 = Scaling3Ds(r2, n_SCALE);
			n3 = Scaling3Ds(r3, n_SCALE);

			g_triangle_3D_smooth_core_s(
				r0,r1,r3,
				n0, n1, n3,
				DivideLevel, Wire, Fill
			);
			g_triangle_3D_smooth_core_s(
				r0,r3,r2,
				n0, n3, n2,
				DivideLevel, Wire, Fill
			);
			ctmp = c;
			dtmp = d;
			c = ctmp * C - dtmp * D;
			d = dtmp * C + ctmp * D;
		}
		atmp = a;
		btmp = b;
		a = atmp * A - btmp * B;
		b = btmp * A + atmp * B;
	}
}