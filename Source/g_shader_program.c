#include "glsc3d_private_ext.h"

G_BOOL g_lighting_enabled;

#ifdef G_USE_CORE_PROFILE

#define GLSL_VERSION_DECL "#version 410 core\n"

#define CONSTANT_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec3 in_position;\n" \
	"layout(location = 1) in vec3 in_normal;\n" \
	"layout(location = 2) in vec4 in_color;\n" \
	"layout(location = 0) out vec4 vary_color;\n" \
	"void main() { gl_Position = proj * (view * vec4(in_position, 1)); vary_color = in_color; }"

#define CONSTANT_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec4 vary_color;\n" \
	"out vec4 out_color;\n" \
	"void main() { out_color = vary_color; }"

#define LIGHTING_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec3 in_position;\n" \
	"layout(location = 1) in vec3 in_normal;\n" \
	"layout(location = 2) in vec4 in_color;\n" \
	"layout(location = 0) out vec4 vary_color;\n" \
	"layout(location = 1) out vec4 vary_normal;\n" \
	"layout(location = 2) out vec4 vary_position;\n" \
	"void main() {\n" \
	"	vec4 view_pos = view * vec4(in_position, 1);\n" \
	"	gl_Position = proj * view_pos;\n" \
	"	vary_color = in_color;\n" \
	"	vary_normal = view * vec4(in_normal, 0);\n" \
	"	vary_position = view_pos;\n" \
	"}"

#define LIGHTING_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Lights { vec4 direction; float ambient, diffuse, specular, shininess; } light;\n" \
	"layout(location = 0) in vec4 vary_color;\n" \
	"layout(location = 1) in vec4 vary_normal;\n" \
	"layout(location = 2) in vec4 vary_position;\n" \
	"out vec4 out_color;\n" \
	"void main() {\n" \
	"	vec3 half_vec = normalize(light.direction.xyz - normalize(vary_position.xyz));\n" \
	"	vec3 normal = normalize(vary_normal.xyz);\n" \
	"	out_color" \
	"		= (light.ambient + light.diffuse * max(dot(light.direction.xyz, normal), 0)) * vary_color" \
	"		+ light.specular * pow(max(dot(normal, half_vec), 0), light.shininess);\n" \
	"}"

GLuint g_constant_program, g_lighting_program;

GLuint g_uniforms[G_NUM_UNIFORMS];

GLint g_get_shader_int(GLuint shader, GLenum pname)
{
	GLint ans;
	glGetShaderiv(shader, pname, &ans);
	return ans;
}

GLint g_get_program_info(GLuint program, GLenum pname)
{
	GLint ans;
	glGetProgramiv(program, pname, &ans);
	return ans;
}

void g_check_shader_compile_status(GLuint shader)
{
	GLint info_log_length = g_get_shader_int(shader, GL_INFO_LOG_LENGTH);
	
	if (info_log_length > 0) {
		char *info_log = malloc(info_log_length);
		
		glGetShaderInfoLog(shader, info_log_length, NULL, info_log);
		printf("%s\n", info_log);

		free(info_log);
		
		if (g_get_shader_int(shader, GL_COMPILE_STATUS) == GL_FALSE)
			printf("Compile Failed.\n");

//		assert(CompileStatus);
	}
}

void g_create_shader(GLuint program, GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);
	
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	g_check_shader_compile_status(shader);
	
	glAttachShader(program, shader);
}

void g_link_program(GLuint program)
{
	glLinkProgram(program);
	
	GLint info_log_length = g_get_program_info(program, GL_INFO_LOG_LENGTH);
	
	if (info_log_length > 0) {
		char *info_log = malloc(info_log_length);
		
		glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, NULL, info_log);
		printf("%s", info_log);
		
		free(info_log);
		
		if (g_get_program_info(program, GL_LINK_STATUS) == GL_FALSE)
			printf("Link Failed.\n");
	}
}

void g_bind_uniform_block(GLuint program, const GLchar *name, GLuint binding)
{
	GLuint uniform_block_index = glGetUniformBlockIndex(program, name);
	glUniformBlockBinding(program, uniform_block_index, binding);

	printf("Uniform block index of %s : %d\n", name, uniform_block_index);
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
	g_link_program(g_constant_program);

	g_lighting_program = glCreateProgram();
	g_create_shader(g_lighting_program, GL_VERTEX_SHADER, LIGHTING_VERT_SHADER_SOURCE);
	g_create_shader(g_lighting_program, GL_FRAGMENT_SHADER, LIGHTING_FRAG_SHADER_SOURCE);
	g_link_program(g_lighting_program);

	glGenBuffers(G_NUM_UNIFORMS, g_uniforms);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_MATRICES]);
	g_bind_uniform_block(g_constant_program, "Matrices", G_UNIFORM_MATRICES);
	g_bind_uniform_block(g_lighting_program, "Matrices", G_UNIFORM_MATRICES);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_LIGHTS]);
	g_bind_uniform_block(g_lighting_program, "Lights", G_UNIFORM_LIGHTS);
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
