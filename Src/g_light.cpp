#include "glsc3d_3_private.h"

#define NUM_LIGHTS 3

#ifdef G_USE_CORE_PROFILE

struct G_LIGHT
{
	G_VECTOR direction;
	float power;
};

G_LIGHT lights[NUM_LIGHTS];
bool is_lights_enabled[NUM_LIGHTS];

void update_lights()
{
	struct {
		G_LIGHT lights[NUM_LIGHTS];
		int count;
	} enabled_list;

	memset(&enabled_list, 0, sizeof(enabled_list));

	for (int i = 0; i < NUM_LIGHTS; i++) {
		if (is_lights_enabled[i]) {
			enabled_list.lights[enabled_list.count++] = lights[i];
		}
	}

	g_use_program(g_lighting_program);
	g_update_uniform(G_UNIFORM_LIGHTS, sizeof(enabled_list), &enabled_list);
}

#endif

void g_init_light_core(G_UINT lightnum, float x, float y, float z, float power)
{
	if (lightnum >= NUM_LIGHTS) return;

#ifdef G_USE_CORE_PROFILE
	is_lights_enabled[lightnum] = true;

	lights[lightnum].direction = g_normalize(G_VECTOR(x, y, z));
	lights[lightnum].power = power;

	update_lights();
#else
	float vec_zero[] = { 0, 0, 0, 0 };
	float vec_power[] = { power, power, power, 0 };
	float vec_pos[] = { x, y, z, 0 };

	GLenum lightname = GL_LIGHT0 + lightnum;

	glLightfv(lightname, GL_AMBIENT, vec_zero);
	glLightfv(lightname, GL_DIFFUSE, vec_power);
	glLightfv(lightname, GL_SPECULAR, vec_power);
	glLightfv(lightname, GL_POSITION, vec_pos);

	glEnable(lightname);
#endif
}

void g_init_light(G_UINT lightnum, float x, float y, float z)
{
	g_init_light_core(lightnum, x, y, z, 1);
}

void g_disable_light(G_UINT lightnum)
{
#ifdef G_USE_CORE_PROFILE
	is_lights_enabled[lightnum] = false;
	update_lights();
#else
	GLenum lightname = GL_LIGHT0 + lightnum;
	glDisable(lightname);
#endif
}
