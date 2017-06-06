#include "glsc3d_3_private.h"

int  g_current_scale_id;
G_DIMENSION g_scale_dim_flag;

G_SCALE glsc3D_inner_scale[TotalDisplayNumber];

#ifdef G_USE_CORE_PROFILE
struct G_TRANSFORM
{
	G_CAMERA camera;

	// Conversion coefficients: (standard coord.) -> (virtual coord. / 2)
	float pixel_scale_x, pixel_scale_y;

	// g_screen_scale_factor
	float screen_scale;
};
#else
float g_current_pixel_scale;
#endif

void G_SCALE::select()
{
	int x = (int)(g_screen_scale_factor * screen.x);
	int y = glsc3D_height - (int)(g_screen_scale_factor * (screen.height + screen.y));
	int w = (int)(g_screen_scale_factor * screen.width);
	int h = (int)(g_screen_scale_factor * screen.height);

	glViewport(x, y, w, h);
#ifdef G_USE_CORE_PROFILE
	G_TRANSFORM transform;
	transform.camera = camera;
	transform.pixel_scale_x = 1 / (screen.width * camera.proj.x.x);
	transform.pixel_scale_y = 1 / (screen.height * camera.proj.y.y);
	transform.screen_scale = g_screen_scale_factor;

	g_update_uniform(G_UNIFORM_MATRICES, sizeof(G_TRANSFORM), &transform);
#else
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float *)&camera.proj);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((float *)&camera.view);

	g_current_pixel_scale = 1 / (screen.height * camera.proj.y.y);
#endif
}

void g_def_scale_2D(
	int id,                                                         //id
	double x_left, double x_right, double y_bottom, double y_top,   //仮想座標系
	double x_left_std, double y_top_std,                            //ウィンドウの位置
	double width_std, double height_std)                            //ウィンドウのサイズ
{
	if (id >= TotalDisplayNumber) {
		fprintf(stderr,"too large id number\n");
		g_quit();
	}
	glsc3D_inner_scale[id].camera = g_make_camera_2D(x_left, x_right, y_bottom, y_top);
	glsc3D_inner_scale[id].screen = g_make_screen(x_left_std, y_top_std, width_std, height_std);
}

void g_def_scale_3D(
	int id,
	double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far, //仮想座標系
	double x_left_std, double y_top_std,                          //ウィンドウの位置
	double width_std, double height_std,                          //ウィンドウのサイズ
	double direction_x, double direction_y, double direction_z,
	double r)                                                     //視点位置
{
	g_def_scale_3D_core(id,x_left,x_right,y_bottom,y_top,z_near,z_far,x_left_std,y_top_std,width_std,height_std,direction_x,direction_y,direction_z,r,0,1,0);
}

void g_def_scale_3D_core(
	int id,
	double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far,
	double x_left_std, double y_top_std,
	double width_std, double height_std,
	double direction_x, double direction_y, double direction_z,
	double r,
	double up_x, double up_y, double up_z)
{
	if (id >= TotalDisplayNumber) {
		fprintf(stderr,"too large id number\n");
		g_quit();
	}
	G_VECTOR lower(x_left, y_bottom, z_near);
	G_VECTOR upper(x_right, y_top, z_far);
	G_VECTOR direction(direction_x, direction_y, direction_z);
	G_VECTOR up(up_x, up_y, up_z);

	float aspect = (float)width_std / (float)height_std;

	glsc3D_inner_scale[id].camera = g_make_camera_3D_core(lower, upper, g_normalize(direction), r, aspect, up);
	glsc3D_inner_scale[id].screen = g_make_screen(x_left_std, y_top_std, width_std, height_std);
}

void g_sel_scale_private(int id, G_DIMENSION dimension, bool boundary)
{
	g_triangle_buffer_flush();
	g_vertex_buffer_flush();

	if (boundary) {
		G_SCALE scale;
		scale.screen = glsc3D_inner_scale[id].screen;
		scale.camera.proj = G_MATRIX::Identity();
		scale.camera.view = G_MATRIX::Identity();
		scale.select();

		float line_size = g_current_line_size;
		g_line_width(line_size * 2);

		g_move_2D(-1, -1);
		g_plot_2D(+1, -1);
		g_plot_2D(+1, +1);
		g_plot_2D(-1, +1);
		g_plot_2D(-1, -1);

		g_vertex_buffer_flush();
		g_line_width(line_size);
	}

	glsc3D_inner_scale[id].select();

	if (dimension == G_3D)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	g_scale_dim_flag = dimension;
	g_current_scale_id = id;
}

void g_sel_scale_2D(int id)
{
	g_sel_scale_private(id, G_2D, false);
}

void g_sel_scale_3D(int id)
{
	g_sel_scale_private(id, G_3D, false);
}

void g_sel_scale_2D_boundary(int id)
{
	g_sel_scale_private(id, G_2D, true);
}

void g_sel_scale_3D_boundary(int id)
{
	g_sel_scale_private(id, G_3D, true);
}
