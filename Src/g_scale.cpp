#include "glsc3d_3_private.h"

int g_current_scale_id;
G_SCALE *g_current_scale_ptr = nullptr;

G_SCALE glsc3D_inner_scale[TotalDisplayNumber];

struct G_DEF_SCALE
{
	G_VECTOR r_0, r_1;
	G_VECTOR r_0_f, r_1_f;
	G_VECTOR eye;
	G_VECTOR up;
	double zoom;
	double x_left_std,  y_top_std,  width_std,  height_std;
};
G_DEF_SCALE def_scale[TotalDisplayNumber];

G_BOOL g_clipping_enabled = G_YES;

struct G_TRANSFORM
{
	G_CAMERA camera;

	// Conversion coefficients: (standard coord.) -> (virtual coord. / 2)
	float pixel_scale_x, pixel_scale_y;

	// g_screen_scale_factor
	float screen_scale;

	float pad = 0;
};

void g_select_camera(const G_CAMERA &camera, float screen_width, float screen_height)
{
#ifdef G_USE_VERTEX_BUFFERS
	G_TRANSFORM transform;
	transform.camera = camera;
	transform.pixel_scale_x = 1 / (screen_width * camera.proj.x.x);
	transform.pixel_scale_y = 1 / (screen_height * camera.proj.y.y);
	transform.screen_scale = g_screen_scale_factor;

#ifdef G_USE_METAL
	g_command_encoder->setVertexBytes(&transform, sizeof(G_TRANSFORM), G_BUFFER_MATRICES);
	g_command_encoder->setFragmentBytes(&transform, sizeof(G_TRANSFORM), G_BUFFER_MATRICES);
#else
	g_update_uniform(G_UNIFORM_MATRICES, sizeof(G_TRANSFORM), &transform);
#endif
#else
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float *)&camera.proj);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((float *)&camera.view);
#endif
}

#ifndef G_USE_VERTEX_BUFFERS
float g_current_pixel_scale;
#endif

void G_SCALE::select()
{
	int width = (int)(g_screen_scale_factor * screen.width);
	int height = (int)(g_screen_scale_factor * screen.height);

	if (g_clipping_enabled) {
		int x = (int)(g_screen_scale_factor * screen.x);
#ifdef G_USE_METAL
		int y = (int)(g_screen_scale_factor * screen.y);
#else
		int y = glsc3D_height - (int)(g_screen_scale_factor * (screen.height + screen.y));
#endif
		g_set_viewport(x, y, width, height);
		g_select_camera(camera, screen.width, screen.height);
	}
	else {
		g_set_viewport(0, 0, glsc3D_width, glsc3D_height);

		float x0 = -2.f * screen.x / screen.width - 1.f;
		float x1 = 2.f * glsc3D_width / width + x0;
		float y1 = 2.f * screen.y / screen.height + 1.f;
		float y0 = -2.f * glsc3D_height / height + y1;

		G_CAMERA c = camera;

		c.proj *= G_MATRIX::Ortho2D(x0, x1, y0, y1);
		g_select_camera(c, screen.width, screen.height);
	}

#ifndef G_USE_VERTEX_BUFFERS
	g_current_pixel_scale = 1 / (screen.height * camera.proj.y.y);
#endif

#ifdef G_USE_METAL
	g_command_encoder->setFrontFacingWinding(
		is_left_handed ? MTL::WindingClockwise : MTL::WindingCounterClockwise);
#else
	glFrontFace(is_left_handed ? GL_CW : GL_CCW);
#endif
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
		exit(0);
	}

	glsc3D_inner_scale[id].camera.proj = G_MATRIX::Ortho2D(x_left, x_right, y_bottom, y_top);
	glsc3D_inner_scale[id].camera.view = G_MATRIX::Identity();
	glsc3D_inner_scale[id].screen = g_make_screen(x_left_std, y_top_std, width_std, height_std);
	glsc3D_inner_scale[id].is_3D = false;
	glsc3D_inner_scale[id].is_left_handed = false;
}

void g_def_scale_3D_fix(int id,
	double x_0_f, double x_1_f, double y_0_f, double y_1_f, double z_0_f, double z_1_f,
	double x_left_std, double y_top_std, double width_std, double height_std)
{
	g_def_scale_3D(id,
		x_0_f, x_1_f, y_0_f, y_1_f, z_0_f, z_1_f,
		x_0_f, x_1_f, y_0_f, y_1_f, z_0_f, z_1_f,
		x_left_std, y_top_std, width_std, height_std
	);
}

void g_calc_matrix(int id)
{
	if (id >= TotalDisplayNumber) {
		fprintf(stderr,"too large id number\n");
		exit(0);
	}

	G_VECTOR lower = def_scale[id].r_0_f;
	G_VECTOR upper = def_scale[id].r_1_f;
	G_VECTOR eye = def_scale[id].eye;
	G_VECTOR up = def_scale[id].up;

	float aspect = (float)def_scale[id].width_std / (float)def_scale[id].height_std;

	G_VECTOR center = 0.5f * (lower + upper);
	float sphere_r = g_norm(upper - lower) / 2;

	float R = g_norm(eye - center);
	float r_div_sr = R / sphere_r;
	float c = sqrtf(r_div_sr * r_div_sr - 1) * def_scale[id].zoom;

	G_VECTOR r_10 = def_scale[id].r_1 - def_scale[id].r_0;
	G_VECTOR r_10_f = def_scale[id].r_1_f - def_scale[id].r_0_f;

	float scale_x = r_10_f.x / r_10.x;
	float scale_y = r_10_f.y / r_10.y;
	float scale_z = r_10_f.z / r_10.z;

	float trans_x = (def_scale[id].r_1.x*def_scale[id].r_0_f.x - def_scale[id].r_0.x*def_scale[id].r_1_f.x) / r_10.x;
	float trans_y = (def_scale[id].r_1.y*def_scale[id].r_0_f.y - def_scale[id].r_0.y*def_scale[id].r_1_f.y) / r_10.y;
	float trans_z = (def_scale[id].r_1.z*def_scale[id].r_0_f.z - def_scale[id].r_0.z*def_scale[id].r_1_f.z) / r_10.z;

	G_MATRIX trans = G_MATRIX::Translation(G_VECTOR(trans_x, trans_y, trans_z));
	G_MATRIX look_at = G_MATRIX::LookAt(eye, center, up);

	glsc3D_inner_scale[id].camera.proj = G_MATRIX::Perspective(c, aspect, R*0.25f, R + sphere_r);
	glsc3D_inner_scale[id].camera.view = G_MATRIX::Scaling(scale_x, scale_y, scale_z) * trans * look_at;
	glsc3D_inner_scale[id].camera.view_normal = G_MATRIX::Scaling(1/scale_x, 1/scale_y, 1/scale_z) * trans * look_at;
	glsc3D_inner_scale[id].screen = g_make_screen(def_scale[id].x_left_std, def_scale[id].y_top_std, def_scale[id].width_std, def_scale[id].height_std);
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

	G_VECTOR center = (def_scale[id].r_0_f + def_scale[id].r_1_f) * 0.5f;
	G_VECTOR eye, up;
	double zoom;

	eye = G_VECTOR (1.3 * (x_1_f - x_0_f), -2.4 * (y_1_f - y_0_f), 2.0 * (z_1_f - z_0_f)) + center;
	up = G_VECTOR (0,0,1);
	zoom = 1;

	def_scale[id].eye = eye;
	def_scale[id].up = up;
	def_scale[id].zoom = zoom,
	def_scale[id].x_left_std = x_left_std;
	def_scale[id].y_top_std = y_top_std;
	def_scale[id].width_std = width_std;
	def_scale[id].height_std = height_std;

	g_calc_matrix(id);
}

void g_vision(
	int id,
	double eye_x, double eye_y, double eye_z,
	double up_x, double up_y, double up_z,
	double zoom
)
{
	G_VECTOR center = (def_scale[id].r_0_f + def_scale[id].r_1_f) * 0.5f;

	def_scale[id].eye = G_VECTOR(eye_x, eye_y, eye_z) + center;
	def_scale[id].up = G_VECTOR(up_x, up_y, up_z);
	def_scale[id].zoom = zoom;

	g_calc_matrix(id);
}

void g_sel_scale(int id)
{
	g_triangle_buffer_flush();
	g_vertex_buffer_flush();
	g_reset_primitive_mode();

	g_clipping_enabled = G_YES;

	g_current_scale_id = id;
	g_current_scale_ptr = &glsc3D_inner_scale[id];
	g_current_scale_ptr->select();
}

void g_boundary(void)
{
	if (g_current_scale_ptr == nullptr) {
		printf("Call g_sel_scale before g_boundary\n");
		g_quit();
	}

	g_vertex_buffer_flush();

	g_set_viewport(0, 0, glsc3D_width, glsc3D_height);

	float screen_width = glsc3D_width / g_screen_scale_factor;
	float screen_height = glsc3D_height / g_screen_scale_factor;

	G_CAMERA camera;
	camera.proj = G_MATRIX::Ortho2D(0, screen_width, screen_height, 0);
	camera.view = G_MATRIX::Identity();
	camera.view_normal = G_MATRIX::Identity();

	g_select_camera(camera, screen_width, screen_height);

	G_SCREEN &s = g_current_scale_ptr->screen;
	g_move_3D(s.x, s.y, 0);
	g_plot_3D(s.x + s.width, s.y, 0);
	g_plot_3D(s.x + s.width, s.y + s.height, 0);
	g_plot_3D(s.x, s.y + s.height, 0);
	g_plot_3D(s.x, s.y, 0);

	g_vertex_buffer_flush();

	g_current_scale_ptr->select();
}

void g_clipping(G_BOOL value)
{
	if (g_current_scale_ptr == nullptr) {
		printf("Call g_sel_scale before g_clipping\n");
		g_quit();
	}

	g_clipping_enabled = value;
	g_current_scale_ptr->select();
}