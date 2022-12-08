#ifndef GLSC3D_MATH_H
#define GLSC3D_MATH_H

#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif

typedef double G_REAL;

#ifdef __cplusplus

struct G_COLOR
{
	float r, g, b, a;

	G_COLOR() {}
	G_COLOR(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

struct G_VECTOR
{
	float x, y, z;

	G_VECTOR() {}
	G_VECTOR(float x, float y, float z = 0) : x(x), y(y), z(z) {}

	void operator +=(G_VECTOR v) { x += v.x, y += v.y, z += v.z; }
	void operator -=(G_VECTOR v) { x -= v.x, y -= v.y, z -= v.z; }
	void operator *=(float s) { x *= s, y *= s, z *= s; }
	void operator /=(float s) { (*this) *= 1 / s; }

	G_VECTOR operator +() { return *this; }
	G_VECTOR operator -() { return G_VECTOR(-x, -y, -z); }
};

inline G_VECTOR operator +(G_VECTOR u, G_VECTOR v) { u += v; return u; }
inline G_VECTOR operator -(G_VECTOR u, G_VECTOR v) { u -= v; return u; }

inline G_VECTOR operator *(float a, G_VECTOR u) { u *= a; return u; }
inline G_VECTOR operator *(G_VECTOR u, float a) { u *= a; return u; }
inline G_VECTOR operator /(G_VECTOR u, float a) { u /= a; return u; }

#else

typedef struct { float r, g, b, a; } G_COLOR;
typedef struct { float x, y, z; } G_VECTOR;

#endif // __cplusplus

typedef G_VECTOR G_POSITION;
typedef G_VECTOR G_DIRECTION;

static inline G_VECTOR g_vector(G_REAL x, G_REAL y, G_REAL z)
{
	G_VECTOR v;
	v.x = (float)x;
	v.y = (float)y;
	v.z = (float)z;
	return v;
}

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
	return g_vector(x, y, z);
}

static inline G_VECTOR g_plus (G_VECTOR u,G_VECTOR v)
{
	return g_vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}

static inline G_VECTOR g_minus (G_VECTOR u,G_VECTOR v)
{
	return g_vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}

static inline G_VECTOR g_multi (G_REAL a, G_VECTOR u)
{
	return g_vector3(a*u.x, a*u.y, a*u.z);
}

static inline G_VECTOR g_cross (G_VECTOR u,G_VECTOR v)
{
	return g_vector3(
		u.y*v.z - v.y*u.z,
		u.z*v.x - v.z*u.x,
		u.x*v.y - v.x*u.y);
}

static inline float g_dot (G_VECTOR u, G_VECTOR v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

static inline float g_norm(G_VECTOR u)
{
	return sqrtf(u.x*u.x + u.y*u.y + u.z*u.z);
}

static inline G_VECTOR g_normalize(G_VECTOR u)
{
	float s = 1 / g_norm(u);
	return g_vector3(u.x*s, u.y*s, u.z*s);
}

static inline G_VECTOR g_calc_normal(G_POSITION a, G_POSITION b, G_POSITION c)
{
	return g_normalize(g_cross(g_minus(b, a), g_minus(c, a)));
}

#endif
