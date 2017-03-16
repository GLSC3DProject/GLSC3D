#include "glsc3d_private.h"
#include <vector>

#ifdef G_USE_CORE_PROFILE

#define GLSL_VERSION_DECL "#version 410 core\n"

// Vertex shader for rendering lines and 2D triangles
#define CONSTANT_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec3 in_position;\n" \
	"layout(location = 1) in vec4 in_color;\n" \
	"layout(location = 0) out vec4 vary_color;\n" \
	"void main() {\n" \
	"	gl_Position = proj * (view * vec4(in_position, 1));\n" \
	"	vary_color = in_color;\n" \
	"}"

// Fragment shader for rendering lines and 2D triangles
#define CONSTANT_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec4 vary_color;\n" \
	"out vec4 out_color;\n" \
	"void main() { out_color = vary_color; }"

// Vertex shader for rendering 3D triangles
#define LIGHTING_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec3 in_position;\n" \
	"layout(location = 1) in vec4 in_color;\n" \
	"layout(location = 2) in vec4 in_normal;\n" \
	"layout(location = 0) out vec4 vary_color;\n" \
	"layout(location = 1) out vec4 vary_normal;\n" \
	"layout(location = 2) out vec4 vary_position;\n" \
	"void main() {\n" \
	"	vec4 view_pos = view * vec4(in_position, 1);\n" \
	"	gl_Position = proj * view_pos;\n" \
	"	vary_color = in_color;\n" \
	"	vary_normal = view * vec4(in_normal.xyz, 0);\n" \
	"	vary_position = view_pos;\n" \
	"}"

// Fragment shader for rendering 3D triangles
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
	"	normal *= gl_FrontFacing ? 1.0 : -1.0;\n" \
	"	float amb_dif = light.ambient + light.diffuse * max(dot(light.direction.xyz, normal), 0);\n" \
	"	float spec = light.specular * pow(max(dot(normal, half_vec), 0), light.shininess);\n" \
	"	out_color = vec4(amb_dif * vary_color.rgb + spec, vary_color.a);\n" \
	"}"

// Vertex shader for rendering markers
#define MARKER_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec4 in_position;\n" \
	"layout(location = 1) in vec4 in_color;\n" \
	"layout(location = 0) out vec4 vary_color;\n" \
	"layout(location = 1) out vec4 vary_position;\n" \
	"void main () {\n" \
	"	vec4 view_pos = view * vec4(in_position.xyz, 1);\n" \
	"	gl_Position = proj * view_pos;\n" \
	"	gl_PointSize = in_position.w / -view_pos.z * 8;\n" \
	"	vary_color = in_color;\n" \
	"	vary_position = vec4(view_pos.xyz, in_position.w);\n" \
	"}"

// Fragment shader for rendering markers as 2D squares
#define MARKER_SQUARE_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec4 vary_color;\n" \
	"out vec4 out_color;\n" \
	"void main() { out_color = vary_color; }"

// Fragment shader for rendering markers as 2D circles
#define MARKER_CIRCLE_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec4 vary_color;\n" \
	"out vec4 out_color;\n" \
	"void main() {\n" \
	"	vec2 coord = gl_PointCoord * 2 - 1;\n" \
	"	out_color = vec4(vary_color.rgb, vary_color.a * step(dot(coord, coord), 1));\n" \
	"}"

// Fragment shader for rendering markers as 3D spheres
#define MARKER_SPHERE_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform Matrices { mat4 proj, view; };\n" \
	"layout(location = 0) in vec4 vary_color;\n" \
	"layout(location = 1) in vec4 vary_position;\n" \
	"out vec4 out_color;\n" \
	"void main() {\n" \
	"	vec2 coord = gl_PointCoord * 2 - 1;\n" \
	"	float discriminant = 1 - dot(coord, coord);\n" \
	"	if (discriminant <= 0) discard;\n" \
	"	vec3 normal = vec3(coord, sqrt(discriminant));\n" \
	"	vec4 pos = proj * vec4(vary_position.xyz + normal * vary_position.w, 1);\n" \
	"//	out_color = vec4(vary_color.rgb * normal.z, vary_color.a);\n" \
	"	out_color = vec4(pos.z / pos.w - 2.5, 0, 0, 1);\n" \
	"//	out_color = vec4(gl_FragCoord.w, 0, 0, 1);\n" \
	"//	float depth = dot(proj[2], pos) / dot(proj[3], pos) * 0.5 + 0.5;\n" \
	"//	float depth = pos.z / pos.w * gl_FragCoord.w;\n" \
	"//	gl_FragDepth = 0.95;\n" \
	"//	gl_FragDepth = discriminant <= 0 ? 1 : gl_FragCoord.z;\n" \
	"}"

// Vertex shader for rendering text
#define TEXTURE_VERT_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"layout(location = 0) in vec2 in_texcoord;\n" \
	"layout(location = 0) out vec2 vary_texcoord;\n" \
	"void main() {\n" \
	"	gl_Position = vec4(in_texcoord.x * 2 - 1, in_texcoord.y * -2 + 1, 0, 1);\n" \
	"	vary_texcoord = in_texcoord;\n" \
	"}"

// Fragment shader for rendering text
#define TEXTURE_FRAG_SHADER_SOURCE \
	GLSL_VERSION_DECL \
	"uniform sampler2D tex;\n" \
	"uniform vec4 color;\n" \
	"layout(location = 0) in vec2 vary_texcoord;\n" \
	"out vec4 out_color;\n" \
	"void main() {\n" \
	"	out_color = vec4(color.rgb, color.a * texture(tex, vary_texcoord).r);\n" \
	"}"

GLuint g_constant_program, g_lighting_program;
GLuint g_marker_programs[G_NUM_MARKER_TYPES];
GLuint g_texture_program;
GLuint g_current_program;

GLint g_texture_sampler_location, g_texture_color_location;

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

GLuint g_create_shader(GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	g_check_shader_compile_status(shader);

	return shader;
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

GLuint g_create_program(const char *vert_shader_source, const char *frag_shader_source)
{
	GLuint program = glCreateProgram();

	glAttachShader(program, g_create_shader(GL_VERTEX_SHADER, vert_shader_source));
	glAttachShader(program, g_create_shader(GL_FRAGMENT_SHADER, frag_shader_source));
	g_link_program(program);

	return program;
}

GLuint g_create_program(GLuint vert_shader, const char *frag_shader_source)
{
	GLuint program = glCreateProgram();

	glAttachShader(program, vert_shader);
	glAttachShader(program, g_create_shader(GL_FRAGMENT_SHADER, frag_shader_source));
	g_link_program(program);

	return program;
}

void g_bind_uniform_block(GLuint program, const GLchar *name, GLuint binding)
{
	GLuint uniform_block_index = glGetUniformBlockIndex(program, name);
	glUniformBlockBinding(program, uniform_block_index, binding);
}

void g_shader_program_init()
{
	g_constant_program = g_create_program(CONSTANT_VERT_SHADER_SOURCE, CONSTANT_FRAG_SHADER_SOURCE);

	g_lighting_program = g_create_program(LIGHTING_VERT_SHADER_SOURCE, LIGHTING_FRAG_SHADER_SOURCE);

	GLuint marker_vert_shader = g_create_shader(GL_VERTEX_SHADER, MARKER_VERT_SHADER_SOURCE);
	g_marker_programs[G_MARKER_SQUARE] = g_create_program(marker_vert_shader, MARKER_SQUARE_FRAG_SHADER_SOURCE);
	g_marker_programs[G_MARKER_CIRCLE] = g_create_program(marker_vert_shader, MARKER_CIRCLE_FRAG_SHADER_SOURCE);
	g_marker_programs[G_MARKER_SPHERE] = g_create_program(marker_vert_shader, MARKER_SPHERE_FRAG_SHADER_SOURCE);

	g_texture_program = g_create_program(TEXTURE_VERT_SHADER_SOURCE, TEXTURE_FRAG_SHADER_SOURCE);

	glGenBuffers(G_NUM_UNIFORMS, g_uniforms);

	g_texture_sampler_location = glGetUniformLocation(g_texture_program, "tex");
	g_texture_color_location = glGetUniformLocation(g_texture_program, "color");

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_MATRICES]);
	g_bind_uniform_block(g_constant_program, "Matrices", G_UNIFORM_MATRICES);
	g_bind_uniform_block(g_lighting_program, "Matrices", G_UNIFORM_MATRICES);
	g_bind_uniform_block(g_marker_programs[0], "Matrices", G_UNIFORM_MATRICES);
	g_bind_uniform_block(g_marker_programs[1], "Matrices", G_UNIFORM_MATRICES);
	g_bind_uniform_block(g_marker_programs[2], "Matrices", G_UNIFORM_MATRICES);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_LIGHTS]);
	g_bind_uniform_block(g_lighting_program, "Lights", G_UNIFORM_LIGHTS);
}

void g_update_uniform(GLuint index, GLsizei size, GLvoid *data)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, g_uniforms[index]);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void g_use_program(GLuint program)
{
	if (g_current_program != program) {
		g_vertex_buffer_flush();

		glUseProgram(program);
		g_current_program = program;
	}
}

#else

G_BOOL g_lighting_enabled;

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
