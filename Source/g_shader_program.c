//
//  g_shader_program.c
//  GLSC3D
//
//  Created by student on 2017/02/07.
//  Copyright © 2017年 student. All rights reserved.
//

#include "glsc3d_private.h"

#define CONSTANT_VERT_SHADER_SOURCE \
	"#version 330 core\n" \
//	"uniform mat4 ModelView" \
//	"in vec3 Position;"

#define CONSTANT_FRAG_SHADER_SOURCE \
	""

G_BOOL g_lighting_enabled;

GLuint g_constant_program, g_lighting_program;

void g_assert_shader_compile_status(GLuint shader, const char *debug_name)
{
	GLint InfoLogLength, CompileStatus;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	
	if (InfoLogLength > 0) {
		char info_log[InfoLogLength];
		
		glGetShaderInfoLog(shader, InfoLogLength, &InfoLogLength, info_log);
		printf("In compiling %s:\n%s\n", debug_name, info_log);
		
		glGetShaderiv(shader, GL_COMPILE_STATUS, &CompileStatus);
		assert(CompileStatus);
	}
}

GLuint g_create_shader(GLuint program, GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);
	
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	g_assert_shader_compile_status(shader, source);
	
	glAttachShader(program, shader);
	
	return shader;
}

void g_shader_program_init()
{
	g_constant_program = glCreateProgram();
	
	g_create_shader(g_constant_program, GL_VERTEX_SHADER, CONSTANT_VERT_SHADER_SOURCE);
	g_create_shader(g_constant_program, GL_FRAGMENT_SHADER, CONSTANT_FRAG_SHADER_SOURCE);
	
	glLinkProgram(g_constant_program);
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
