#include "glsc3d_private.h"

#ifdef G_USE_CORE_PROFILE

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
	GLuint	num_lights, pad1, pad2, pad3;
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

void g_disable_light(G_UINT lightnum)
{
	lights[lightnum].enabled = false;
	update_lights();
}

#else

void g_init_light_core(G_UINT lightnum, float x, float y, float z, float diffuse)
{
//	static GLfloat lit_amb[4]={0.3, 0.3, 0.3, 0.0};	// 環境光の強さ
	GLfloat lit_dif[4]={lit_pow, lit_pow, lit_pow, 0.0};	// 拡散光の強さ
	static GLfloat lit_spc[4]={1.0, 1.0, 1.0, 0.0};	// 鏡面反射光の強さ
	static GLfloat lit_pos[4]={0.0, 0.0, 0.0, 0.0}; // 光源の位置
	int lightname=0;
	
	if (lightnum==0) {
		lightname = GL_LIGHT0;
	}else if (lightnum==1) {
		lightname = GL_LIGHT1;
	}else if (lightnum==2) {
		lightname = GL_LIGHT2;
	}else if (lightnum==3) {
		lightname = GL_LIGHT3;
	}else if (lightnum==4) {
		lightname = GL_LIGHT4;
	}else if (lightnum==5) {
		lightname = GL_LIGHT5;
	}else if (lightnum==6) {
		lightname = GL_LIGHT6;
	}else if (lightnum==7) {
		lightname = GL_LIGHT7;
	}

//	glLightfv(lightname, GL_AMBIENT, lit_amb);
	glLightfv(lightname, GL_DIFFUSE, lit_dif);
	glLightfv(lightname, GL_SPECULAR, lit_spc);
	lit_pos[0] = lit_pos_x;
	lit_pos[1] = lit_pos_y;
	lit_pos[2] = lit_pos_z;
	glLightfv(lightname, GL_POSITION, lit_pos);

	glEnable(lightname);
}
#endif

void g_init_light(G_UINT lightnum, float x, float y, float z)
{
	g_init_light_core(lightnum, x, y, z, 1);
}
