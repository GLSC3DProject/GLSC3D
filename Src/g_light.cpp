#include "glsc3d_private.h"

#define NUM_LIGHTS 3

struct G_LIGHT
{
	G_VECTOR direction;
	GLuint enabled; // ignored in the shader
	float ambient, diffuse, specular, shininess;
};

struct G_LIGHT_LIST
{
	G_LIGHT lights[NUM_LIGHTS];
	GLuint	num_lights, pad0, pad1, pad2;
};

G_LIGHT lights[NUM_LIGHTS];

void update_lights()
{
	G_LIGHT_LIST enabled_lights;
	enabled_lights.num_lights = 0;

	for (G_LIGHT light : lights) {
		if (light.enabled)
			enabled_lights.lights[enabled_lights.num_lights++] = light;
	}

	g_update_uniform(G_UNIFORM_LIGHTS, sizeof(G_LIGHT_LIST), &enabled_lights);
}

void g_init_light_core(G_UINT lightnum, float x, float y, float z, float power)
{
	if (lightnum >= NUM_LIGHTS) return;

	G_LIGHT &target = lights[lightnum];
	target.direction = g_normalize(G_VECTOR(x, y, z));
	target.enabled = true;
	target.ambient = 0;
	target.diffuse = power;
	target.specular = power;
	target.shininess = 64;

	update_lights();
}

void g_init_light(G_UINT lightnum, float x, float y, float z)
{
	g_init_light_core(lightnum, x, y, z, 1);
}

void g_disable_light(G_UINT lightnum)
{
	lights[lightnum].enabled = false;
	update_lights();
}
