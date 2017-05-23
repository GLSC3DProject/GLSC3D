#ifndef GLSC3D_PRIVATE_H
#define GLSC3D_PRIVATE_H

#include "glsc3d_3.h"

#ifdef __cplusplus
#include <cassert>
#else
#include <assert.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glext.h>
#undef near
#undef far
#endif

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#endif

#define TotalDisplayNumber (128)

#define PI M_PI

#ifdef __cplusplus
extern "C"
{
#endif

// On Windows and Linux, it is required to get address of OpenGL 1.2+ functions.
#ifndef __APPLE__

#define G_EXTERN_DECL_GLEXT(Type, Name) extern Type Name;

#ifdef _WIN32
#define G_DECL_INIT_GLEXT(Type, Name) Type Name = (Type)wglGetProcAddress(#Name)
#else
#define G_DECL_INIT_GLEXT(Type, Name) Type Name = (Type)glXGetProcAddress((const GLubyte *)#Name)
#endif

#define G_EMIT_GLEXT(Action) \
Action(PFNGLVERTEXATTRIBPOINTERPROC,        glVertexAttribPointer) \
Action(PFNGLENABLEVERTEXATTRIBARRAYPROC,    glEnableVertexAttribArray) \
Action(PFNGLGENVERTEXARRAYSPROC,            glGenVertexArrays) \
Action(PFNGLBINDVERTEXARRAYPROC,            glBindVertexArray) \
Action(PFNGLGENBUFFERSPROC,                 glGenBuffers) \
Action(PFNGLBUFFERDATAPROC,                 glBufferData) \
Action(PFNGLBUFFERSUBDATAPROC,              glBufferSubData) \
Action(PFNGLBINDBUFFERPROC,                 glBindBuffer) \
Action(PFNGLCREATEPROGRAMPROC,              glCreateProgram) \
Action(PFNGLLINKPROGRAMPROC,                glLinkProgram) \
Action(PFNGLUSEPROGRAMPROC,                 glUseProgram) \
Action(PFNGLCREATESHADERPROC,               glCreateShader) \
Action(PFNGLSHADERSOURCEPROC,               glShaderSource) \
Action(PFNGLCOMPILESHADERPROC,              glCompileShader) \
Action(PFNGLATTACHSHADERPROC,               glAttachShader) \
Action(PFNGLGETPROGRAMIVPROC,               glGetProgramiv) \
Action(PFNGLGETSHADERIVPROC,                glGetShaderiv) \
Action(PFNGLGETPROGRAMINFOLOGPROC,          glGetProgramInfoLog) \
Action(PFNGLGETSHADERINFOLOGPROC,           glGetShaderInfoLog) \
Action(PFNGLGETUNIFORMBLOCKINDEXPROC,       glGetUniformBlockIndex) \
Action(PFNGLUNIFORMBLOCKBINDINGPROC,        glUniformBlockBinding) \
Action(PFNGLGETUNIFORMLOCATIONPROC,         glGetUniformLocation) \
Action(PFNGLUNIFORM1IPROC,                  glUniform1i) \
Action(PFNGLUNIFORM4FVPROC,                 glUniform4fv) \
Action(PFNGLGENSAMPLERSPROC,                glGenSamplers) \
Action(PFNGLBINDSAMPLERPROC,                glBindSampler) \
Action(PFNGLSAMPLERPARAMETERIPROC,          glSamplerParameteri) \
Action(PFNGLBINDBUFFERBASEPROC,             glBindBufferBase)

G_EMIT_GLEXT(G_EXTERN_DECL_GLEXT);

#endif // ifndef __APPLE__

G_REAL g_direction_phi(G_VECTOR v);
G_REAL g_direction_theta(G_VECTOR v);

G_VECTOR Rn(G_VECTOR u,G_VECTOR n,G_REAL theta);
G_VECTOR Rx(G_VECTOR u,G_REAL theta);
G_VECTOR Ry(G_VECTOR u,G_REAL theta);
G_VECTOR Rz(G_VECTOR u,G_REAL theta);

G_VECTOR Scaling3Ds(G_VECTOR u,G_VECTOR s);

G_VECTOR Rx2D(G_VECTOR u,G_REAL theta);

#ifdef __cplusplus

struct G_SCREEN
{
	int x, y, width, height;
};

struct G_CAMERA
{
	G_MATRIX proj, view;
};

struct G_SCALE
{
	G_SCREEN screen;
	G_CAMERA camera;

	void select();
};

// ---- g_camera.cpp

G_SCREEN g_make_screen(int x, int y, int width, int height);
G_CAMERA g_make_camera_3D_core(G_VECTOR lower, G_VECTOR upper, G_VECTOR direction, float r, float aspect, G_VECTOR up);
G_CAMERA g_make_camera_2D(float x_left, float x_right, float y_bottom, float y_top);
//void g_camera_rotate(G_CAMERA *cam, G_ROTATE_DIRECTION dire, G_ROTATE_ANCHOR anc, float theta);

// ---- g_screen.cpp

extern G_SCALE glsc3D_inner_scale[TotalDisplayNumber];

#endif

typedef struct
{
	G_VECTOR	position;
	float		size;
	G_COLOR		color;
	G_VECTOR	normal;
	float		pad;	// for 16-byte alignment
} G_VERTEX;

typedef struct
{
	G_VERTEX vertex[3];
} G_TRIANGLE;

// ---- g_area.cpp

extern G_COLOR g_current_area_color_3D, g_current_area_color_2D;

static inline G_VERTEX g_make_vertex(G_POSITION position, G_VECTOR normal)
{
	G_VERTEX v = {position, 0, g_current_area_color_3D, normal, 0};
	return v;
}

static inline G_TRIANGLE g_make_triangle_core(G_VERTEX v0, G_VERTEX v1, G_VERTEX v2)
{
	G_TRIANGLE t = {{v0, v1, v2}};
	return t;
}

// ---- g_init.cpp

extern int				glsc3D_width;
extern int				glsc3D_height;
extern float			g_screen_scale_factor;
extern float			g_retina_scale_factor;

extern int g_enable_transparent;
extern int TRIANGLE_BUFFER_SIZE;
extern int TEMPORARY_TRIANGLE_BUFFER_SIZE;

// ---- g_array_buffer.c

extern void * GLSC3D_Data_Buffer;
void * GLSC3D_Array_Buffer(size_t array_size);

// ---- g_scale.cpp

extern int				g_current_scale_id;
extern G_DIMENSION		g_scale_dim_flag;

// ---- g_input.c

void g_keyboard_func(G_KEY_CODE key, G_INPUT_STATE state);
void g_mouse_func(G_KEY_CODE button, G_INPUT_STATE state, int x, int y);

void update_input_key_state(void);

// ---- g_line.cpp

#ifdef __cplusplus

extern G_COLOR	g_current_line_color;
extern float	g_current_line_size;
extern int		g_current_line_stipple;
//extern bool		g_need_line_stipple_updated;

#endif

// ---- g_marker.cpp

extern G_COLOR	g_current_marker_color;
extern float	g_current_marker_size;
extern G_UINT	g_current_marker_type;
extern G_UINT	g_current_marker_size_type;

// ---- g_off_screen.c

void g_init_off_screen_rendering();

// ---- g_text.cpp

extern G_COLOR g_current_text_color;

void g_text_init();

// ---- g_triangle_buffer.cpp

void g_triangle_buffer_init();
void g_triangle_buffer_append(const G_TRIANGLE *t);
void g_triangle_buffer_draw();
void g_triangle_buffer_flush();

// ---- g_triangle.c

void g_triangle_3D_smooth_worker(G_VECTOR r0, G_VECTOR r1, G_VECTOR r2, G_VECTOR n0, G_VECTOR n1, G_VECTOR n2, int DivideLevel);
void g_triangle_3D_flat_worker(G_VECTOR r0, G_VECTOR r1, G_VECTOR r2, int DivideLevel);
void g_triangle_3D_wire_worker(G_VECTOR r0, G_VECTOR r1, G_VECTOR r2);

void g_set_triangle(const G_TRIANGLE *t);
void g_triangle_terminal(const G_TRIANGLE *t);

// ---- g_sdl_wrapper.cpp

void g_sdl_init(const char *WindowName, int pos_x, int pos_y, int width, int height);
void g_swap_buffers(void);
void g_poll_events(void);
void g_quit(void);

// ---- g_shader_program.cpp

//enum G_UNIFORM_BINDING { G_UNIFORM_MATRICES, G_UNIFORM_LIGHTS, G_NUM_UNIFORMS };
enum G_MARKER_SIZE_TYPE { G_MARKER_SIZE_STANDARD, G_MARKER_SIZE_VIRTUAL, G_NUM_MARKER_SIZE_TYPES };

extern GLuint g_constant_program, g_lighting_program;
extern GLuint g_marker_programs[G_NUM_MARKER_SIZE_TYPES][G_NUM_MARKER_TYPES];
//extern GLuint g_line_program;
extern GLuint g_texture_program;
extern GLuint g_current_program;

//extern GLint g_line_stipple_location;
extern GLint g_texture_sampler_location, g_texture_color_location;

void g_shader_program_init();
//void g_update_uniform(GLuint index, GLsizei size, const void *data);
void g_use_program(GLuint program);

// ---- g_vertex_buffer.c

void g_vertex_buffer_init();
void g_vertex_buffer_append(G_VERTEX vertex);
void g_emit_vertex(G_VECTOR position);

void g_emit_line(G_VECTOR p, G_VECTOR q);
void g_emit_triangle(G_VECTOR p, G_VECTOR q, G_VECTOR r);

void g_vertex_buffer_flush(void);

void g_begin_points(void);
void g_begin_lines(void);
void g_begin_triangles(void);

void g_begin_line_strip(void);
void g_begin_line_loop(void);

void g_begin_triangle_strip(void);
void g_begin_triangle_fan(void);

#ifdef __cplusplus
}
#endif

#endif
