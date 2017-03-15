#include "glsc3d.h"

#include <cstdlib>
#include <vector>

#define NUM_PARTICLES	256
//#define NUM_PARTICLES	262144

#define MIN_PARTICLE_SIZE	32
#define MAX_PARTICLE_SIZE	128

#define TIME_STEP		(1.f / 512)

#define WINDOW_SIZE		2560

// return rand between 0 and 1
float frand() { return (float)rand() / RAND_MAX; }

class Particle
{
	G_VECTOR position, velocity;
	G_COLOR color;
	float size;

public:
	// Constructor
	Particle() :
		position(frand() * 2 - 1, frand() * 2 - 1, frand() * 2 - 1),
		velocity(frand() * 2 - 1, frand() * 2 - 1, frand() * 2 - 1),
		color(frand(), frand(), frand(), 1),
		size(frand() * (MAX_PARTICLE_SIZE - MIN_PARTICLE_SIZE) + MIN_PARTICLE_SIZE) {}

	void Update()
	{
		position += velocity * TIME_STEP;

		if (position.x >= 1 || position.x <= -1) velocity.x *= -1;
		if (position.y >= 1 || position.y <= -1) velocity.y *= -1;
		if (position.z >= 1 || position.z <= -1) velocity.z *= -1;
	}

	void Render()
	{
		g_marker_size(size);
		g_marker_color_s(color);
		g_marker_s(position);
	}
};

int main()
{
	std::vector<Particle> particles(NUM_PARTICLES);

	g_init("GLSC3D", WINDOW_SIZE, WINDOW_SIZE);

	g_def_scale_3D(0, -1, 1, -1, 1, -1, 1, 0, 0, WINDOW_SIZE, WINDOW_SIZE, 0, 0.75f, 1, 8);

	g_marker_type(G_MARKER_SPHERE);

	bool rotation_mode = false;
	float theta = 0;

	while (true) {
		if (g_key_state(' ') == G_DOWN)
			rotation_mode = !rotation_mode;

		g_cls();

		if (rotation_mode) {
			theta += (float)M_PI / 720;
			g_def_scale_3D(0, -1, 1, -1, 1, -1, 1, 0, 0, WINDOW_SIZE, WINDOW_SIZE, sin(theta), 0.75f, cos(theta), 8);
		}

		g_sel_scale_3D(0);
		g_box_3D_core(0, 0, 0, 2, 2, 2, 0, G_WIRE);

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

		g_finish();
	}
	return 0;
}