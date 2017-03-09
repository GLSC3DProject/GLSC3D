#include "glsc3d_private_ext.h"

int             glsc3D_width;
int             glsc3D_height;
float			g_screen_scale_factor;

int g_enable_transparent, TRIANGLE_BUFFER_SIZE, TEMPORARY_TRIANGLE_BUFFER_SIZE;

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

void g_init_core(
					const char *WindowName,int width,int height,
					int pos_x,int pos_y,
					double r,double g,double b,
					int g_enable_transparent_out,
					int TEMPORARY_TRIANGLE_BUFFER_SIZE_out,
					int TRIANGLE_BUFFER_SIZE_out
				)
{
#if defined(_WIN32) && !defined(__MINGW32_VERSION)
	SetProcessDPIAware();
#endif

	g_sdl_init(WindowName, pos_x, pos_y, width, height);

//	printf("OpenGL Version : %s\n", glGetString(GL_VERSION));

#ifdef G_ENABLE_OPENGL_DEBUG_CALLBACK
	glDebugMessageCallback(g_debug_callback, NULL);
#endif

	g_vertex_buffer_init();

#ifdef G_USE_CORE_PROFILE
	g_shader_program_init();
#else
	GLfloat specular[4] = {1, 1, 1, 1};

	glShadeModel(GL_SMOOTH);

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
#endif

//	g_input_init();
	g_text_init();

	g_init_light(0, 1, 1, 1);

	g_enable_transparent = g_enable_transparent_out;

	if(g_enable_transparent)
	{
		glEnable(GL_BLEND);

		if(TEMPORARY_TRIANGLE_BUFFER_SIZE_out == 0)
		{
			TEMPORARY_TRIANGLE_BUFFER_SIZE = 1 << 10;
		}
		else
		{
			TEMPORARY_TRIANGLE_BUFFER_SIZE = TEMPORARY_TRIANGLE_BUFFER_SIZE_out;
		}
		if(TRIANGLE_BUFFER_SIZE_out == 0)
		{
			TRIANGLE_BUFFER_SIZE = 1 << 20;
		}
		else
		{
			TRIANGLE_BUFFER_SIZE = TRIANGLE_BUFFER_SIZE_out;
		}

		g_triangle_buffer_init();
	}

	g_scr_color(r,g,b);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	g_finish();
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//if(WindowName == G_OFF_SCREEN)
	//{
	//	g_init_off_screen_rendering();
	//	g_begin_off_screen_rendering();
	//	glutHideWindow();
	//}
}

void g_init (const char *WindowName,int width,int height)
{
	g_init_core(WindowName,width,height,G_WINDOWPOS_CENTERED,G_WINDOWPOS_CENTERED,0,0,0,0,0,0);
}
