#include "glsc3d_private.h"

#define USE_g_triangle_3D_core

void g_sphere_3D_core(double x, double y, double z, double radius, int FaceNumberLevel, int DivideLevel, G_WIREFILL WireFill)
{
	int theta, phi;
	int Nt = FaceNumberLevel * 3, Np = 2 * Nt;
	double dth = M_PI / (Nt - 1);
	double dp = 2.0 * M_PI / (Np - 1);
	double a = 1, b = 0, c = 1, d = 0, P, Q, R, S;
	double atmp, btmp, ctmp, dtmp;
	double A = cos(dth), B = sin(dth), C = cos(dp), D = sin(dp);
	G_POSITION r0, r1, r2, r3, X( x, y, z );
#ifndef USE_g_triangle_3D_core
	G_VERTEX   v0, v1, v2, v3;
	G_TRIANGLE t0, t1;
	double r_inv = 1 / radius;
#endif
	for (theta = 0; theta < Nt - 1; theta++)
	{
		for (phi = 0; phi < Np - 1; phi++)
		{
			P = (b * A + a * B); Q = (a * A - b * B);
			R = (c * C - d * D); S = (d * C + c * D);
			r0 = g_position(radius * b * c, radius * b * d, radius * a);
			r1 = g_position(radius * P * c, radius * P * d, radius * Q);
			r2 = g_position(radius * b * R, radius * b * S, radius * a);
			r3 = g_position(radius * P * R, radius * P * S, radius * Q);

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
			v0 = g_make_vertex(g_plus(r0, X), g_multi(r_inv, r0));
			v1 = g_make_vertex(g_plus(r1, X), g_multi(r_inv, r1));
			v2 = g_make_vertex(g_plus(r2, X), g_multi(r_inv, r2));
			v3 = g_make_vertex(g_plus(r3, X), g_multi(r_inv, r3));
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

void g_sphere_3D(double x, double y, double z, double radius)
{
	g_sphere_3D_core(x, y, z, radius, 12, 0, G_FILL);
}
