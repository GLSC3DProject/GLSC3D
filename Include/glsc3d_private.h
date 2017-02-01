#ifndef GLSC3D_STRUCT_H
#define GLSC3D_STRUCT_H

#include "glsc3d.h"
#include <GL/freeglut.h>

#include <math.h>
#include <float.h>
#include <stdlib.h>

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
#define G_REAL double
	//#else
	//#define G_REAL float
	//#endif

	typedef struct { float r, g, b, a; } G_COLOR;

	//3D homogeneous coordinate
	typedef struct { float x, y, z, w; } G_VECTOR_F;
	typedef struct { G_REAL x, y, z, w; } G_VECTOR;

	typedef G_VECTOR G_POSITION;
	typedef G_VECTOR G_DIRECTION;

	static inline G_COLOR g_color_core(double r, double g, double b, double a)
	{
		G_COLOR c = {r, g, b, a};
		return c;
	}

	static inline G_COLOR g_color(double r, double g, double b)
	{
		return g_color_core(r, g, b, 1);
	}

	static inline G_VECTOR g_vector_core(G_REAL x, G_REAL y, G_REAL z, G_REAL w)
	{
		G_VECTOR v = {x, y, z, w};
		return v;
	}

	static inline G_VECTOR g_vector(G_REAL x, G_REAL y, G_REAL z)
	{
		return g_vector_core(x, y, z, 1);
	}

	static inline G_VECTOR g_vector4(G_REAL x, G_REAL y, G_REAL z, G_REAL w)
	{
		return g_vector_core(x, y, z, w);
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
		return g_vector_core(x, y, z, 1);
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
		return g_vector_core(x, y, z, 0);
	}

	static inline G_DIRECTION g_direction3(G_REAL x, G_REAL y, G_REAL z)
	{
		return g_direction(x, y, z);
	}

	static inline G_DIRECTION g_direction2(G_REAL x, G_REAL y)
	{
		return g_direction(x, y, 0);
	}
	
	static inline G_VECTOR_F g_vector4f(float x, float y, float z, float w)
	{
		G_VECTOR_F v = {x, y, z, w};
		return v;
	}
	
	static inline G_VECTOR_F g_vector3f(float x, float y, float z)
	{
		return g_vector4f(x, y, z, 1);
	}
	
	static inline G_VECTOR_F g_vector2f(float x, float y)
	{
		return g_vector4f(x, y, 0, 1);
	}

#define EPS_helper(g_real) (sizeof(g_real) == sizeof(float) ? 1e-6 : 1e-14)
#define EPS EPS_helper(G_REAL)

	static inline G_VECTOR g_plus (G_VECTOR u,G_VECTOR v)
	{
		assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
		return g_vector4(u.x + v.x, u.y + v.y, u.z + v.z, u.w);
	}

	static inline G_VECTOR g_minus (G_VECTOR u,G_VECTOR v)
	{
		assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
		return g_vector4(u.x - v.x, u.y - v.y, u.z - v.z, u.w);
	}

	static inline G_VECTOR g_multi (G_REAL a, G_VECTOR u)
	{
		return g_vector4(a*u.x, a*u.y, a*u.z, u.w);
	}

	static inline G_VECTOR g_cross (G_VECTOR u,G_VECTOR v)
	{
		assert(fabs(u.w - v.w) <= EPS*fabs(u.w) || fabs(u.w - v.w) <= EPS*fabs(v.w));
		return g_vector4(
				u.y*v.z - v.y*u.z,
				u.z*v.x - v.z*u.x,
				u.x*v.y - v.x*u.y, u.w);
	}

	static inline G_REAL g_dot (G_VECTOR u, G_VECTOR v)
	{
		return u.x*v.x + u.y*v.y + u.z*v.z;
	}

	static inline G_REAL g_norm(G_VECTOR u)
	{
		return sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
	}

	static inline G_VECTOR g_linearize(G_VECTOR u)
	{
		return g_vector4(u.x/u.w, u.y/u.w, u.z/u.w, 1);
	}	

	static inline G_VECTOR g_normrize(G_VECTOR u)
	{
		return g_vector4(u.x/g_norm(u), u.y/g_norm(u), u.z/g_norm(u), u.w);
	}

	static const G_VECTOR    G_ZERO = {0.0, 0.0, 0.0, 1};

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

	typedef struct
	{
		G_COLOR ambient, diffuse, specular, emission;
		float shininess;
	}G_MATERIAL;

	static inline G_MATERIAL g_make_material_core(G_COLOR ambient, G_COLOR diffuse, G_COLOR specular, G_COLOR emission, float shininess)
	{
		G_MATERIAL mate = {ambient, diffuse, specular, emission, shininess};
		return mate;
	}

	static inline G_MATERIAL g_make_material(G_COLOR ambient_and_diffuse)
	{
		float par = 0.01;
		G_MATERIAL mat = {{ambient_and_diffuse.r*par, ambient_and_diffuse.g*par, ambient_and_diffuse.b*par, ambient_and_diffuse.a}, ambient_and_diffuse, {1, 1, 1, 1}, {0, 0, 0, 1}, 127};
		return mat;
	}

	typedef struct
	{
		G_POSITION  position;
		G_DIRECTION normal;
		G_MATERIAL  front, back;
	}G_VERTEX;

	typedef struct
	{
		G_VERTEX vertex[3];
	}G_TRIANGLE;

	static inline G_VERTEX g_color_vertex_core(G_POSITION p, G_DIRECTION n, G_COLOR c_front,  G_COLOR c_back)
	{
		G_VERTEX v = {p, n, g_make_material(c_front), g_make_material(c_back)};
		return v;
	}

	static inline G_VERTEX g_color_vertex(G_POSITION p, G_DIRECTION n, G_COLOR c)
	{
		G_VERTEX v = {p, n, g_make_material(c), g_make_material(c)};
		return v;
	}

	static inline G_VERTEX g_material_vertex_core(G_POSITION p, G_DIRECTION n, G_MATERIAL m_front, G_MATERIAL m_back)
	{
		G_VERTEX v = {p, n, m_front, m_back};
		return v;
	}

	static inline G_VERTEX g_material_vertex(G_POSITION p, G_DIRECTION n, G_MATERIAL m)
	{
		G_VERTEX v = {p, n, m, m};
		return v;
	}

	static inline G_TRIANGLE g_make_triangle_core(G_VERTEX v0, G_VERTEX v1, G_VERTEX v2)
	{
		G_TRIANGLE t = {{v0, v1, v2}};
		return t;
	}

	static inline G_TRIANGLE g_make_triangle(G_POSITION p0, G_POSITION p1, G_POSITION p2, G_COLOR c)
	{
		G_DIRECTION n = g_cross(g_minus(p1, p0), g_minus(p2, p0));
		n.w = 0;
		G_MATERIAL m = g_make_material(c);

		G_TRIANGLE t = {{{p0, n, m, m}, {p1, n, m, m}, {p2, n, m, m}}};
		return t;
	}


	extern G_CAMERA glsc3D_inner_camera[TotalDisplayNumber];
	extern G_SCREEN glsc3D_inner_screen[TotalDisplayNumber];

	extern G_COLOR current_area_color_3D, current_area_color_2D, current_line_color, current_text_color, current_marker_color;

	//extern int color2D_id, color3D_id;
	
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


	typedef struct
	{
		unsigned int light_id;
		G_COLOR ambient, diffuse, specular;
		G_POSITION position;
		G_DIRECTION direction;
		float exponent, cutoff, constant_attenuation, linear_attenuation, quadratic_attenuation;
	}G_LIGHT;

	G_LIGHT g_make_light_core(GLenum light_id, G_COLOR ambient, G_COLOR diffuse, G_COLOR specular, G_POSITION position, G_POSITION direction, float exponent, float cutoff, float constant_attenuation, float linear_attenuation, float quadratic_attenuation);

	G_LIGHT g_make_light(GLenum light_id, G_COLOR ambient_and_diffuse, G_COLOR specular, G_POSITION position);

	G_LIGHT g_make_light_easy(GLenum light_id, G_POSITION position);

	void g_set_light(G_LIGHT light);
	void g_remove_light(G_LIGHT light);

	void g_move_s(G_VECTOR u);
	void g_plot_s(G_VECTOR u);

	void g_input_init(void);
	void g_get_input(void);

	DELETE_ON_CXX(double g_max(int N_x, int N_z, double data[N_x][N_z]);)
#define g_max(i,j,k)     (g_max((i),(j),(double(*)[(j)])(k)))

	DELETE_ON_CXX(double g_min(int N_x, int N_z, double data[N_x][N_z]);)
#define g_min(i,j,k)     (g_min((i),(j),(double(*)[(j)])(k)))

	void g_set_material(GLenum face, G_MATERIAL mat);
	void g_set_triangle(G_TRIANGLE t);
	void g_triangle_terminal(G_TRIANGLE t);
	
	// ------ g_vertex_buffer.c ------
	
	void g_vertex_buffer_init();
	void g_vertex_buffer_append(G_VECTOR_F vertex);
	void g_vertex_buffer_append_line(G_VECTOR_F a, G_VECTOR_F b);
	void g_vertex_buffer_append_triangle(G_VECTOR_F a, G_VECTOR_F b, G_VECTOR_F c);

	void g_vertex_buffer_draw();
//	void g_set_primitive_mode(GLenum mode);
	
	void g_begin_lines();
	void g_begin_triangles();

#ifdef __cplusplus
}
#endif

#endif
