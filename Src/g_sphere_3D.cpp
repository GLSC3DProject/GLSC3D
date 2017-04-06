#include "glsc3d_private.h"

void g_sphere_3D_core(double x, double y, double z, double radius, int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill)
{
	int theta, phi;
	int Nt = FaceNumberLevel * 2, Np = 2 * Nt;
	double dth = M_PI / Nt;
	double dp = 2.0 * M_PI / Np;
	double a = 1, b = 0, c = 1, d = 0, P, Q, R, S;
	double atmp, btmp, ctmp, dtmp;
	double A = cos(dth), B = sin(dth), C = cos(dp), D = sin(dp);
	G_VECTOR r0, r1, r2, r3, X(x,y,z);
	float r_inv = 1 / (float)radius;

	for (theta = 0; theta < Nt; theta++)
	{
		for (phi = 0; phi < Np; phi++)
		{
			P = (b * A + a * B); Q = (a * A - b * B);
			R = (c * C - d * D); S = (d * C + c * D);
			r0 = G_VECTOR (radius * b * c, radius * b * d, radius * a);
			r1 = G_VECTOR (radius * P * c, radius * P * d, radius * Q);
			r2 = G_VECTOR (radius * b * R, radius * b * S, radius * a);
			r3 = G_VECTOR (radius * P * R, radius * P * S, radius * Q);

			g_triangle_3D_smooth_core_s(
					r0 + X,r1 + X,r3 + X,
					r_inv * r0,r_inv * r1,r_inv * r3,
					DivideLevel, Wire, Fill
			);
			g_triangle_3D_smooth_core_s(
					r0 + X,r3 + X,r2 + X,
					r_inv * r0,r_inv * r3,r_inv * r2,
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

void g_sphere_3D(double x, double y, double z, double radius, G_BOOL Wire, G_BOOL Fill)
{
	g_sphere_3D_core(x, y, z, radius, 12, 0, Wire, Fill);
}
