#include "glsc3d.h"

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

#define WINDOW_SIZE		1024

// returns rand between 0 and 1
float frand() { return (float)rand() / RAND_MAX; }

#ifdef USE_VIRTUAL_SIZE
float frand_coord() { return frand() * (2 * MAX_PARTICLE_SIZE) - MAX_PARTICLE_SIZE; }
#else
float frand_coord() { return frand() * 2 - 1; }
#endif

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

int main()
{
	std::vector<Particle> particles(NUM_PARTICLES);

	g_init("GLSC3D", WINDOW_SIZE, WINDOW_SIZE);

	//g_scr_color(1, 1, 1);
	//g_line_color(0, 0, 0, 1);

	g_def_scale_3D(0, -1, 1, -1, 1, -1, 1, 0, 0, WINDOW_SIZE, WINDOW_SIZE, 0, 0.75f, 1, 8);

	g_marker_type(G_MARKER_SPHERE);

	bool rotation_mode = false;
	float theta = 0;

	while (true) {
		if (g_key_state(' ') == G_DOWN)
			rotation_mode = !rotation_mode;

		if (g_key_state('0') == G_DOWN)
			g_marker_type(0);

		if (g_key_state('1') == G_DOWN)
			g_marker_type(1);

		if (g_key_state('2') == G_DOWN)
			g_marker_type(2);

		g_cls();

		if (rotation_mode) {
			theta += (float)M_PI / 720;
			g_def_scale_3D(0, -1, 1, -1, 1, -1, 1, 0, 0, WINDOW_SIZE, WINDOW_SIZE, sin(theta), 0.75f, cos(theta), 8);
		}

		g_sel_scale_3D(0);
		g_line_width(2);

		if (rotation_mode) {
			for (Particle &p : particles) {
				p.Render();
			}
		}
		else {
			for (Particle &p : particles) {
				p.Update();
				p.Render();
			}
		}

		g_box_3D_core(0, 0, 0, 2, 2, 2, 0, G_WIRE);

		g_finish();
	}
	return 0;
}