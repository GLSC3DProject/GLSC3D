#include "glsc3d_private.h"

int             glsc3D_width;
int             glsc3D_height;
float			g_screen_scale_factor;
float			g_retina_scale_factor;

int g_enable_transparent, TRIANGLE_BUFFER_SIZE, TEMPORARY_TRIANGLE_BUFFER_SIZE;

void g_init_core(
					const char *WindowName,int width,int height,
					int pos_x,int pos_y,
					float r,float g,float b,
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
//	glDebugMessageCallback(g_debug_callback, NULL);
#endif

	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_CULL_FACE);

//	int range[2];
//	glGetIntegerv(GL_POINT_SIZE_RANGE, range);
//	printf("Supported point size range : [%d, %d]\n", range[0], range[1]);

	g_vertex_buffer_init();

#ifdef G_USE_CORE_PROFILE
	g_shader_program_init();
	g_text_init();

	glEnable(GL_PROGRAM_POINT_SIZE);
#else
	GLfloat specular[4] = {1, 1, 1, 1};

	glShadeModel(GL_SMOOTH);

//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
#endif

	g_init_light(0, 1, 1, 1);

	g_enable_transparent = g_enable_transparent_out;

	if(g_enable_transparent)
	{
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
}

void g_init (const char *WindowName,int width,int height)
{
	g_init_core(WindowName,width,height,G_WINDOWPOS_CENTERED,G_WINDOWPOS_CENTERED,0,0,0,0,0,0);
}
