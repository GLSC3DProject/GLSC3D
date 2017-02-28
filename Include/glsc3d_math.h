#ifndef GLSC3D_MATH_H
#define GLSC3D_MATH_H

#include <math.h>
#include <float.h>

typedef float G_REAL;

typedef struct { float r, g, b, a; } G_COLOR;

#ifdef __cplusplus

struct G_VECTOR
{
	float x, y, z;

	G_VECTOR() = default;
	G_VECTOR(float x, float y, float z) : x(x), y(y), z(z) {}
};

static inline G_VECTOR operator +(G_VECTOR u, G_VECTOR v)
{
	return G_VECTOR(u.x + v.x, u.y + v.y, u.z + v.z);
}

static inline G_VECTOR operator -(G_VECTOR u, G_VECTOR v)
{
	return G_VECTOR(u.x - v.x, u.y - v.y, u.z - v.z);
}

static inline G_VECTOR operator *(G_REAL a, G_VECTOR u)
{
	return G_VECTOR(a*u.x, a*u.y, a*u.z);
}

#else

typedef struct { G_REAL x, y, z; } G_VECTOR;

#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif
static inline G_COLOR g_color_core(float r, float g, float b, float a)
{
	G_COLOR c = {r, g, b, a};
	return c;
}

static inline G_COLOR g_color(float r, float g, float b)
{
	return g_color_core(r, g, b, 1);
}

typedef G_VECTOR G_POSITION;
typedef G_VECTOR G_DIRECTION;

static inline G_VECTOR g_vector_core(G_REAL x, G_REAL y, G_REAL z)
{
	G_VECTOR v = {x, y, z};
	return v;
}

static inline G_VECTOR g_vector(G_REAL x, G_REAL y, G_REAL z)
{
	return g_vector_core(x, y, z);
}

//static inline G_VECTOR g_vector4(G_REAL x, G_REAL y, G_REAL z, G_REAL w)
//{
//	return g_vector_core(x, y, z, w);
//}

static inline G_VECTOR g_vector3(G_REAL x, G_REAL y, G_REAL z)
{
	return g_vector(x, y, z);
}

static inline G_VECTOR g_vector2(G_REAL x, G_REAL y)
{
	return g_vector(x, y, 0);
}

static inline G_POSITION g_position(G_REAL x, G_REAL y, G_REAL z)
{
	return g_vector_core(x, y, z);
}

static inline G_POSITION g_position3(G_REAL x, G_REAL y, G_REAL z)
{
	return g_position(x, y, z);
}

static inline G_POSITION g_position2(G_REAL x, G_REAL y)
{
	return g_position(x, y, 0);
}

static inline G_DIRECTION g_direction(G_REAL x, G_REAL y, G_REAL z)
{
	return g_vector_core(x, y, z);
}

static inline G_DIRECTION g_direction3(G_REAL x, G_REAL y, G_REAL z)
{
	return g_direction(x, y, z);
}

static inline G_DIRECTION g_direction2(G_REAL x, G_REAL y)
{
	return g_direction(x, y, 0);
}
	
//static inline G_VECTOR_F g_vector4f(float x, float y, float z, float w)
//{
//	G_VECTOR_F v = {x, y, z, w};
//	return v;
//}
//	
//static inline G_VECTOR_F g_vector3f(float x, float y, float z)
//{
//	return g_vector4f(x, y, z, 1);
//}
//	
//static inline G_VECTOR_F g_vector2f(float x, float y)
//{
//	return g_vector4f(x, y, 0, 1);
//}
//	
//static inline G_VECTOR_F g_vectorf(G_VECTOR v)
//{
//	return g_vector4f(v.x, v.y, v.z, v.w);
//}

#define EPS_helper(g_real) (sizeof(g_real) == sizeof(float) ? 1e-6 : 1e-14)
#define EPS EPS_helper(G_REAL)

static inline G_VECTOR g_plus (G_VECTOR u,G_VECTOR v)
{
	//assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
	return g_vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}

static inline G_VECTOR g_minus (G_VECTOR u,G_VECTOR v)
{
	//assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
	return g_vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}

static inline G_VECTOR g_multi (G_REAL a, G_VECTOR u)
{
	return g_vector3(a*u.x, a*u.y, a*u.z);
}

static inline G_VECTOR g_cross (G_VECTOR u,G_VECTOR v)
{
	//assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
	return g_vector3(
			u.y*v.z - v.y*u.z,
			u.z*v.x - v.z*u.x,
			u.x*v.y - v.x*u.y);
}

static inline G_REAL g_dot (G_VECTOR u, G_VECTOR v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

static inline G_REAL g_norm(G_VECTOR u)
{
	return sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
}

static inline G_VECTOR g_normalize(G_VECTOR u)
{
	G_REAL s = 1 / g_norm(u);
	return g_vector3(u.x*s, u.y*s, u.z*s);
}
	
static inline G_VECTOR g_calc_normal(G_POSITION u, G_POSITION v, G_POSITION w)
{
	return g_normalize(g_cross(g_minus(v, u), g_minus(w, u)));
}

static const G_VECTOR g_vector_zero = {0, 0, 0};
static const G_VECTOR g_vector_unit_x = {1, 0, 0}, g_vector_unit_y = {0, 1, 0}, g_vector_unit_z = {0, 0, 1};

#ifdef __cplusplus

struct G_VECTOR4
{
	float x, y, z, w;

	G_VECTOR4() = default;
	G_VECTOR4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	//explicit G_VECTOR4(G_VECTOR v) : x(v.x), y(v.y), z(v.z), w(1) {}
};

struct G_MATRIX
{
	G_VECTOR4 x, y, z, w;

	G_MATRIX() = default;
	G_MATRIX(
		float xx, float xy, float xz, float xw,
		float yx, float yy, float yz, float yw,
		float zx, float zy, float zz, float zw,
		float wx, float wy, float wz, float ww
	) :
		x(xx, xy, xz, xw),
		y(yx, yy, yz, yw),
		z(zx, zy, zz, zw),
		w(wx, wy, wz, ww) { };

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
			c, 0, 0, s,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-s, 0, 0, c
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

	static G_MATRIX Ortho(float left, float right, float bottom, float top, float near, float far)
	{
		float a = 1 / (right - left);
		float b = 1 / (top - bottom);
		float c = 1 / (far - near);

		return G_MATRIX(
			2*a, 0, 0, 0,
			0, 2*b, 0, 0,
			0, 0, -2*c, 0,
			-(right+left)*a, -(top+bottom)*b, -(far+near)*c, 1
		);
	}

	static G_MATRIX Perspective(float fov, float aspect, float near, float far)
	{
		float c = 1 / tan(fov / 2);

		return G_MATRIX(
			c / aspect, 0, 0, 0,
			0, c, 0, 0,
			0, 0, (near + far) / (near - far), -1,
			0, 0, 2*near*far / (near - far), 0
		);
	}

	static G_MATRIX LookAt(G_VECTOR eye, G_VECTOR center, G_VECTOR up)
	{
		G_VECTOR Z = g_normalize(g_minus(eye, center));
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

//static inline G_MATRIX g_mult_matrix(G_MATRIX A, G_MATRIX B)
//{
//
//}

//static inline G_VECTOR g_transform(G_MATRIX m, G_VECTOR v)
//{
//	return g_vector(
//		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0],
//		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1],
//		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2]
//	);
//}

#else

typedef struct { float x, y, z, w; } G_VECTOR4;
typedef struct { G_VECTOR4 x, y, z, w; } G_MATRIX;

#endif // __cplusplus

#ifdef __cplusplus
}
#endif

#endif
