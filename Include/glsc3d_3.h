#ifndef GLSC3D_H
#define GLSC3D_H

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#include "glsc3d_3_math.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned int G_UINT;

/*typedef enum
 {
 G_TRUE  = 1,
 G_FALSE = 0
 } G_BOOL; */

typedef int G_BOOL;
#define G_FALSE	0
#define G_TRUE	1
#define G_NO	0
#define G_YES	1

/*typedef enum
 {
 G_2D   = 0,
 G_3D   = 1,
 } G_DIMENSION; */

#define G_OFF_SCREEN		NULL

#define G_WINDOW_CENTERED	0x2FFF0000

#define DBG_WRITE(...)
//#define DBG_WRITE(...) fprintf(stderr, "%s:%d In function '%s'", __FILE__, __LINE__, __func__), fprintf(stderr, "  "  __VA_ARGS__), fprintf(stderr, "\n")

// ---- g_init.cpp

void g_init_core (
	const char *WindowName, int width, int height,
	int pos_x, int pos_y,
	float r, float g, float b,
	int enable_transparent,
	int temporary_triangle_buffer_size,
	int triangle_buffer_size);

void g_init(const char *WindowName, int width, int height);

void g_enable_highdpi();
void g_set_antialiasing(G_UINT level);

// ----g_area.c

void g_area_color_s(G_COLOR color);

void g_area_color(float r, float g, float b, float a);

void g_def_area(int id, float r, float g, float b, float a);
void g_sel_area(int id);

// ---- g_cls_finish.c

void g_scr_color(float r, float g, float b);
void g_cls();
void g_finish();

void g_sleep(double wait_time);

// ---- g_capture.c

int g_capture();
int g_capture_set(const char *name);

// ---- g_light.cpp

void g_init_light_core(G_UINT lightnum, float x, float y, float z, float power);
void g_init_light(G_UINT lightnum, float x, float y, float z);
void g_disable_light(G_UINT lightnum);

// ---- g_scale.cpp

void g_def_scale_2D(
	int id,
	double x_left, double x_right, double y_bottom, double y_top,
	double x_left_std, double y_top_std,
	double width_std, double height_std);

void g_def_scale_3D_fix(int id,
	double x_0_f, double x_1_f, double y_0_f, double y_1_f, double z_0_f, double z_1_f,
	double x_left_std, double y_top_std, double width_std, double height_std);

void g_def_scale_3D(
	int id,
	double x_0, double x_1, double y_0, double y_1, double z_0, double z_1,
	double x_0_f, double x_1_f, double y_0_f, double y_1_f, double z_0_f, double z_1_f,
	double x_left_std, double y_top_std, double width_std, double height_std
);

void g_vision(
	int id,
	double eye_x, double eye_y, double eye_z,
	double up_x, double up_y, double up_z,
	double zoom
);

void g_sel_scale(int id);

void g_boundary(void);
void g_clipping(G_BOOL value);

// ---- g_move_plot.c

void g_move_s(G_VECTOR position);
void g_move_3D(double x, double y, double z);
void g_move_2D(double x, double y);

void g_plot_s(G_VECTOR position);
void g_plot_3D(double x, double y, double z);
void g_plot_2D(double x, double y);

// ---- g_marker.cpp

enum G_MARKER_TYPE { G_MARKER_SQUARE, G_MARKER_CIRCLE, G_MARKER_SPHERE, G_NUM_MARKER_TYPES };

void g_marker_color_s(G_COLOR color);
void g_marker_color(float r, float g, float b, float a);
void g_marker_size(float size);
void g_marker_radius(float size);
void g_marker_type(G_UINT type);

void g_marker_s(G_VECTOR position);
void g_marker_3D(double x, double y, double z);
void g_marker_2D(double x, double y);

void g_def_marker(int id, float r, float g, float b, float a, G_UINT type, float size);
void g_sel_marker(int id);

// ---- g_line.cpp

void g_line_color_s(G_COLOR color);
void g_line_color(float r, float g, float b, float a);
void g_line_width(float size);
void g_line_type(G_UINT type);

void g_def_line(int id, float r, float g, float b, float a, float width, G_UINT type);
void g_sel_line(int id);

// ---- g_text.cpp

void g_text_standard(double x, double y, const char *format, ...);

void g_text_3D_virtual(double x, double y, double z, const char *format, ...);
void g_text_2D_virtual(double x, double y, const char *format, ...);

void g_text_color_s(G_COLOR color);
void g_text_color(float r, float g, float b, float a);
void g_text_font_core(const char *font_file);
void g_text_size(float size);

void g_def_text_core(int id, float r, float g, float b, float a, const char *font_type, float font_size);
//void g_def_text(int id, float r, float g, float b, float a, int font, float font_size);
void g_def_text(int id, float r, float g, float b, float a, float font_size);
void g_sel_text(int id);

// ----------------

void g_bird_view_f_3D(
	double x0, double x1,   //図を表示したい範囲
	double y0, double y1,   //図を表示したい範囲
	int N_x, int N_y,       //配列のサイズ
	double *data,           //二次元配列
	G_BOOL WIRE, G_BOOL FILL);

void g_contln_f_2D(
	double x_left, double x_right,
	double y_bottom, double y_top,
	int N_x, int N_y,
	double *data2D,
	double level);

void g_isosurface_f_3D(
	double x0,double x1,
	double y0,double y1,
	double z0,double z1,
	int number_x,int number_y,int number_z,
	double *u,
	double height);

void g_data_plot_f_3D(
	double x0, double x1,
	double y0, double y1,
	int N_x, int N_y,
	double *data);

void g_data_plot_2D(double x_left, double x_right, double *yy, int n);

#define g_bird_view_3D(x_left, x_right, y_bottom, y_top, N_x, N_z, data, wire, fill) \
g_bird_view_f_3D(x_left, x_right, y_bottom, y_top, N_x, N_z, &data[0][0], wire, fill)

#define g_contln_2D(x_left, x_right, y_bottom, y_top, N_x, N_y, data2D, level) \
g_contln_f_2D(x_left, x_right, y_bottom, y_top, N_x, N_y, &data2D[0][0], level)

#define g_isosurface_3D(x0, x1, y0, y1, z0, z1, number_x, number_y, number_z, u, height) \
g_isosurface_f_3D(x0, x1, y0, y1, z0, z1, number_x, number_y, number_z, &u[0][0][0], height)

#define g_data_plot_3D(x0, x1, y0, y1, N_x, N_y, data) \
g_data_plot_f_3D(x0, x1, y0, y1, N_x, N_y, &data[0][0])

// ------ g_box.c -----

void g_box_2D(
	double x_left, double x_right,
	double y_bottom, double y_top,
	G_BOOL Wire, G_BOOL Fill);

void g_box_3D_core(
	double x0, double x1,
	double y0, double y1,
	double z0, double z1,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_box_3D(
	double x0, double x1,
	double y0, double y1,
	double z0, double z1,
	G_BOOL Wire, G_BOOL Fill);

void g_box_center_2D(
	double x, double y,
	double width, double height,
	G_BOOL WIRE, G_BOOL FILL);

void g_box_center_3D_core(
	double x, double y, double z,
	double width, double height, double depth,
	int DivideLevel, G_BOOL WIRE, G_BOOL FILL);

void g_box_center_3D(
	double x, double y, double z,
	double width, double height, double depth,
	G_BOOL WIRE, G_BOOL FILL);

void g_sphere_3D_core(double x, double y, double z, double radius, int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_sphere_3D(double x, double y, double z, double radius, G_BOOL Wire, G_BOOL Fill);

void g_ellipse_3D_core(
	double x, double y, double z,
	double Sx, double Sy, double Sz,
	double direction_x, double direction_y, double direction_z,
	int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_ellipse_3D(
	double x, double y, double z,
	double Sx, double Sy, double Sz,
	double direction_x, double direction_y, double direction_z,
	G_BOOL Wire, G_BOOL Fill);

void g_ellipse_3D_Euler(
	double x, double y, double z,
	double Sx, double Sy, double Sz,
	double angle_alpha, double angle_beta, double angle_gamma,
	int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_prism_3D_core(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius, double height, double psi, int N,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_prism_3D(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius, double height, double psi, int N, G_BOOL Wire, G_BOOL Fill);

void g_cylinder_3D_core(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius, double height, double psi,
	int N, int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_cylinder_3D(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius, double height, double psi, G_BOOL Wire, G_BOOL Fill);

void g_cone_3D_core(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius, double head_size,
	int N, int DivideLevel, G_BOOL WIRE, G_BOOL FILL);

void g_cone_3D(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius,double head_size, G_BOOL Wire, G_BOOL Fill);

void g_pyramid_3D_core(
	double center_x, double center_y, double center_z,          //中心座標
	double direction_x, double direction_y, double direction_z, //方向
	double radius,double head_size, double psi,                 //半径、高さ
	int N, int DivideLevel, G_BOOL WIRE, G_BOOL FILL);

void g_pyramid_3D(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius,double head_size, double psi,
	int N, G_BOOL WIRE, G_BOOL FILL);

void g_arrow_2D(
	double base_x, double base_y,
	double direction_x, double direction_y,
	double arrow_size, double head_size,
	int type);

void g_arrow_3D_core(
	double base_x, double base_y, double base_z,                //根元の座標
	double direction_x, double direction_y, double direction_z, //方向
	double arrow_size, double head_size,
	int N, int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_arrow_3D(
	double base_x, double base_y, double base_z,
	double direction_x, double direction_y, double direction_z,
	double arrow_size, double head_size, G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D_smooth_core_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	G_VECTOR n0, G_VECTOR n1, G_VECTOR n2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D_smooth_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	G_VECTOR n0, G_VECTOR n1, G_VECTOR n2,
	G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D_core_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D_smooth_core(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	double nx0, double ny0, double nz0,
	double nx1, double ny1, double nz1,
	double nx2, double ny2, double nz2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D_smooth(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	double nx0, double ny0, double nz0,
	double nx1, double ny1, double nz1,
	double nx2, double ny2, double nz2,
	G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D_core(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_triangle_3D(
	double x0, double y0, double z0,
	double x1, double y1, double z1,
	double x2, double y2, double z2,
	G_BOOL Wire, G_BOOL Fill);

void g_triangle_2D_s(
	G_VECTOR r0, G_VECTOR r1, G_VECTOR r2,
	G_BOOL Wire, G_BOOL Fill);

void g_triangle_2D(
	double x0, double y0,
	double x1, double y1,
	double x2, double y2,
	G_BOOL Wire, G_BOOL Fill);

void g_fan_2D(
	double center_x, double center_y,
	double direction_x, double direction_y,
	double radius,
	double angle,
	G_BOOL Wire, G_BOOL Fill);

void g_fan_3D_core(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius,
	double angle, double psi,
	int FaceNumberLevel, int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_fan_3D(
	double center_x, double center_y, double center_z,
	double direction_x, double direction_y, double direction_z,
	double radius,
	double angle, double psi,
	G_BOOL Wire, G_BOOL Fill);

void g_circle_2D(
	double center_x, double center_y,
	double radius, G_BOOL Wire, G_BOOL Fill);

void g_circle_3D_core(
	double center_x, double center_y, double center_z,
	double radius, double theta, double phi,
	int N, int DivideLevel, G_BOOL Wire, G_BOOL Fill);

void g_circle_3D(
	double center_x, double center_y, double center_z,
	double radius, double theta, double phi,
	G_BOOL Wire, G_BOOL Fill);

void g_polygon_2D(double *xx, double *yy, int n, G_BOOL WIRE, G_BOOL FILL);

void g_polyline_2D(double *xx, double *yy, int n);

void g_polyline_3D(double *xx, double *yy, double *zz, int n);

void g_rectangle_3D_core(
	double x, double y, double z,
	double direction_x, double direction_y, double direction_z,
	double width, double depth,
	double psi,
	int DivideLevel, G_BOOL WIRE, G_BOOL FILL);

void g_rectangle_3D(
	double x, double y, double z,
	double direction_x, double direction_y, double direction_z,
	double width, double depth,
	double psi,
	G_BOOL WIRE, G_BOOL FILL);

extern char *g_key_code_string[];

typedef enum
{
	G_NONE   = 0,
	G_DOWN   = 1,
	G_UP     = 2,
	G_REPEAT = 3,
} G_INPUT_STATE;

enum
{
	G_KEY_INVALID = 0x00,
	G_MOUSE_LEFT = 0x01,
	G_MOUSE_MIDDLE = 0x02,
	G_MOUSE_RIGHT = 0x03,
	G_KEY_PAGE_UP = 0x04,
	G_KEY_PAGE_DOWN = 0x05,
	G_KEY_HOME = 0x06,
	G_KEY_END = 0x07,
	G_KEY_INSERT = 0x0b,
	G_KEY_F1 = 0x0e,
	G_KEY_F2 = 0x0f,
	G_KEY_F3 = 0x10,
	G_KEY_F4 = 0x11,
	G_KEY_F5 = 0x12,
	G_KEY_F6 = 0x13,
	G_KEY_F7 = 0x14,
	G_KEY_F8 = 0x15,
	G_KEY_F9 = 0x16,
	G_KEY_F10 = 0x17,
	G_KEY_F11 = 0x18,
	G_KEY_F12 = 0x19,
	G_KEY_LEFT = 0x1c,
	G_KEY_UP = 0x1d,
	G_KEY_RIGHT = 0x1e,
	G_KEY_DOWN = 0x1f,
};

typedef int G_KEY_CODE;

G_INPUT_STATE g_key_state(G_KEY_CODE key);
G_INPUT_STATE g_input_state(G_KEY_CODE key, int *x, int *y);

//void g_init_off_screen_rendering();
//void g_begin_off_screen_rendering();
//void g_end_off_screen_rendering();

#ifdef __cplusplus
}
#endif

#endif