//
//  g_shader_program.c
//  GLSC3D
//
//  Created by student on 2017/02/07.
//  Copyright © 2017年 student. All rights reserved.
//

#include "glsc3d_private_ext.h"

G_BOOL g_lighting_enabled;

#ifdef G_USE_CORE_PROFILE

#define GLSL_VERSION_DECL "#version 410 core\n"

#define CONSTANT_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform MatrixBlock { mat4 Proj, View; };\n" \
	"layout(location = 0) in vec3 Position;\n" \
	"layout(location = 1) in vec3 Normal;\n" \
	"layout(location = 2) in vec3 Color;\n" \
	"layout(location = 0) out vec3 OutputColor;\n" \
	"void main() { gl_Position = Proj * (View * vec4(Position, 1)); OutputColor = Color; }"

#define CONSTANT_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec3 Color;\n" \
	"out vec3 OutputColor;\n" \
	"void main() { OutputColor = Color; }"

#define LIGHTING_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform MatrixBlock { mat4 Proj, View; };\n" \
	"layout(location = 0) in vec3 Position;\n" \
	"layout(location = 1) in vec3 Normal;\n" \
	"layout(location = 2) in vec4 Color;\n" \
	"layout(location = 0) out vec4 OutputColor;\n" \
	"layout(location = 1) out vec4 OutputNormal;\n" \
	"layout(location = 2) out vec4 OutputPosition;\n" \
	"void main() {\n" \
	"	vec4 ViewPos = View * vec4(Position, 1);\n" \
	"	gl_Position = Proj * ViewPos;\n" \
	"	OutputColor = Color;\n" \
	"	OutputNormal = View * vec4(Normal, 0);\n" \
	"	OutputPosition = ViewPos;\n" \
	"}"

#define LIGHTING_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform LightBlock { vec4 LightPos; float Ambient, Diffuse, Specular, Shininess; };\n" \
	"layout(location = 0) in vec4 Color;\n" \
	"layout(location = 1) in vec3 Normal;\n" \
	"layout(location = 2) in vec3 Position;\n" \
	"out vec4 OutputColor;\n" \
	"void main() {\n" \
	"	vec3 Half = normalize(LightPos.xyz - Position);\n" \
	"	vec3 Normal = normalize(Normal);\n" \
	"//	Normal = faceforward(Normal, Position, Normal);\n" \
	"	OutputColor" \
	"		= (Ambient + Diffuse * max(dot(LightPos.xyz, Normal), 0)) * Color" \
	"		+ Specular * pow(max(dot(Normal, Half), 0), Shininess);\n" \
	"}"

GLuint g_constant_program, g_lighting_program;

GLuint g_uniforms[G_NUM_UNIFORMS];

void g_assert_shader_compile_status(GLuint shader)
{
	GLint InfoLogLength, CompileStatus;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	
	if (InfoLogLength > 0) {
		char *info_log = malloc(InfoLogLength);
		
		glGetShaderInfoLog(shader, InfoLogLength, &InfoLogLength, info_log);
		printf("%s\n", info_log);

		free(info_log);
		
		glGetShaderiv(shader, GL_COMPILE_STATUS, &CompileStatus);
		if (CompileStatus == GL_FALSE) printf("Compile Failed.\n");

		assert(CompileStatus);
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

void g_bind_uniform_block(GLuint program, const GLchar *name, GLuint binding)
{
	GLuint uniform_block_index = glGetUniformBlockIndex(program, name);
	glUniformBlockBinding(program, uniform_block_index, binding);
}

void g_update_uniform(GLuint index, GLsizei size, GLvoid *data)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, g_uniforms[index]);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void g_shader_program_init()
{
	g_constant_program = glCreateProgram();
	g_create_shader(g_constant_program, GL_VERTEX_SHADER, CONSTANT_VERT_SHADER_SOURCE);
	g_create_shader(g_constant_program, GL_FRAGMENT_SHADER, CONSTANT_FRAG_SHADER_SOURCE);
	glLinkProgram(g_constant_program);

	g_lighting_program = glCreateProgram();
	g_create_shader(g_lighting_program, GL_VERTEX_SHADER, LIGHTING_VERT_SHADER_SOURCE);
	g_create_shader(g_lighting_program, GL_FRAGMENT_SHADER, LIGHTING_FRAG_SHADER_SOURCE);
	glLinkProgram(g_lighting_program);

	glGenBuffers(G_NUM_UNIFORMS, g_uniforms);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_MATRICES]);
	g_bind_uniform_block(g_constant_program, "MatrixBlock", G_UNIFORM_MATRICES);
	g_bind_uniform_block(g_lighting_program, "MatrixBlock", G_UNIFORM_MATRICES);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_LIGHTS]);
	g_bind_uniform_block(g_lighting_program, "LightBlock", G_UNIFORM_LIGHTS);
}

void g_enable_lighting()
{
	if (g_lighting_enabled == G_TRUE) return;

	glUseProgram(g_lighting_program);
	g_lighting_enabled = G_TRUE;
}

void g_disable_lighting()
{
	if (g_lighting_enabled == G_FALSE) return;

	glUseProgram(g_constant_program);
	g_lighting_enabled = G_FALSE;
}

#else

void g_shader_program_init()
{
}

void g_set_camera(G_CAMERA *c)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float *)&c->proj);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((float *)&c->view);
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

#endif
