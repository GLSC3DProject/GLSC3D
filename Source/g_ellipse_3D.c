#include "glsc3d_private.h"

//#define USE_g_triangle_3D_core

void g_ellipse_3D_core(double x, double y, double z,                                 //中心座標
	double Sx, double Sy, double Sz,                              //x,y,z方向への拡大率
	double direction_x, double direction_y, double direction_z,
	int FaceNumberLevel, int DivideLevel, G_WIREFILL WireFill)
{
	int theta, phi;
	int Nt = FaceNumberLevel * 3, Np = 2 * Nt;
	double dth = M_PI / (Nt - 1);
	double dp = 2.0 * M_PI / (Np - 1);
	double a = 1, b = 0, c = 1, d = 0, P, Q, R, S;
	double atmp, btmp, ctmp, dtmp;
	double A = cos(dth), B = sin(dth), C = cos(dp), D = sin(dp);
	double beta = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double alpha = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
	G_POSITION X = { x, y, z }, r0, r1, r2, r3;
	G_VECTOR center = g_vector3(x, y, z);
	G_VECTOR SCALE = g_vector3(Sx, Sy, Sz);

#ifndef USE_g_triangle_3D_core
	G_VERTEX   v0, v1, v2, v3;
	G_TRIANGLE t0, t1;
	G_VECTOR n0, n1, n2, n3;
	G_VECTOR n_SCALE = g_vector3(1 / Sx, 1 / Sy, 1 / Sz);
#endif


	for (theta = 0; theta < Nt - 1; ++theta)
	{
		for (phi = 0; phi < Np - 1; phi++)
		{
			P = (b * A + a * B); Q = (a * A - b * B);
			R = (c * C - d * D); S = (d * C + c * D);

			r0 = g_position(b * c, b * d, a);
			r0 = g_plus(center, Ry(Rz(Scaling3Ds(r0, SCALE), beta), alpha));
			r1 = g_position(P * c, P * d, Q);
			r1 = g_plus(center, Ry(Rz(Scaling3Ds(r1, SCALE), beta), alpha));
			r2 = g_position(b * R, b * S, a);
			r2 = g_plus(center, Ry(Rz(Scaling3Ds(r2, SCALE), beta), alpha));
			r3 = g_position(P * R, P * S, Q);
			r3 = g_plus(center, Ry(Rz(Scaling3Ds(r3, SCALE), beta), alpha));

#ifdef USE_g_triangle_3D_core
			r0 = g_plus(r0, X);            r1 = g_plus(r1, X);
			r2 = g_plus(r2, X);            r3 = g_plus(r3, X);
			if (theta < Nt - 2)
				g_triangle_3D_core(r0.x, r0.y, r0.z,
					r1.x, r1.y, r1.z,
					r3.x, r3.y, r3.z,
					DivideLevel, WireFill);

			if (theta > 0)
				g_triangle_3D_core(r0.x, r0.y, r0.z,
					r3.x, r3.y, r3.z,
					r2.x, r2.y, r2.z,
					DivideLevel, WireFill);
#else

			n0 = Scaling3Ds(r0, n_SCALE);
			n1 = Scaling3Ds(r1, n_SCALE);
			n2 = Scaling3Ds(r2, n_SCALE);
			n3 = Scaling3Ds(r3, n_SCALE);

			v0 = g_make_vertex(g_plus(r0, X), g_normalize(n0));
			v1 = g_make_vertex(g_plus(r1, X), g_normalize(n1));
			v2 = g_make_vertex(g_plus(r2, X), g_normalize(n2));
			v3 = g_make_vertex(g_plus(r3, X), g_normalize(n3));

			t0 = g_make_triangle_core(v0, v1, v3);
			t1 = g_make_triangle_core(v0, v3, v2);
			if (theta < Nt - 2)g_set_triangle(t0);
			if (theta > 0)g_set_triangle(t1);
#endif

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

void g_ellipse_3D(double x, double y, double z,                                 //中心座標
	double Sx, double Sy, double Sz,                              //x,y,z方向への拡大率
	double direction_x, double direction_y, double direction_z)   //方向
{
	g_ellipse_3D_core(x, y, z, Sx, Sy, Sz, direction_x, direction_y, direction_z, 12, 0, 1);
}
