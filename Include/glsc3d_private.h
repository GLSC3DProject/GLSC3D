#ifndef GLSC3D_STRUCT_H
#define GLSC3D_STRUCT_H

#include "glsc3d.h"
#include <GL/freeglut.h>

#include <math.h>
#include <float.h>
#include <stdio.h>

#define TotalDisplayNumber (100)

#define PI M_PI


#ifdef __cplusplus
#include <cassert>
#else
#include <assert.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	//#ifdef G_USE_DOUBLE_PRECISION_VECTOR
	//#define G_REAL double
	//#else
	//#define G_REAL float
	//#endif
	typedef float G_REAL;

	typedef struct { float r, g, b, a; } G_COLOR;

	//3D homogeneous coordinate
	typedef struct { G_REAL x, y, z; } G_VECTOR;

	typedef G_VECTOR G_POSITION;
	typedef G_VECTOR G_DIRECTION;

	static inline G_COLOR g_color_core(float r, float g, float b, float a)
	{
		G_COLOR c = {r, g, b, a};
		return c;
	}

	static inline G_COLOR g_color(float r, float g, float b)
	{
		return g_color_core(r, g, b, 1);
	}

	static inline G_VECTOR g_vector_core(G_REAL x, G_REAL y, G_REAL z)
	{
		G_VECTOR v = {x, y, z};
		return v;
	}

	static inline G_VECTOR g_vector(G_REAL x, G_REAL y, G_REAL z)
	{
		return g_vector_core(x, y, z);
	}

//	static inline G_VECTOR g_vector4(G_REAL x, G_REAL y, G_REAL z, G_REAL w)
//	{
//		return g_vector_core(x, y, z, w);
//	}

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
	
//	static inline G_VECTOR_F g_vector4f(float x, float y, float z, float w)
//	{
//		G_VECTOR_F v = {x, y, z, w};
//		return v;
//	}
//	
//	static inline G_VECTOR_F g_vector3f(float x, float y, float z)
//	{
//		return g_vector4f(x, y, z, 1);
//	}
//	
//	static inline G_VECTOR_F g_vector2f(float x, float y)
//	{
//		return g_vector4f(x, y, 0, 1);
//	}
//	
//	static inline G_VECTOR_F g_vectorf(G_VECTOR v)
//	{
//		return g_vector4f(v.x, v.y, v.z, v.w);
//	}

#define EPS_helper(g_real) (sizeof(g_real) == sizeof(float) ? 1e-6 : 1e-14)
#define EPS EPS_helper(G_REAL)

	static inline G_VECTOR g_plus (G_VECTOR u,G_VECTOR v)
	{
		assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
		return g_vector3(u.x + v.x, u.y + v.y, u.z + v.z);
	}

	static inline G_VECTOR g_minus (G_VECTOR u,G_VECTOR v)
	{
		assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
		return g_vector3(u.x - v.x, u.y - v.y, u.z - v.z);
	}

	static inline G_VECTOR g_multi (G_REAL a, G_VECTOR u)
	{
		return g_vector3(a*u.x, a*u.y, a*u.z);
	}

	static inline G_VECTOR g_cross (G_VECTOR u,G_VECTOR v)
	{
		assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
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

	static const G_VECTOR g_vector_zero = {0.0, 0.0, 0.0};

	G_REAL g_direction_phi(G_VECTOR v);
	G_REAL g_direction_theta(G_VECTOR v);

	void g_polar(G_VECTOR v, G_REAL *phi, G_REAL *theta);

	G_VECTOR Rn(G_VECTOR u,G_VECTOR n,G_REAL theta);
	G_VECTOR Rx(G_VECTOR u,G_REAL theta);
	G_VECTOR Ry(G_VECTOR u,G_REAL theta);
	G_VECTOR Rz(G_VECTOR u,G_REAL theta);

	G_VECTOR Heiko3D(G_VECTOR u,G_REAL x,G_REAL y,G_REAL z);
	G_VECTOR Heiko3Ds(G_VECTOR u,G_VECTOR r);

	G_VECTOR Scaling3D(G_VECTOR u,G_REAL x,G_REAL y,G_REAL z);
	G_VECTOR Scaling3Ds(G_VECTOR u,G_VECTOR s);

	G_VECTOR Rx2D(G_VECTOR u,G_REAL theta);
	
	extern G_CAMERA glsc3D_inner_camera[TotalDisplayNumber];
	extern G_SCREEN glsc3D_inner_screen[TotalDisplayNumber];
	
	extern G_COLOR g_current_area_color_3D, g_current_area_color_2D;
	extern G_COLOR g_current_line_color, g_current_text_color, g_current_marker_color;
//	extern G_COLOR current_color;
	
	extern G_BOOL g_lighting_enabled;

	typedef struct
	{
		G_VECTOR position;
		G_VECTOR normal;
		G_COLOR  color;
	}G_VERTEX;

	typedef struct
	{
		G_VERTEX vertex[3];
	}G_TRIANGLE;

	static inline G_VERTEX g_make_vertex(G_POSITION position, G_VECTOR normal, G_COLOR color)
	{
		G_VERTEX v = {position, normal, color};
		return v;
	}
	
//	static inline G_VERTEX g_make_vertex_normal(G_POSITION position, G_DIRECTION normal)
//	{
//		G_VERTEX v = {position, normal, current_color};
//		return v;
//	}

	static inline G_TRIANGLE g_make_triangle_core(G_VERTEX v0, G_VERTEX v1, G_VERTEX v2)
	{
		G_TRIANGLE t = {{v0, v1, v2}};
		return t;
	}

//	static inline G_TRIANGLE g_make_triangle_3D(G_POSITION p0, G_POSITION p1, G_POSITION p2)
//	{
//		G_COLOR c = current_area_color_3D;
//		
//		G_DIRECTION n = g_cross(g_minus(p1, p0), g_minus(p2, p0));
////		n.w = 0;
//
//		G_TRIANGLE t = {{{p0, n, c}, {p1, n, c}, {p2, n, c}}};
//		return t;
//	}
	
	void g_text_init();
	

	extern int             glsc3D_width;
	extern int             glsc3D_height;

	extern int             get_scale_id_number;
	extern G_DIMENSION     g_scale_dim_flag;

	void g_triangle_buffer_init();
	void g_triangle_buffer_append(G_TRIANGLE t);
	void g_triangle_buffer_draw();
	void g_triangle_buffer_flush();

	void g_line_loop(void);
	void g_line_strip(void);
	void g_lines(void);
	void g_polygon(void);
	void g_triangles(void);
	void g_triangle_strip(void);
	void g_triangle_fan(void);
	void g_quads(void);
	void g_quad_strip(void);
	void g_points(void);

	void glNormals(G_VECTOR u);
	void glVertexs(G_VECTOR u);

	void g_move_s(G_VECTOR u);
	void g_plot_s(G_VECTOR u);

	void g_input_init(void);
	void g_get_input(void);

	DELETE_ON_CXX(double g_max(int N_x, int N_z, double data[N_x][N_z]);)
#define g_max(i,j,k)     (g_max((i),(j),(double(*)[(j)])(k)))

	DELETE_ON_CXX(double g_min(int N_x, int N_z, double data[N_x][N_z]);)
#define g_min(i,j,k)     (g_min((i),(j),(double(*)[(j)])(k)))

	// ---- g_triangle.c
	
	void g_triangle_3D_core_worker(G_POSITION r0, G_POSITION r1, G_POSITION r2, int DivideLevel);

//	void g_set_material(GLenum face, G_MATERIAL mat);
	void g_set_triangle(G_TRIANGLE t);
	void g_triangle_terminal(G_TRIANGLE t);
	
	// ---- g_vertex_buffer.c
	
	void g_vertex_buffer_init();
	void g_vertex_buffer_append(G_VERTEX vertex);
//	void g_vertex_buffer_append_normal(G_VECTOR_F position, G_VECTOR_F normal);

	void g_vertex_buffer_append_line(G_VECTOR a, G_VECTOR b);
	void g_vertex_buffer_append_triangle_2D(G_VECTOR a, G_VECTOR b, G_VECTOR c);

//	void g_vertex_buffer_append_triangle_normal(G_VECTOR_F a, G_VECTOR_F b, G_VECTOR_F c, G_VECTOR_F n);

	void g_vertex_buffer_flush();
//	void g_set_primitive_mode(GLenum mode);
	
	void g_begin_lines();
	void g_begin_triangles();

#ifdef __cplusplus
}
#endif

#endif
