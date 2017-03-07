#include "glsc3d_private_ext.h"
#include <vector>

G_BOOL g_lighting_enabled;

#ifdef G_USE_CORE_PROFILE

#define GLSL_VERSION_DECL "#version 410 core\n"

#define CONSTANT_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec4 in_position;\n" \
	"layout(location = 1) in vec4 in_normal;\n" \
	"layout(location = 2) in vec4 in_color;\n" \
	"layout(location = 0) out vec4 vary_color;\n" \
	"void main() {\n" \
	"	gl_Position = proj * (view * in_position);\n" \
	"	vary_color = in_color;\n" \
	"}"

#define CONSTANT_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec4 vary_color;\n" \
	"out vec4 out_color;\n" \
	"void main() { out_color = vary_color; }"

#define LIGHTING_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec4 in_position;\n" \
	"layout(location = 1) in vec4 in_normal;\n" \
	"layout(location = 2) in vec4 in_color;\n" \
	"layout(location = 0) out vec4 vary_color;\n" \
	"layout(location = 1) out vec4 vary_normal;\n" \
	"layout(location = 2) out vec4 vary_position;\n" \
	"void main() {\n" \
	"	vec4 view_pos = view * in_position;\n" \
	"	gl_Position = proj * view_pos;\n" \
	"	vary_color = in_color;\n" \
	"	vary_normal = view * in_normal;\n" \
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
	"	float amb_dif = light.ambient + light.diffuse * max(dot(light.direction.xyz, normal), 0);\n" \
	"	float spec = light.specular * pow(max(dot(normal, half_vec), 0), light.shininess);\n" \
	"	out_color = amb_dif * vary_color + spec;\n" \
	"}"

#define TEXTURE_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec2 in_texcoord;\n" \
	"layout(location = 0) out vec2 vary_texcoord;\n" \
	"void main() {\n" \
	"	gl_Position = vec4(in_texcoord * 2 - 1, 0, 1);\n" \
	"	vary_texcoord = in_texcoord;\n" \
	"}"

#define TEXTURE_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform sampler2D tex;\n" \
	"layout(location = 0) in vec2 vary_texcoord;\n" \
	"out vec4 out_color;\n" \
	"void main() {\n" \
	"	out_color = texture(tex, vary_texcoord);\n" \
	"//	out_color = vec4(vary_texcoord, 0, 1);\n" \
	"}"

GLuint g_constant_program, g_lighting_program, g_texture_program;
GLuint g_current_program;

GLint g_texture_location;

GLuint g_uniforms[G_NUM_UNIFORMS];

GLint g_get_shader_int(GLuint shader, GLenum pname)
{
	GLint param;
	glGetShaderiv(shader, pname, &param);
	return param;
}

GLint g_get_program_info(GLuint program, GLenum pname)
{
	GLint param;
	glGetProgramiv(program, pname, &param);
	return param;
}

void g_check_shader_compile_status(GLuint shader)
{
	GLint info_log_length = g_get_shader_int(shader, GL_INFO_LOG_LENGTH);
	
	if (info_log_length > 0) {
		std::vector<char> info_log((size_t)info_log_length);
		
		glGetShaderInfoLog(shader, info_log_length, NULL, info_log.data());
		printf("%s\n", info_log.data());

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
		std::vector<char> info_log((size_t)info_log_length);
		
		glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, NULL, info_log.data());
		printf("%s", info_log.data());

		if (g_get_program_info(program, GL_LINK_STATUS) == GL_FALSE)
			printf("Link Failed.\n");
	}
}

void g_bind_uniform_block(GLuint program, const GLchar *name, GLuint binding)
{
	GLuint uniform_block_index = glGetUniformBlockIndex(program, name);
	glUniformBlockBinding(program, uniform_block_index, binding);

//	printf("Uniform block index of %s : %d\n", name, uniform_block_index);
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

	g_texture_program = glCreateProgram();
	g_create_shader(g_texture_program, GL_VERTEX_SHADER, TEXTURE_VERT_SHADER_SOURCE);
	g_create_shader(g_texture_program, GL_FRAGMENT_SHADER, TEXTURE_FRAG_SHADER_SOURCE);
	g_link_program(g_texture_program);

	glGenBuffers(G_NUM_UNIFORMS, g_uniforms);

	g_texture_location = glGetUniformLocation(g_texture_program, "tex");
	//printf("location : 0x%X\n", g_texture_location);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_MATRICES]);
	g_bind_uniform_block(g_constant_program, "Matrices", G_UNIFORM_MATRICES);
	g_bind_uniform_block(g_lighting_program, "Matrices", G_UNIFORM_MATRICES);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_LIGHTS]);
	g_bind_uniform_block(g_lighting_program, "Lights", G_UNIFORM_LIGHTS);
}

void g_use_program(GLuint program)
{
	if (g_current_program != program) {
		glUseProgram(program);
		g_current_program = program;
	}
}

void g_enable_lighting()
{
	g_use_program(g_lighting_program);
	g_lighting_enabled = G_TRUE;
}

void g_disable_lighting()
{
	g_use_program(g_constant_program);
	g_lighting_enabled = G_FALSE;
}

void g_activate_texture_mode()
{
	g_vertex_buffer_flush();
	g_use_program(g_texture_program);

	glUniform1i(g_texture_location, 0);
}

#else

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
