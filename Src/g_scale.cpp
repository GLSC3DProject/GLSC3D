#include "glsc3d_3_private.h"

int  g_current_scale_id;

G_SCALE glsc3D_inner_scale[TotalDisplayNumber];

typedef struct
{
	G_VECTOR r_0, r_1;
	G_VECTOR r_0_f, r_1_f;
	G_VECTOR eye;
	G_VECTOR up;
	double zoom;
	double x_left_std,  y_top_std,  width_std,  height_std;
} def_scale_struct;
def_scale_struct def_scale[TotalDisplayNumber];

G_BOOL g_clipping_enabled = G_YES;

struct G_TRANSFORM
{
	G_CAMERA camera;

	// Conversion coefficients: (standard coord.) -> (virtual coord. / 2)
	float pixel_scale_x, pixel_scale_y;

	// g_screen_scale_factor
	float screen_scale;
};

#ifndef G_USE_CORE_PROFILE
float g_current_pixel_scale;
#endif

void G_SCALE::select()
{
	int x = (int)(g_screen_scale_factor * screen.x);
	int y = glsc3D_height - (int)(g_screen_scale_factor * (screen.height + screen.y));
	int w = (int)(g_screen_scale_factor * screen.width);
	int h = (int)(g_screen_scale_factor * screen.height);

	G_TRANSFORM transform;
	transform.camera = camera;

	if (g_clipping_enabled) {
		glViewport(x, y, w, h);
	}
	else {
		glViewport(0, 0, glsc3D_width, glsc3D_height);

		float x0 = -2.f * screen.x / screen.width - 1.f;
		float x1 = 2.f * glsc3D_width / w + x0;
		float y1 = 2.f * screen.y / screen.height + 1.f;
		float y0 = -2.f * glsc3D_height / h + y1;

		transform.camera.proj *= G_MATRIX::Ortho2D(x0, x1, y0, y1);
	}

#ifdef G_USE_CORE_PROFILE
	transform.pixel_scale_x = 1 / (screen.width * camera.proj.x.x);
	transform.pixel_scale_y = 1 / (screen.height * camera.proj.y.y);
	transform.screen_scale = g_screen_scale_factor;

	g_update_uniform(G_UNIFORM_MATRICES, sizeof(G_TRANSFORM), &transform);
#else
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float *)&transform.camera.proj);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((float *)&transform.camera.view);

	g_current_pixel_scale = 1 / (screen.height * camera.proj.y.y);
#endif

	glFrontFace(is_left_handed ? GL_CW : GL_CCW);
}

G_SCREEN g_make_screen(int x, int y, int width, int height)
{
	G_SCREEN screen = {x, y, width, height};
	return screen;
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
	glsc3D_inner_scale[id].camera.proj = G_MATRIX::Ortho2D(x_left, x_right, y_bottom, y_top);
	glsc3D_inner_scale[id].camera.view = G_MATRIX::Identity();
	glsc3D_inner_scale[id].screen = g_make_screen(x_left_std, y_top_std, width_std, height_std);
	glsc3D_inner_scale[id].is_3D = false;
	glsc3D_inner_scale[id].is_left_handed = false;
}

void g_def_scale_3D_fix(int id,
	double x_0, double x_1, double y_0, double y_1, double z_0, double z_1,
	double x_left_std, double y_top_std, double width_std, double height_std)
{

	g_def_scale_3D(id,
		x_0, x_1, y_0, y_1, z_0, z_1,
		x_0, x_1, y_0, y_1, z_0, z_1,
		x_left_std, y_top_std, width_std, height_std
	);
}

void g_calc_matrix(
	int id, def_scale_struct *tmp_def_scale
)
{
	if (id >= TotalDisplayNumber) {
		fprintf(stderr,"too large id number\n");
		g_quit();
	}

	G_VECTOR lower = tmp_def_scale[id].r_0_f;
	G_VECTOR upper = tmp_def_scale[id].r_1_f;
	G_VECTOR eye = tmp_def_scale[id].eye;
	G_VECTOR up = tmp_def_scale[id].up;

	float aspect = (float)tmp_def_scale[id].width_std / (float)tmp_def_scale[id].height_std;

	G_VECTOR center = 0.5f * (lower + upper);
	float sphere_r = g_norm(upper - lower) / 2;

	float R = g_norm(eye - center);
	float r_div_sr = R / sphere_r;
	float c = sqrtf(r_div_sr * r_div_sr - 1) * tmp_def_scale[id].zoom;

	G_VECTOR r_10 = tmp_def_scale[id].r_1 - tmp_def_scale[id].r_0;
	G_VECTOR r_10_f = tmp_def_scale[id].r_1_f - tmp_def_scale[id].r_0_f;

	float scale_x = r_10_f.x / r_10.x;
	float scale_y = r_10_f.y / r_10.y;
	float scale_z = r_10_f.z / r_10.z;

	float trans_x = (tmp_def_scale[id].r_1.x*tmp_def_scale[id].r_0_f.x - tmp_def_scale[id].r_0.x*tmp_def_scale[id].r_1_f.x) / r_10.x;
	float trans_y = (tmp_def_scale[id].r_1.y*tmp_def_scale[id].r_0_f.y - tmp_def_scale[id].r_0.y*tmp_def_scale[id].r_1_f.y) / r_10.y;
	float trans_z = (tmp_def_scale[id].r_1.z*tmp_def_scale[id].r_0_f.z - tmp_def_scale[id].r_0.z*tmp_def_scale[id].r_1_f.z) / r_10.z;

	G_MATRIX trans = G_MATRIX::Translation(G_VECTOR(trans_x, trans_y, trans_z));
	G_MATRIX look_at = G_MATRIX::LookAt(eye, center, up);

	glsc3D_inner_scale[id].camera.proj = G_MATRIX::Perspective(c, aspect, R*0.25f, R + sphere_r);
	glsc3D_inner_scale[id].camera.view = G_MATRIX::Scaling(scale_x, scale_y, scale_z) * trans * look_at;
	glsc3D_inner_scale[id].camera.view_normal = G_MATRIX::Scaling(1/scale_x, 1/scale_y, 1/scale_z) * trans * look_at;
	glsc3D_inner_scale[id].screen = g_make_screen(tmp_def_scale[id].x_left_std, tmp_def_scale[id].y_top_std, tmp_def_scale[id].width_std, tmp_def_scale[id].height_std);
	glsc3D_inner_scale[id].is_3D = true;
	glsc3D_inner_scale[id].is_left_handed = (scale_x * scale_y * scale_z < 0);
}

void g_def_scale_3D(
	int id,
	double x_0, double x_1, double y_0, double y_1, double z_0, double z_1,
	double x_0_f, double x_1_f, double y_0_f, double y_1_f, double z_0_f, double z_1_f,
	double x_left_std, double y_top_std, double width_std, double height_std
)
{
	if (x_0_f >= x_1_f) printf("ERROR: GLSC3D requires the condition x_0_f < x_1_f: EXIT...\n"), exit(0);
	if (y_0_f >= y_1_f) printf("ERROR: GLSC3D requires the condition y_0_f < y_1_f: EXIT...\n"), exit(0);
	if (z_0_f >= z_1_f) printf("ERROR: GLSC3D requires the condition z_0_f < z_1_f: EXIT...\n"), exit(0);

	def_scale[id].r_0 = G_VECTOR (x_0,y_0,z_0);
	def_scale[id].r_1 = G_VECTOR (x_1,y_1,z_1);
	def_scale[id].r_0_f = G_VECTOR (x_0_f,y_0_f,z_0_f);
	def_scale[id].r_1_f = G_VECTOR (x_1_f,y_1_f,z_1_f);

	G_VECTOR eye, up;
	double zoom;

	eye = G_VECTOR (1.3 * (x_1_f - x_0_f), -2.4 * (y_1_f - y_0_f), 2.0 * (z_1_f - z_0_f));
	up = G_VECTOR (0,0,1);
	zoom = 1;

	def_scale[id].eye = eye;
	def_scale[id].up = up;
	def_scale[id].zoom = zoom,
	def_scale[id].x_left_std = x_left_std, def_scale[id].y_top_std = y_top_std, def_scale[id].width_std = width_std, def_scale[id].height_std = height_std;

	g_calc_matrix(id, def_scale);
}

void g_vision(
	int id,
	double eye_x, double eye_y, double eye_z,
	double up_x, double up_y, double up_z,
	double zoom
)
{
	def_scale[id].eye = G_VECTOR(eye_x, eye_y, eye_z);
	def_scale[id].up = G_VECTOR(up_x, up_y, up_z);
	def_scale[id].zoom = zoom;

	g_calc_matrix(id, def_scale);
}

void g_sel_scale(int id)
{
	g_triangle_buffer_flush();
	g_vertex_buffer_flush();

	g_clipping_enabled = G_YES;
	glsc3D_inner_scale[id].select();

	g_current_scale_id = id;
}

void g_boundary(void)
{
	G_SCALE scale;
	scale.screen = g_make_screen(0, 0, glsc3D_width, glsc3D_height);
	scale.camera.proj = G_MATRIX::Ortho2D(0, glsc3D_width, glsc3D_height, 0);
	scale.camera.view = G_MATRIX::Identity();
	scale.camera.view_normal = G_MATRIX::Identity();

	g_vertex_buffer_flush();

	G_BOOL previous_clipping_state = g_clipping_enabled;
	g_clipping_enabled = G_YES;
	scale.select();

	G_SCREEN &s = glsc3D_inner_scale[g_current_scale_id].screen;
	g_move_3D(s.x, s.y, -1);
	g_plot_3D(s.x + s.width, s.y, -1);
	g_plot_3D(s.x + s.width, s.y + s.height, -1);
	g_plot_3D(s.x, s.y + s.height, -1);
	g_plot_3D(s.x, s.y, -1);

	g_vertex_buffer_flush();

	g_clipping_enabled = previous_clipping_state;
	glsc3D_inner_scale[g_current_scale_id].select();
}

void g_clipping(G_BOOL value)
{
	g_clipping_enabled = value;
	glsc3D_inner_scale[g_current_scale_id].select();
}