#include "glsc3d_3_private.h"

#define NUM_LIGHTS 3

struct G_LIGHT
{
	G_VECTOR direction;
	float power;
	//float ambient, diffuse, specular, shininess;
};

G_LIGHT lights[NUM_LIGHTS];
bool lights_enabled[NUM_LIGHTS];

void update_lights()
{
	//G_LIGHT lights[NUM_LIGHTS];
	G_VECTOR light_direction[NUM_LIGHTS];
	float light_power[NUM_LIGHTS];
	int num_lights = 0;

	for (int i = 0; i < NUM_LIGHTS; i++) {
		if (lights_enabled[i]) {
			G_LIGHT &light = lights[i];
			light_direction[num_lights] = light.direction;
			light_power[num_lights] = light.power;
			num_lights++;
		}
	}

#ifndef __APPLE__
	G_DECL_INIT_GLEXT(PFNGLUNIFORM3FVPROC, glUniform3fv);
	G_DECL_INIT_GLEXT(PFNGLUNIFORM1FVPROC, glUniform1fv);
#endif

	g_use_program(g_lighting_program);
	glUniform1i(g_lighting_num_lights_location, num_lights);
	glUniform3fv(g_lighting_light_direction_location, num_lights, (const float *)light_direction);
	glUniform1fv(g_lighting_light_power_location, num_lights, light_power);
	//g_update_uniform(G_UNIFORM_LIGHTS, sizeof(G_LIGHT_LIST), &enabled_lights);
}

void g_init_light_core(G_UINT lightnum, float x, float y, float z, float power)
{
	if (lightnum >= NUM_LIGHTS) return;

#if 0
	G_COLOR vec_zero(0, 0, 0, 0);
	G_COLOR vec_power(power, power, power, power);
	G_VECTOR4 vec_pos(x, y, z, 0);

	GLenum lightname = GL_LIGHT0 + lightnum;

	glLightfv(lightname, GL_AMBIENT, (float *)&vec_zero);
	glLightfv(lightname, GL_DIFFUSE, (float *)&vec_power);
	glLightfv(lightname, GL_SPECULAR, (float *)&vec_power);
	glLightfv(lightname, GL_POSITION, (float *)&vec_pos);

	glEnable(lightname);
#else
	G_LIGHT &target = lights[lightnum];
	target.direction = g_normalize(G_VECTOR(x, y, z));
	target.power = power;

	lights_enabled[lightnum] = true;

	update_lights();
#endif
}

void g_init_light(G_UINT lightnum, float x, float y, float z)
{
	g_init_light_core(lightnum, x, y, z, 1);
}

void g_disable_light(G_UINT lightnum)
{
	lights_enabled[lightnum] = false;
	update_lights();
}
