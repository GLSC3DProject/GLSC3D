#include "glsc3d_private.h"

#ifdef G_USE_CORE_PROFILE

struct G_LIGHT
{
	G_VECTOR direction;
	float direction_w;
	float ambient, diffuse, specular, shininess;
};

void g_init_light_core(int lightnum, float lit_pos_x, float lit_pos_y, float lit_pos_z, float lit_pow)
{
	if (lightnum != 0) return;

	G_LIGHT light;
	light.direction = g_normalize(G_VECTOR(lit_pos_x, lit_pos_y, lit_pos_z));
	light.direction_w = 0;
	light.ambient = 0;
	light.diffuse = lit_pow;
	light.specular = 1.0f;
	light.shininess = 64;

	g_update_uniform(G_UNIFORM_LIGHTS, sizeof(light), &light);
}

#else

void g_init_light_core(int lightnum, float lit_pos_x, float lit_pos_y, float lit_pos_z, float lit_pow)
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

void g_init_light(int lightnum, float lit_pos_x, float lit_pos_y, float lit_pos_z)
{
	g_init_light_core(lightnum,lit_pos_x,lit_pos_y,lit_pos_z, 1.0);
}
