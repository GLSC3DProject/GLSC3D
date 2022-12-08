#include "glsc3d_3_private.h"

#ifndef G_USE_METAL

void g_general_init()
{
//	printf("OpenGL Version : %s\n", glGetString(GL_VERSION));

	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

//	glEnable(GL_CULL_FACE);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#ifdef G_USE_OPENGL_CORE_PROFILE
	glEnable(GL_PROGRAM_POINT_SIZE);
#else
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	glEnable(GL_POINT_SPRITE);

	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
#endif
}

void g_set_viewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void g_use_program(GLuint program)
{
	glUseProgram(program);
}

#endif