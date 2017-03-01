#ifndef GLSC3D_PRIVATE_H
#define GLSC3D_PRIVATE_H

#undef NDEBUG

#include "glsc3d.h"
#include "glsc3d_math.h"

#ifdef __cplusplus
#include <cassert>
#else
#include <assert.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glext.h>
#undef near
#undef far
#endif

#define TotalDisplayNumber (100)

#define PI M_PI
//#define DEGREES (PI / 180)

#ifdef __cplusplus
extern "C"
{
#endif

	G_REAL g_direction_phi(G_VECTOR v);
	G_REAL g_direction_theta(G_VECTOR v);

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
		int x, y, width, height;
	}G_SCREEN;

	typedef struct
	{
		G_MATRIX proj, view;
		G_VECTOR eye;
		float _pad;
	}G_CAMERA;

	typedef enum
	{
		G_EYE,
		G_CENTER,
	}G_ROTATE_ANCHOR;

	typedef enum
	{
		G_PITCH,
		G_ROLL,
		G_YAW,
	}G_ROTATE_DIRECTION;

	G_SCREEN g_make_screen(int x, int y, int width, int height);
	//G_CAMERA g_make_camera_core(float fovy, float near, float far, float eyeX, float  eyeY, float  eyeZ, float  centerX, float  centerY, float  centerZ, float  upX, float  upY, float  upZ);
	//G_CAMERA g_make_camera(float eyeX, float  eyeY, float  eyeZ, float  centerX, float  centerY, float  centerZ);
	//G_CAMERA g_make_camera_2D(float X, float  Y);
	G_CAMERA g_make_camera_3D_core(G_VECTOR lower, G_VECTOR upper, G_VECTOR direction, float r, float aspect, G_VECTOR up);
	//G_CAMERA g_make_camera_3D(float x_left, float x_right, float y_bottom, float y_top, float z_near, float z_far, float direction_x, float direction_y, float direction_z, float r, float aspect);
	G_CAMERA g_make_camera_2D(float x_left, float x_right, float y_bottom, float y_top);
	//void g_camera_rotate(G_CAMERA *cam, G_ROTATE_DIRECTION dire, G_ROTATE_ANCHOR anc, float theta);

	extern G_CAMERA glsc3D_inner_camera[TotalDisplayNumber];
	extern G_SCREEN glsc3D_inner_screen[TotalDisplayNumber];
	
	extern G_COLOR g_current_area_color_3D, g_current_area_color_2D;
	extern G_COLOR g_current_line_color, g_current_text_color, g_current_marker_color;
	extern G_COLOR g_current_color;
	
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

	static inline G_TRIANGLE g_make_triangle_core(G_VERTEX v0, G_VERTEX v1, G_VERTEX v2)
	{
		G_TRIANGLE t = {{v0, v1, v2}};
		return t;
	}
	
	void g_text_init();

	extern int				glsc3D_width;
	extern int				glsc3D_height;

	extern int				get_scale_id_number;
	extern G_DIMENSION		g_scale_dim_flag;

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

	// ---- g_triangle.c
	
	void g_triangle_3D_core_worker(G_POSITION r0, G_POSITION r1, G_POSITION r2, int DivideLevel);

//	void g_set_material(GLenum face, G_MATERIAL mat);
	void g_set_triangle(G_TRIANGLE t);
	void g_triangle_terminal(G_TRIANGLE t);
	
	// ---- g_vertex_buffer.c
	
	void g_vertex_buffer_init();
	void g_vertex_buffer_append(G_VERTEX vertex);
	void g_vertex_buffer_append_position(G_VECTOR position);

	void g_vertex_buffer_append_line(G_VECTOR a, G_VECTOR b);
	void g_vertex_buffer_append_triangle_2D(G_VECTOR a, G_VECTOR b, G_VECTOR c);

	void g_vertex_buffer_flush();

	void g_begin_points();
	void g_begin_lines();
	void g_begin_triangles();
	
	void g_check_opengl_error(int checkpoint);

	// ---- g_shader_program.c

	void g_shader_program_init();

	void g_enable_lighting();
	void g_disable_lighting();

	// ---- g_sdl_wrapper.c

	void g_sdl_init(const char *WindowName, int pos_x, int pos_y, int width, int height);
	void g_swap_buffers();
	void g_poll_events();
	void g_quit();

#ifdef __cplusplus
}
#endif

#endif
