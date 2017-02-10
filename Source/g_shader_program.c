//
//  g_shader_program.c
//  GLSC3D
//
//  Created by student on 2017/02/07.
//  Copyright © 2017年 student. All rights reserved.
//

#include "glsc3d_private.h"

#define CONSTANT_VERT_SHADER_SOURCE \
	"//#version 310 core\n" \
	"uniform mat4 ModelView;\n" \
	"attribute vec3 Position;\n" \
	"void main() { gl_Position = ModelView * vec4(Position, 1); }"

#define CONSTANT_FRAG_SHADER_SOURCE \
	""

G_BOOL g_lighting_enabled;

GLuint g_constant_program, g_lighting_program;

static const char * ErrorNames[] = {
	"GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION",
	"GL_STACK_OVERFLOW", "GL_STACK_UNDERFLOW", "GL_OUT_OF_MEMORY"
};

void CheckGLError(int checkpoint)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		printf("OpenGL Error : %s Checkpoint: %d\n",
			ErrorNames[error - GL_INVALID_ENUM], checkpoint);
}

void g_assert_shader_compile_status(GLuint shader)
{
	GLint InfoLogLength, CompileStatus;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	
	if (InfoLogLength > 0) {
		char info_log[InfoLogLength];
		
		glGetShaderInfoLog(shader, InfoLogLength, &InfoLogLength, info_log);
		printf("%s\n", info_log);
		
		glGetShaderiv(shader, GL_COMPILE_STATUS, &CompileStatus);
		if (CompileStatus == GL_FALSE) printf("Compile Failed.\n");
//		assert(CompileStatus);
	}
}

GLuint g_create_shader(GLuint program, GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);
	
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	g_assert_shader_compile_status(shader);
	
	glAttachShader(program, shader);
	
	return shader;
}

void g_shader_program_init()
{
//	g_constant_program = glCreateProgram();
//	
//	g_create_shader(g_constant_program, GL_VERTEX_SHADER, CONSTANT_VERT_SHADER_SOURCE);
//	g_create_shader(g_constant_program, GL_FRAGMENT_SHADER, CONSTANT_FRAG_SHADER_SOURCE);
//	
//	glLinkProgram(g_constant_program);
}

void g_enable_lighting()
{
	glEnable(GL_LIGHTING);
	g_lighting_enabled = G_TRUE;
}

void g_disable_lighting()
{
	glDisable(GL_LIGHTING);
	g_lighting_enabled = G_FALSE;
}
