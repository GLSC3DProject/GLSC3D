#ifndef GLSC3D_H
#define GLSC3D_H

#include <math.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    
/*typedef enum
{
	G_TRUE  = 1,
	G_FALSE = 0
} G_BOOL;
*/
typedef int G_BOOL;
#define G_TRUE  1
#define G_FALSE 0
	
/*typedef enum
{
    G_2D   = 0,
    G_3D   = 1,
} G_DIMENSION;
*/
typedef int G_DIMENSION;
#define G_2D 0
#define G_3D 1

/*typedef enum
{
    G_WIRE   = 0,
    G_FILL   = 1,
} G_WIREFILL;
*/
typedef int G_WIREFILL;
#define G_WIRE 0
#define G_FILL 1

/*typedef enum
{
	G_IPA_GOTHIC = 0,
	G_IPA_GOTHIC_PROPORTIONAL = 1,
	G_IPA_MINCHO = 2,
	G_IPA_MINCHO_PROPORTIONAL = 3,
}G_FONT_ID;
*/
typedef int G_FONT_ID;
#define G_IPA_GOTHIC              0
#define G_IPA_GOTHIC_PROPORTIONAL 1
#define G_IPA_MINCHO              2
#define G_IPA_MINCHO_PROPORTIONAL 3


#define G_OFF_SCREEN			NULL

#define G_WINDOWPOS_CENTERED	0x2FFF0000

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
extern int g_enable_transparent;

extern int TRIANGLE_BUFFER_SIZE;
extern int TEMPORARY_TRIANGLE_BUFFER_SIZE;

#ifndef NDEBUG
#define DBG_WRITE(...) fprintf(stderr, "%s:%d In function '%s'", __FILE__, __LINE__, __func__), fprintf(stderr, "  "  __VA_ARGS__), fprintf(stderr, "\n")
#else
#define DBG_WRITE(...)
#endif
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

extern int color2D_id;
extern int color3D_id;

// ---- g_init.cpp

void g_init_light_core(int lightnum, float lit_pos_x, float lit_pos_y, float lit_pos_z, float lit_pow);

void g_init_light(int lightnum, float lit_pos_x, float lit_pos_y, float lit_pos_z);

void g_init_core (
                      const char *WindowName,int width,int height,
                      int pos_x,int pos_y,
                      double r,double g,double b,
                      int g_enable_transparent_out,
                      int TEMPORARY_TRIANGLE_BUFFER_SIZE_out,
                      int TRIANGLE_BUFFER_SIZE_out
                      );


void g_init (const char *WindowName,int width,int height);

// ---- g_color.cpp

void g_area_color_3D(double r, double g, double b, double a);
void g_area_color_2D(double r, double g, double b, double a);
void g_line_color(double r, double g, double b, double a);
void g_text_color(double r, double g, double b, double a);
void g_marker_color(double r,double g,double b,double a);

void g_scr_color(double r, double g, double b);
void g_cls();
void g_finish();

void g_sleep(double wait_time);

int g_capture();
int g_capture_set(const char *name);
    
void g_def_scale_2D(int id,
                    double x_left, double x_right, double y_bottom, double y_top,
                    double x_left_std, double y_top_std,
                    double width_std, double height_std);

void g_sel_scale_2D(int id);

void g_def_scale_3D_core(int id,
                         double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far,
                         double x_left_std, double y_top_std,
                         double width_std, double height_std,
                         double direction_x, double direction_y, double direction_z,
                         double r,
                         double up_x, double up_y, double up_z);

void g_def_scale_3D(int id,
                    double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far,
                    double x_left_std, double y_top_std,
                    double width_std, double height_std,
                    double direction_x, double direction_y, double direction_z,
                    double r);

void g_sel_scale_3D(int id);

void g_move_3D(double x,double y,double z);
void g_move_2D(double x,double y);

void g_plot_3D(double x,double y,double z);
void g_plot_2D(double x,double y);

void g_marker_type(int type);
void g_marker_size(double size);

void g_marker_3D(double x,double y,double z);
void g_marker_2D(double x,double y);

void g_def_marker(int id, double r, double g, double b, double a, int type, double size);
void g_sel_marker(int id);

void g_line_color(double r,double g,double b,double a);
void g_line_width(double size);
void g_line_type(int type);

void g_def_line(int id, double r, double g, double b, double a, double width, int type);
void g_sel_line(int id);

void g_text_standard(double x,double y, const char *str, ...);

void g_text_3D_virtual(double x, double y, double z, const char *str, ...);
void g_text_2D_virtual(double x, double y, const char *str, ...);

void g_text_font(G_FONT_ID id, unsigned int font_size);
void g_text_font_core(const char *font_type, unsigned int font_size);

void g_def_text(int id, double r, double g, double b, double a, int font, unsigned int font_size);
void g_sel_text(int id);

//#ifndef __cplusplus
//
//void g_bird_view_3D(double x0, double x1,
//	double y0, double y1,
//	int N_x, int N_y,
//	double data[N_x][N_y],
//	G_WIREFILL WireFill);
//
//void g_contln_2D(double x_left, double x_right,
//	double y_bottom, double y_top,
//	int N_x, int N_y,
//	double data2D[N_x][N_y],
//	double level);
//void g_isosurface_3D(double x0, double x1,
//	double y0, double y1,
//	double z0, double z1,
//	int number_x, int number_y, int number_z,
//	double u[number_x][number_y][number_z],
//	double height);
//void g_data_plot_3D(double x0, double x1,
//	double y0, double y1,
//	int N_x, int N_y,
//	double data[N_x][N_y]);
//
//#endif


void g_bird_view_f_3D(double x0, double x1,
                      double y0, double y1,
                      int N_x, int N_y,
                      double *data,
                      G_WIREFILL WireFill);

void g_contln_f_2D(double x_left, double x_right,
                   double y_bottom, double y_top,
                   int N_x, int N_y,
                   double *data2D,
                   double level);

void g_isosurface_f_3D(double x0,double x1,
                         double y0,double y1,
                         double z0,double z1,
                         int number_x,int number_y,int number_z,
                         double *u,
                         double height);

void g_data_plot_f_3D(double x0, double x1,
                      double y0, double y1,
                      int N_x, int N_y,
                      double *data);

void g_data_plot_2D(double x_left, double x_right,
                    double *yy, int n);

// ------ g_box_vert.c ------

//void g_box_2D_vert(double left, double right, double bottom, double top, G_WIREFILL WireFill);
//
//void g_box_3D_vert_core(double x_min, double x_max,
//						double y_min, double y_max,
//						double z_min, double z_max,
//						int DivideLevel, G_WIREFILL WireFill);
//
//void g_box_3D_vert(double x_min, double x_max,
//				   double y_min, double y_max,
//				   double z_min, double z_max);

// ------ g_box.c -----

void g_box_2D(double x, double y,
              double width, double height,
              G_WIREFILL WireFill);

void g_box_3D_core(double x, double y, double z,
                   double width, double height, double depth,
                   int DivideLevel, G_WIREFILL WireFill);
    
void g_box_3D(double x, double y, double z,
              double width, double height, double depth);

void g_sphere_3D_core(double x, double y, double z, double radius, int FaceNumberLevel, int DivideLevel, G_WIREFILL WireFill);
    
void g_sphere_3D(double x, double y, double z, double radius);
    
void g_ellipse_3D_core(double x, double y, double z,
                       double Sx, double Sy, double Sz,
                       double direction_x, double direction_y, double direction_z,
                       int FaceNumberLevel, int DivideLevel, G_WIREFILL WireFill);
    
void g_ellipse_3D(double x, double y, double z,
                  double Sx, double Sy, double Sz,
                  double direction_x, double direction_y, double direction_z);
    
void g_prism_3D_core(double center_x, double center_y, double center_z,
                     double direction_x, double direction_y, double direction_z,
                     double radius,double height, double psi, int N,
                     int DivideLevel, G_WIREFILL WireFill);
    
void g_prism_3D(double center_x, double center_y, double center_z,
                double direction_x, double direction_y, double direction_z,
                double radius,double height, double psi, int N);

void g_cylinder_3D_core(double center_x, double center_y, double center_z,
                        double direction_x, double direction_y, double direction_z,
                        double radius,double height, double psi,
                        int N, int DivideLevel, G_WIREFILL WireFill);
    
void g_cylinder_3D(double center_x, double center_y, double center_z,
                   double direction_x, double direction_y, double direction_z,
                   double radius,double height, double psi);

void g_cone_2D(double center_x, double center_y,
               double direction_x, double direction_y,
               double radius, double head_size,
               int type);

void g_cone_3D_core(double center_x, double center_y, double center_z,
                    double direction_x, double direction_y, double direction_z,
                    double radius,double head_size,
                    int FaceNumberLevel_X, int FaceNumberLevel_Y, int DivideLevel, G_WIREFILL WireFill);
    
void g_cone_3D(double center_x, double center_y, double center_z,
               double direction_x, double direction_y, double direction_z,
               double radius,double head_size);

void g_pyramid_3D_core(double center_x, double center_y, double center_z,
                       double direction_x, double direction_y, double direction_z,
                       double radius,double head_size, double psi,
                       int N, int DivideLevel, G_WIREFILL WireFill);
    
void g_pyramid_3D(double center_x, double center_y, double center_z,
                  double direction_x, double direction_y, double direction_z,
                  double radius,double head_size, double psi,
                  int N);

void g_arrow_2D(double base_x, double base_y,
                double direction_x, double direction_y,
                double arrow_size, double head_size,
                int type);

void g_arrow_3D_core(double base_x, double base_y, double base_z,
                     double direction_x, double direction_y, double direction_z,
                     double arrow_size, double head_size, int FaceNumberLevel_X, int FaceNumberLevel_Y, int DivideLevel, G_WIREFILL WireFill);
    
void g_arrow_3D(double base_x, double base_y, double base_z,
                double direction_x, double direction_y, double direction_z,
                double arrow_size, double head_size);

void g_triangle_3D(double x0, double y0, double z0,
                   double x1, double y1, double z1,
                   double x2, double y2, double z2,
                   G_WIREFILL WireFill);

void g_triangle_3D_core(double x0, double y0, double z0,
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        int DivideLevel, G_WIREFILL WireFill);

void g_triangle_2D(double x0, double y0,
                   double x1, double y1,
                   double x2, double y2,
                   G_WIREFILL WireFill);

void g_fan_2D(double center_x, double center_y,
              double direction_x, double direction_y,
              double radius,
              double angle,
              G_WIREFILL WireFill);

void g_fan_3D_core(double center_x, double center_y, double center_z,
                   double direction_x, double direction_y, double direction_z,
                   double radius,
                   double angle, double psi,
                   int FaceNumberLevel, int DivideLevel, G_WIREFILL WireFill);

void g_fan_3D(double center_x, double center_y, double center_z,
              double direction_x, double direction_y, double direction_z,
              double radius,
              double angle, double psi,
              G_WIREFILL WireFill);

void g_circle_2D(double center_x, double center_y,
                 double radius, G_WIREFILL WireFill);

void g_circle_3D_core(double center_x, double center_y, double center_z,
                      double radius,
                      double theta, double phi,
                      int N, int DivideLevel, G_WIREFILL WireFill);

void g_circle_3D(double center_x, double center_y, double center_z,
                 double radius,
                 double theta, double phi,
                 G_WIREFILL WireFill);

void g_polygon_2D(double *xx, double *yy, int n, G_WIREFILL WireFill);

void g_polyline_2D(double *xx, double *yy, int n);

void g_polyline_3D(double *xx, double *yy, double *zz, int n);

void g_rectangle_3D_core(double x, double y, double z,
                         double direction_x, double direction_y, double direction_z,
                         double width, double depth,
                         double psi,
                         int DivideLevel, G_WIREFILL WireFill);

void g_rectangle_3D(double x, double y, double z,
                     double direction_x, double direction_y, double direction_z,
                     double width, double depth,
                     double psi,
                     G_WIREFILL WireFill);

extern char *g_key_code_string[];

typedef enum
{
	G_NONE   = 0,
	G_DOWN   = 1,
	G_UP     = 2,
	G_REPEAT = 3,
} G_INPUT_STATE;

typedef enum
{
	G_KEY_INVALID   = 0x00,
	G_KEY_PAGE_UP   = 0x01,
	G_KEY_PAGE_DOWN = 0x02,
	G_KEY_HOME      = 0x03,
	G_KEY_END       = 0x04,
	G_MOUSE_LEFT    = 0x05,
	G_MOUSE_MIDDLE  = 0x06,
	G_MOUSE_RIGHT   = 0x07,
	G_KEY_INSERT    = 0x0b,
	G_KEY_F1        = 0x0e,
	G_KEY_F2        = 0x0f,
	G_KEY_F3        = 0x10,
	G_KEY_F4        = 0x11,
	G_KEY_F5        = 0x12,
	G_KEY_F6        = 0x13,
	G_KEY_F7        = 0x14,
	G_KEY_F8        = 0x15,
	G_KEY_F9        = 0x16,
	G_KEY_F10       = 0x17,
	G_KEY_F11       = 0x18,
	G_KEY_F12       = 0x19,
	G_KEY_LEFT      = 0x1c,
	G_KEY_UP        = 0x1d,
	G_KEY_RIGHT     = 0x1e,
	G_KEY_DOWN      = 0x1f,
} G_KEY_CODE_CONSTANT;

typedef int G_KEY_CODE;

G_INPUT_STATE g_key_state(G_KEY_CODE key);
G_INPUT_STATE g_input_state(G_KEY_CODE key, int *x, int *y);

void g_init_off_screen_rendering();
void g_begin_off_screen_rendering();
void g_end_off_screen_rendering();

#ifdef __cplusplus
}
#endif

#endif
