#include "glsc3d_3.h"

#include <cstdlib>
#include <vector>

//#define MANY_PARTICLES

#define USE_VIRTUAL_SIZE

#ifdef MANY_PARTICLES
#define NUM_PARTICLES	262144

#ifdef USE_VIRTUAL_SIZE
#define MIN_PARTICLE_SIZE	(1.f / 512)
#define MAX_PARTICLE_SIZE	(1.f / 128)
#else
#define MIN_PARTICLE_SIZE	2
#define MAX_PARTICLE_SIZE	4
#endif

#else

#define NUM_PARTICLES	256
#ifdef USE_VIRTUAL_SIZE
#define MIN_PARTICLE_SIZE	(1.f / 32)
#define MAX_PARTICLE_SIZE	(1.f / 8)
#else
#define MIN_PARTICLE_SIZE	32
#define MAX_PARTICLE_SIZE	64
#endif

#endif

#define TIME_STEP		(1.f / 512)

#define WINDOW_SIZE		640

// returns rand between 0 and 1
float frand() { return (float)rand() / RAND_MAX; }

class Particle
{
	G_VECTOR position, velocity;
	G_COLOR color;
	float size;

public:
	Particle() :
		position(frand() * 2 - 1, frand() * 2 - 1, frand() * 2 - 1),
		velocity(frand() * 2 - 1, frand() * 2 - 1, frand() * 2 - 1),
		color(frand(), frand(), frand(), 1),
		size(frand() * (MAX_PARTICLE_SIZE - MIN_PARTICLE_SIZE) + MIN_PARTICLE_SIZE)
	{
#ifdef USE_VIRTUAL_SIZE
		position *= (1 - size);
#endif
	}

	void Update()
	{
		position += velocity * TIME_STEP;

#ifdef USE_VIRTUAL_SIZE
		if (fabs(position.x) + size >= 1) velocity.x *= -1;
		if (fabs(position.y) + size >= 1) velocity.y *= -1;
		if (fabs(position.z) + size >= 1) velocity.z *= -1;
#else
		if (fabs(position.x) >= 1) velocity.x *= -1;
		if (fabs(position.y) >= 1) velocity.y *= -1;
		if (fabs(position.z) >= 1) velocity.z *= -1;
#endif
	}

	void Render()
	{
#ifdef USE_VIRTUAL_SIZE
		g_marker_radius(size);
#else
		g_marker_size(size);
#endif
		g_marker_color_s(color);
		g_marker_s(position);
	}
};

//int main()
//{
//	std::vector<Particle> particles(NUM_PARTICLES);
//
//	g_enable_highdpi();
//	g_init("GLSC3D", WINDOW_SIZE, WINDOW_SIZE);
//
//	//g_scr_color(1, 1, 1);
//	//g_line_color(0, 0, 0, 1);
//
//	g_def_scale_3D(0, -1, 1, -1, 1, -1, 1, 0, 0, WINDOW_SIZE, WINDOW_SIZE, 0, 0.75f, 1, 8);
//
//	g_marker_type(G_MARKER_SPHERE);
//	g_line_width(2);
//	g_line_type(1);
//	g_text_size(18);
//
//	bool rotation_mode = false;
//	float theta = 0;
//
//	while (true) {
//		if (g_key_state(' ') == G_DOWN)
//			rotation_mode = !rotation_mode;
//
//		if (g_key_state('0') == G_DOWN)
//			g_marker_type(0);
//
//		if (g_key_state('1') == G_DOWN)
//			g_marker_type(1);
//
//		if (g_key_state('2') == G_DOWN)
//			g_marker_type(2);
//
//		g_cls();
//
//		if (rotation_mode) {
//			theta += (float)M_PI / 720;
//			g_def_scale_3D(0, -1, 1, -1, 1, -1, 1, 0, 0, WINDOW_SIZE, WINDOW_SIZE, sin(theta), 0.75f, cos(theta), 8);
//		}
//
//		g_sel_scale_3D(0);
//
//		if (rotation_mode) {
//			for (Particle &p : particles) {
//				p.Render();
//			}
//		}
//		else {
//			for (Particle &p : particles) {
//				p.Update();
//				p.Render();
//			}
//		}
//
//		g_box_center_3D_core(0, 0, 0, 2, 2, 2, 0, G_YES, G_NO);
//
//		g_text_standard(0, 20, "Press space key to toggle rotation mode");
//		g_text_standard(0, 40, "Press 0, 1, 2 key to change marker type");
//
//		g_finish();
//	}
//	return 0;
//}

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_ellipse.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

//	g_init_light(0,0,1,1);

	double dt = 0.01;

	float scale_x = 1.0, scale_y = 1.0, scale_z = 2.0;
	float obj_scale_x = 1.0, obj_scale_y = 1.0, obj_scale_z = 1.0;

	for (int i_time = 0;; i_time++)
	{
		double t = i_time * dt;

		g_def_scale_3D_core(0, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, scale_x, scale_y, scale_z, 4 *cos(t), 4*sin(t), 4*0.707, 1, 0, 0, 1);
//		g_def_scale_3D_core_legacy(0, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, 4 *cos(t), 4*sin(t), 4*0.707, 1, 0, 0, 1);

		g_cls();

		g_sel_scale_3D(0);

		float radius = 2.0;

		G_VECTOR r[4] = { {radius*cosf(2*M_PI/3), radius*sinf(2*M_PI/3), 0},
						  {radius*cosf(4*M_PI/3), radius*sinf(4*M_PI/3), 0},
						  {radius*cosf(6*M_PI/3), radius*sinf(6*M_PI/3), 0},
						  {0, 0, radius * obj_scale_z} };
//		G_VECTOR g = (r[0]+r[1]+r[2]+r[3])/4;
//		for(int i=0; i<4; i++){
//			r[i] -= g;
//		}

		g_triangle_3D_s(r[0], r[1], r[2], 1, 1);
		g_triangle_3D_s(r[0], r[1], r[3], 1, 1);
		g_triangle_3D_s(r[1], r[2], r[3], 1, 1);
		g_triangle_3D_s(r[0], r[2], r[3], 1, 1);

		g_finish();
	}
	return 0;
}