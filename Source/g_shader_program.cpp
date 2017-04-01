#include "glsc3d_private.h"
#include <vector>

#ifdef G_USE_CORE_PROFILE

#define GLSL_VERSION_DECL "#version 410 core\n"

#define MATRICES_UNIFORM_DECL \
"uniform Matrices { mat4 proj, view; float pixel_scale, screen_scale; vec2 screen_size; };"

// Vertex shader for rendering lines and 2D triangles
const char * const CONSTANT_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 0) out vec4 vary_color;
void main() {
	gl_Position = proj * (view * vec4(in_position, 1));
	vary_color = in_color;
})";

// Fragment shader for rendering lines and 2D triangles
const char * const CONSTANT_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
layout(location = 0) in vec4 vary_color;
out vec4 out_color;
void main() { out_color = vary_color; })";

// ----------------------------------------------------------------

// Vertex shader for rendering 3D triangles
const char * const LIGHTING_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec4 in_normal;
layout(location = 0) out vec4 vary_color;
layout(location = 1) out vec4 vary_normal;
layout(location = 2) out vec4 vary_position;
void main() {
	vec4 view_pos = view * vec4(in_position, 1);
	gl_Position = proj * view_pos;
	vary_color = in_color;
	vary_normal = view * vec4(in_normal.xyz, 0);
	vary_position = view_pos;
})";

// Fragment shader for rendering 3D triangles
const char * const LIGHTING_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
struct G_LIGHT { vec3 direction; float pad; float ambient, diffuse, specular, shininess; };
uniform Lights { G_LIGHT lights[3]; int num_lights; };
layout(location = 0) in vec4 vary_color;
layout(location = 1) in vec4 vary_normal;
layout(location = 2) in vec4 vary_position;
out vec4 out_color;
vec3 calc_light(vec3 normal, G_LIGHT light) {
	vec3 half_vec = normalize(light.direction - normalize(vary_position.xyz));
	float amb_dif = light.diffuse * max(dot(light.direction, normal), 0);
	float spec = light.specular * pow(max(dot(normal, half_vec), 0), light.shininess);
	return amb_dif * vary_color.rgb + spec;
}
void main() {
	vec3 normal = normalize(vary_normal.xyz);
	normal *= gl_FrontFacing ? 1.0 : -1.0;

	vec3 color = vec3(0.0);
	for (int i = 0; i < num_lights; i++)
		color += calc_light(normal, lights[i]);

	out_color = vec4(color, vary_color.a);
})";

// ----------------------------------------------------------------

// Vertex shader for rendering markers (size = diameter in standard coordinates)
const char * const MARKER_STANDARD_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
out VS_TO_FS {
	vec4 color;
	vec3 position;
	float radius;
} Output;
void main () {
	vec4 view_pos = view * vec4(in_position.xyz, 1);
	float size = in_position.w;
	gl_Position = proj * view_pos;
	gl_PointSize = size * screen_scale;
	Output.color = in_color;
	Output.position = view_pos.xyz;
	Output.radius = size * gl_Position.w / pixel_scale;
})";

// Vertex shader for rendering markers (size = radius in virtual coordinates)
const char * const MARKER_VIRTUAL_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
out VS_TO_FS {
	vec4 color;
	vec3 position;
	float radius;
} Output;
void main () {
	vec4 view_pos = view * vec4(in_position.xyz, 1);
	float size = in_position.w;
	gl_Position = proj * view_pos;
	gl_PointSize = size * screen_scale * pixel_scale / gl_Position.w;
	Output.color = in_color;
	Output.position = view_pos.xyz;
	Output.radius = size;
})";

// Fragment shader for rendering markers as 2D squares
const char * const MARKER_SQUARE_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
in VS_TO_FS {
	vec4 color;
	vec3 position;
	float radius;
} Input;
out vec4 out_color;
void main() {
	out_color = Input.color;
})";

// Fragment shader for rendering markers as 2D circles
const char * const MARKER_CIRCLE_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
in VS_TO_FS {
	vec4 color;
	vec3 position;
	float radius;
} Input;
out vec4 out_color;
void main() {
	vec2 coord = gl_PointCoord * 2 - 1;
	float discriminant = 1 - dot(coord, coord);
	if (discriminant <= 0) discard;
	out_color = Input.color;
})";

// Fragment shader for rendering markers as 3D spheres
const char * const MARKER_SPHERE_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
in VS_TO_FS {
	vec4 color;
	vec3 position;
	float radius;
} Input;
out vec4 out_color;
void main() {
	vec2 coord = gl_PointCoord * 2 - 1;
	float discriminant = 1 - dot(coord, coord);
	if (discriminant <= 0) discard;
	vec3 normal = vec3(coord, sqrt(discriminant));
	vec4 pos = proj * vec4(Input.position + normal * Input.radius, 1);
	out_color = vec4(Input.color.rgb * normal.z, Input.color.a);
	gl_FragDepth = pos.z / pos.w * 0.5 + 0.5;
})";

// ----------------------------------------------------------------

const char * const LINE_VERTEX_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;
out VS_TO_GS {
	vec4 color;
	vec3 position;
	float half_width;
} Output;
void main () {
	vec4 view_pos = view * vec4(in_position.xyz, 1);
	float size = in_position.w;
	gl_Position = proj * view_pos;
	Output.color = in_color;
	Output.position = view_pos.xyz;
	Output.half_width = size * gl_Position.w / pixel_scale;
})";

const char * const LINE_GEOMETRY_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;
in VS_TO_GS {
	vec4 color;
	vec3 position;
	float half_width;
} Input[2];
out GS_TO_FS {
	vec4 color;
	noperspective float coord;
} Output;

void emit_vertices(uint id, vec4 r, float coord) {
	Output.color = Input[id].color;
	vec4 center = gl_in[id].gl_Position;
	vec4 offset = proj * (Input[id].half_width * r);
	Output.coord = coord;

	gl_Position = center + offset;
	EmitVertex();

	gl_Position = center - offset;
	EmitVertex();
}
void main () {
	vec3 p = Input[0].position;
	vec3 q = Input[1].position;
	vec4 v = vec4(p.y - q.y, q.x - p.x, 0, 0);
	vec4 r = normalize(v);
	float c = length(p - q) * 4;
	emit_vertices(0, r, 0);
	emit_vertices(1, r, c);
})";

const char * const LINE_FRAGMENT_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
uniform int stipple;
in GS_TO_FS {
	vec4 color;
	noperspective float coord;
} Input;
out vec4 out_color;
void main() {
	int i = int(fract(Input.coord) * 8);
	int a = (stipple >> i) & 1;
	out_color = vec4(Input.color.rgb, Input.color.a * float(a));
})";

// ----------------------------------------------------------------

// Vertex shader for rendering text
const char * const TEXTURE_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
layout(location = 0) in vec2 in_position;
layout(location = 0) out vec2 vary_texcoord;
void main() {
	gl_Position = vec4(in_position.xy, 0, 1);
	vary_texcoord = in_position * vec2(0.5, -0.5) + 0.5;
})";

// Fragment shader for rendering text
const char * const TEXTURE_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
uniform sampler2D tex;
uniform vec4 color;
layout(location = 0) in vec2 vary_texcoord;
out vec4 out_color;
void main() {
	out_color = vec4(color.rgb, color.a * texture(tex, vary_texcoord).r);
})";

GLuint g_constant_program, g_lighting_program;
GLuint g_marker_programs[G_NUM_MARKER_SIZE_TYPES][G_NUM_MARKER_TYPES];
GLuint g_line_program;
GLuint g_texture_program;
GLuint g_current_program;

GLint g_line_stipple_location;
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
		printf("%s\n", info_log.data());

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

GLuint g_create_program(GLuint vert_shader, GLuint frag_shader)
{
	GLuint program = glCreateProgram();

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
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
	glGenBuffers(G_NUM_UNIFORMS, g_uniforms);
	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_MATRICES]);

	g_constant_program = g_create_program(CONSTANT_VERT_SHADER_SOURCE, CONSTANT_FRAG_SHADER_SOURCE);
	g_bind_uniform_block(g_constant_program, "Matrices", G_UNIFORM_MATRICES);

	g_lighting_program = g_create_program(LIGHTING_VERT_SHADER_SOURCE, LIGHTING_FRAG_SHADER_SOURCE);
	g_bind_uniform_block(g_lighting_program, "Matrices", G_UNIFORM_MATRICES);

	GLuint marker_vert_shaders[] = {
		g_create_shader(GL_VERTEX_SHADER, MARKER_STANDARD_VERT_SHADER_SOURCE),
		g_create_shader(GL_VERTEX_SHADER, MARKER_VIRTUAL_VERT_SHADER_SOURCE),
	};
	GLuint marker_frag_shaders[] = {
		g_create_shader(GL_FRAGMENT_SHADER, MARKER_SQUARE_FRAG_SHADER_SOURCE),
		g_create_shader(GL_FRAGMENT_SHADER, MARKER_CIRCLE_FRAG_SHADER_SOURCE),
		g_create_shader(GL_FRAGMENT_SHADER, MARKER_SPHERE_FRAG_SHADER_SOURCE),
	};

	for (GLuint i = 0; i < G_NUM_MARKER_SIZE_TYPES; i++) {
		for (GLuint j = 0; j < G_NUM_MARKER_TYPES; j++) {
			g_marker_programs[i][j] = g_create_program(marker_vert_shaders[i], marker_frag_shaders[j]);
			g_bind_uniform_block(g_marker_programs[i][j], "Matrices", G_UNIFORM_MATRICES);
		}
	}

	g_line_program = glCreateProgram();
	glAttachShader(g_line_program, g_create_shader(GL_VERTEX_SHADER, LINE_VERTEX_SHADER_SOURCE));
	glAttachShader(g_line_program, g_create_shader(GL_GEOMETRY_SHADER, LINE_GEOMETRY_SHADER_SOURCE));
	glAttachShader(g_line_program, g_create_shader(GL_FRAGMENT_SHADER, LINE_FRAGMENT_SHADER_SOURCE));
	g_link_program(g_line_program);
	g_bind_uniform_block(g_line_program, "Matrices", G_UNIFORM_MATRICES);
	g_line_stipple_location = glGetUniformLocation(g_line_program, "stipple");

	g_texture_program = g_create_program(TEXTURE_VERT_SHADER_SOURCE, TEXTURE_FRAG_SHADER_SOURCE);

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_LIGHTS]);
	g_bind_uniform_block(g_lighting_program, "Lights", G_UNIFORM_LIGHTS);

	g_texture_sampler_location = glGetUniformLocation(g_texture_program, "tex");
	g_texture_color_location = glGetUniformLocation(g_texture_program, "color");
}

void g_update_uniform(GLuint index, GLsizei size, const void *data)
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
