#ifndef GLSC3D_3_PRIVATE_MATH_H
#define GLSC3D_3_PRIVATE_MATH_H

#include "glsc3d_3_math.h"

struct G_VECTOR4
{
	float x, y, z, w;

	G_VECTOR4() {}
	G_VECTOR4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	G_VECTOR4(G_VECTOR v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
};

inline G_VECTOR4 operator +(G_VECTOR4 u, G_VECTOR4 v)
{
	return G_VECTOR4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

inline G_VECTOR4 operator *(float a, G_VECTOR4 u)
{
	return G_VECTOR4(a * u.x, a * u.y, a * u.z, a * u.w);
}

struct G_MATRIX
{
	G_VECTOR4 x, y, z, w;

	G_MATRIX() {}
	G_MATRIX(G_VECTOR4 x, G_VECTOR4 y, G_VECTOR4 z, G_VECTOR4 w)
		: x(x), y(y), z(z), w(w) {}
	G_MATRIX(
		float xx, float xy, float xz, float xw,
		float yx, float yy, float yz, float yw,
		float zx, float zy, float zz, float zw,
		float wx, float wy, float wz, float ww
	) :
		x(xx, xy, xz, xw),
		y(yx, yy, yz, yw),
		z(zx, zy, zz, zw),
		w(wx, wy, wz, ww) {}

	static G_MATRIX Identity()
	{
		return G_MATRIX(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	static G_MATRIX RotationX(float angle)
	{
		float c = cos(angle), s = sin(angle);

		return G_MATRIX(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		);
	}

	static G_MATRIX RotationY(float angle)
	{
		float c = cos(angle), s = sin(angle);

		return G_MATRIX(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1
		);
	}

	static G_MATRIX RotationZ(float angle)
	{
		float c = cos(angle), s = sin(angle);

		return G_MATRIX(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	static G_MATRIX Translation(G_VECTOR v)
	{
		return G_MATRIX(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			v.x, v.y, v.z, 1
		);
	}

	static G_MATRIX Scaling(float x, float y, float z)
	{
		return G_MATRIX(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
	}

	static G_MATRIX Ortho2D(float left, float right, float bottom, float top)
	{
		float a = 1 / (right - left);
		float b = 1 / (top - bottom);

		return G_MATRIX(
			2*a, 0, 0, 0,
			0, 2*b, 0, 0,
			0, 0, 1, 0,
			-(right+left)*a, -(top+bottom)*b, 0, 1
		);
	}

	static G_MATRIX Perspective(float c, float aspect, float near, float far)
	{
		//float c = 1 / tan(fov / 2);

		return G_MATRIX(
			c / aspect, 0, 0, 0,
			0, c, 0, 0,
#ifdef G_USE_METAL
			0, 0, far / (near - far), -1,
			0, 0, near*far / (near - far), 0
#else
			0, 0, (near + far) / (near - far), -1,
			0, 0, 2*near*far / (near - far), 0
#endif
		);
	}

	static G_MATRIX LookAt(G_VECTOR eye, G_VECTOR center, G_VECTOR up)
	{
		G_VECTOR Z = g_normalize(eye - center);
		G_VECTOR X = g_normalize(g_cross(up, Z));
		G_VECTOR Y = g_normalize(g_cross(Z, X));

		float ex = g_dot(X, eye);
		float ey = g_dot(Y, eye);
		float ez = g_dot(Z, eye);

		return G_MATRIX(
			X.x, Y.x, Z.x, 0,
			X.y, Y.y, Z.y, 0,
			X.z, Y.z, Z.z, 0,
			-ex, -ey, -ez, 1
		);
	}
};

inline G_VECTOR operator *(G_VECTOR v, const G_MATRIX &m)
{
	return G_VECTOR(
		v.x * m.x.x + v.y * m.y.x + v.z * m.z.x + m.w.x,
		v.x * m.x.y + v.y * m.y.y + v.z * m.z.y + m.w.y,
		v.x * m.x.z + v.y * m.y.z + v.z * m.z.z + m.w.z
	);
}

inline G_VECTOR g_transform_normal(G_VECTOR v, const G_MATRIX &m)
{
	return G_VECTOR(
		v.x * m.x.x + v.y * m.y.x + v.z * m.z.x,
		v.x * m.x.y + v.y * m.y.y + v.z * m.z.y,
		v.x * m.x.z + v.y * m.y.z + v.z * m.z.z
	);
}

inline G_VECTOR4 operator *(G_VECTOR4 v, const G_MATRIX &m)
{
	return G_VECTOR4(
		v.x * m.x.x + v.y * m.y.x + v.z * m.z.x + v.w * m.w.x,
		v.x * m.x.y + v.y * m.y.y + v.z * m.z.y + v.w * m.w.y,
		v.x * m.x.z + v.y * m.y.z + v.z * m.z.z + v.w * m.w.z,
		v.x * m.x.w + v.y * m.y.w + v.z * m.z.w + v.w * m.w.w
	);
}

inline G_MATRIX operator *(const G_MATRIX &A, const G_MATRIX &B)
{
	return G_MATRIX(
		A.x.x * B.x + A.x.y * B.y + A.x.z * B.z + A.x.w * B.w,
		A.y.x * B.x + A.y.y * B.y + A.y.z * B.z + A.y.w * B.w,
		A.z.x * B.x + A.z.y * B.y + A.z.z * B.z + A.z.w * B.w,
		A.w.x * B.x + A.w.y * B.y + A.w.z * B.z + A.w.w * B.w
	);
}

inline void operator *=(G_MATRIX &A, const G_MATRIX &B)
{
	A = A * B;
}

#endif
