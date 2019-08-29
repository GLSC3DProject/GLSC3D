#include "glsc3d_3_private.h"

#ifdef G_USE_CORE_PROFILE

#define GLSL_VERSION_DECL "#version 330 core\n"

#define MATRICES_UNIFORM_DECL \
"uniform Matrices { mat4 proj, view, view_normal; vec2 pixel_scale; float screen_scale; };"

// Vertex shader for rendering 2D triangles
const char * const CONSTANT_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
out VS_TO_FS {
	vec4 color;
} Output;
void main() {
	gl_Position = proj * (view * vec4(in_position, 1.0));
	Output.color = in_color;
})";

// Fragment shader for rendering 2D triangles
const char * const CONSTANT_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
in VS_TO_FS {
	vec4 color;
} Input;
out vec4 out_color;
void main() { out_color = Input.color; })";

// ----------------------------------------------------------------

// Vertex shader for rendering 3D triangles
const char * const LIGHTING_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec4 in_normal;
out VS_TO_FS {
	vec4 color, normal, position;
} Output;
void main() {
	vec4 view_pos = view * vec4(in_position, 1.0);
	gl_Position = proj * view_pos;
	Output.color = in_color;
	Output.normal = normalize(view_normal * vec4(in_normal.xyz, 0.0));
	Output.position = view_pos;
})";

// Fragment shader for rendering 3D triangles
const char * const LIGHTING_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
struct G_LIGHT { vec3 direction; float power; };
uniform Lights { G_LIGHT lights[3]; int num_lights; };
in VS_TO_FS {
	vec4 color, normal, position;
} Input;
out vec4 out_color;
vec3 calc_light(vec3 normal, G_LIGHT light) {
	vec3 half_vec = normalize(light.direction - normalize(Input.position.xyz));
	float amb_dif = light.power * max(dot(light.direction, normal), 0.0);
	float spec = light.power * pow(max(dot(normal, half_vec), 0.0), 64 /*shininess*/);
	return amb_dif * Input.color.rgb + spec;
}
void main() {
	vec3 normal = normalize(Input.normal.xyz);
	normal *= gl_FrontFacing ? 1.0 : -1.0;
	vec3 color = calc_light(normal, lights[0]);
	if (num_lights >= 2)
		color += calc_light(normal, lights[1]);
	if (num_lights >= 3)
		color += calc_light(normal, lights[2]);

//	vec3 color = calc_light(normal, lights[0]);
//	for(int i = 1;i < num_lights;i++)
//	{
//		color += calc_light(normal, lights[i]);
//	}

	out_color = vec4(color, Input.color.a);
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
	vec4 view_pos = view * vec4(in_position.xyz, 1.0);
	float size = in_position.w;
	gl_Position = proj * view_pos;
	gl_PointSize = size * screen_scale;
	Output.color = in_color;
	Output.position = view_pos.xyz;
	Output.radius = size * gl_Position.w * pixel_scale.y;
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
	vec4 view_pos = view * vec4(in_position.xyz, 1.0);
	float size = in_position.w;
	gl_Position = proj * view_pos;
	gl_PointSize = size * screen_scale / (pixel_scale.y * gl_Position.w);
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
	vec2 coord = gl_PointCoord * 2.0 - 1.0;
	float discriminant = 1.0 - dot(coord, coord);
	if (discriminant <= 0.0) discard;
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
	if (discriminant <= 0.0) discard;
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
	vec2 half_width;
} Output;
void main () {
	vec4 view_pos = view * vec4(in_position.xyz, 1.0);
	float size = in_position.w;
	gl_Position = proj * view_pos;
	Output.color = in_color;
	Output.position = view_pos.xyz;
	Output.half_width = size * pixel_scale;
})";

const char * const LINE_GEOMETRY_SHADER_SOURCE =
GLSL_VERSION_DECL MATRICES_UNIFORM_DECL R"(
layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;
in VS_TO_GS {
	vec4 color;
	vec3 position;
	vec2 half_width;
} Input[2];
out GS_TO_FS {
	vec4 color;
	noperspective float coord;
} Output;

void emit_vertices(uint id, vec2 r, float coord) {
	Output.color = Input[id].color;
	vec4 center = gl_in[id].gl_Position;
	vec4 offset = proj * vec4(Input[id].half_width * r * center.w, 0.0, 0.0);
	Output.coord = coord;

	gl_Position = center + offset;
	EmitVertex();

	gl_Position = center - offset;
	EmitVertex();
}
void main () {
	vec2 P = Input[0].position.xy / gl_in[0].gl_Position.w;
	vec2 Q = Input[1].position.xy / gl_in[1].gl_Position.w;
	vec2 r = normalize(vec2(P.y - Q.y, Q.x - P.x));

	// stipple coordinate: 128 = 4 [px/bit] * 8 [bit/period] * 2 * 2
	float c = length((P - Q) / pixel_scale) / 128.0;

	emit_vertices(0u, r, -c);
	emit_vertices(1u, r, +c);
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
	int i = int(fract(Input.coord) * 8.0); // stipple is 8-bit
	int a = (stipple >> i) & 1;
	if (a == 0) discard;
	out_color = vec4(Input.color.rgb, Input.color.a);
})";

// ----------------------------------------------------------------

// Vertex shader for rendering text
const char * const TEXTURE_VERT_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
layout(location = 0) in vec2 in_position;
out vec2 vary_texcoord;
void main() {
	gl_Position = vec4(in_position.xy, 0.0, 1.0);
	vary_texcoord = in_position * vec2(0.5, -0.5) + 0.5;
})";

// Fragment shader for rendering text
const char * const TEXTURE_FRAG_SHADER_SOURCE =
GLSL_VERSION_DECL R"(
uniform sampler2D tex;
uniform vec4 color;
in vec2 vary_texcoord;
out vec4 out_color;
void main() {
	out_color = vec4(color.rgb, color.a * texture(tex, vary_texcoord).r);
})";

#else // G_USE_CORE_PROFILE

#define GLSL_VERSION_DECL "#version 120"

// Vertex shader for rendering markers (size = diameter in standard coordinates)
const char * const MARKER_STANDARD_VERT_SHADER_SOURCE = GLSL_VERSION_DECL R"(
uniform float pixel_scale, screen_scale;
varying vec4 color;
varying vec3 position;
varying float radius;
void main () {
	vec4 view_pos = gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0);
	float size = gl_Vertex.w;
	gl_Position = gl_ProjectionMatrix * view_pos;
	gl_PointSize = size * screen_scale;
	color = gl_Color;
	position = gl_Vertex.xyz;
	radius = size * gl_Position.w * pixel_scale;
})";

// Vertex shader for rendering markers (size = radius in virtual coordinates)
const char * const MARKER_VIRTUAL_VERT_SHADER_SOURCE = GLSL_VERSION_DECL R"(
uniform float pixel_scale, screen_scale;
varying vec4 color;
varying vec3 position;
varying float radius;
void main () {
	vec4 view_pos = gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0);
	float size = gl_Vertex.w;
	gl_Position = gl_ProjectionMatrix * view_pos;
	gl_PointSize = size * screen_scale / (pixel_scale * gl_Position.w);
	color = gl_Color;
	position = view_pos.xyz;
	radius = size;
})";

// Fragment shader for rendering markers as 2D squares
const char * const MARKER_SQUARE_FRAG_SHADER_SOURCE = GLSL_VERSION_DECL R"(
varying vec4 color;
varying vec3 position;
varying float radius;
void main() {
	gl_FragColor = color;
})";

// Fragment shader for rendering markers as 2D circles
const char * const MARKER_CIRCLE_FRAG_SHADER_SOURCE = GLSL_VERSION_DECL R"(
varying vec4 color;
varying vec3 position;
varying float radius;
void main() {
	vec2 coord = gl_PointCoord * 2.0 - 1.0;
	float discriminant = 1.0 - dot(coord, coord);
	if (discriminant <= 0.0) discard;
	gl_FragColor = color;
})";

// Fragment shader for rendering markers as 3D spheres
const char * const MARKER_SPHERE_FRAG_SHADER_SOURCE = GLSL_VERSION_DECL R"(
varying vec4 color;
varying vec3 position;
varying float radius;
void main() {
	vec2 coord = gl_PointCoord * 2.0 - 1.0;
	float discriminant = 1.0 - dot(coord, coord);
	if (discriminant <= 0.0) discard;
	vec3 normal = vec3(coord, sqrt(discriminant));
	vec4 pos = gl_ProjectionMatrix * vec4(position + normal * radius, 1.0);
	gl_FragColor = vec4(color.rgb * normal.z, color.a);
	gl_FragDepth = pos.z / pos.w * 0.5 + 0.5;
})";

// ----------------------------------------------------------------

// Vertex shader for rendering text
const char * const TEXTURE_VERT_SHADER_SOURCE = GLSL_VERSION_DECL R"(
//layout(location = 0) in vec2 in_position;
varying vec2 vary_texcoord;
void main() {
	gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);
	vary_texcoord = gl_Vertex.xy * vec2(0.5, -0.5) + 0.5;
})";

// Fragment shader for rendering text
const char * const TEXTURE_FRAG_SHADER_SOURCE = GLSL_VERSION_DECL R"(
uniform sampler2D tex;
uniform vec4 color;
varying vec2 vary_texcoord;
//out vec4 out_color;
void main() {
	gl_FragColor = vec4(color.rgb, color.a * texture2D(tex, vary_texcoord).r);
})";

#endif // G_USE_CORE_PROFILE

GLuint g_marker_programs[G_NUM_MARKER_SIZE_TYPES][G_NUM_MARKER_TYPES];
GLuint g_texture_program;
GLuint g_current_program;

GLint g_texture_sampler_location;
GLint g_texture_color_location;

#ifdef G_USE_CORE_PROFILE
GLuint g_line_program;
GLuint g_constant_program;
GLuint g_lighting_program;

GLint g_line_stipple_location;

G_BOOL g_need_line_stipple_updated;

GLuint g_uniforms[G_NUM_UNIFORMS];
#else
GLint g_marker_pixel_scale_location[G_NUM_MARKER_SIZE_TYPES][G_NUM_MARKER_TYPES];
GLint g_marker_screen_scale_location[G_NUM_MARKER_SIZE_TYPES][G_NUM_MARKER_TYPES];
#endif

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
		char * info_log = (char *)malloc(info_log_length * sizeof(char));
		
		glGetShaderInfoLog(shader, info_log_length, NULL, info_log);
		printf("%s\n", info_log);

		free(info_log);

		if (g_get_shader_int(shader, GL_COMPILE_STATUS) == GL_FALSE)
			printf("Compile Failed.\n");
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
		char * info_log = (char *)malloc(info_log_length * sizeof(char));
		
		glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, NULL, info_log);
		printf("%s\n", info_log);

		free(info_log);

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

#ifdef G_USE_CORE_PROFILE

void g_bind_uniform_block(GLuint program, const GLchar *name, GLuint binding)
{
	GLuint uniform_block_index = glGetUniformBlockIndex(program, name);
	glUniformBlockBinding(program, uniform_block_index, binding);
}

void g_update_uniform(GLuint index, GLsizei size, const void *data)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, g_uniforms[index]);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

#endif

void g_shader_program_init()
{
#ifdef G_USE_CORE_PROFILE
	glGenBuffers(G_NUM_UNIFORMS, g_uniforms);
	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_MATRICES]);

	g_constant_program = g_create_program(CONSTANT_VERT_SHADER_SOURCE, CONSTANT_FRAG_SHADER_SOURCE);
	g_bind_uniform_block(g_constant_program, "Matrices", G_UNIFORM_MATRICES);

	g_lighting_program = g_create_program(LIGHTING_VERT_SHADER_SOURCE, LIGHTING_FRAG_SHADER_SOURCE);
	g_bind_uniform_block(g_lighting_program, "Matrices", G_UNIFORM_MATRICES);
#else
	float zero[] = { 0, 0, 0, 0 };
	float one[] = { 1, 1, 1, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, one);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
#endif

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

#ifdef G_USE_CORE_PROFILE
			g_bind_uniform_block(g_marker_programs[i][j], "Matrices", G_UNIFORM_MATRICES);
#else
			g_marker_pixel_scale_location[i][j] = glGetUniformLocation(g_marker_programs[i][j], "pixel_scale");
			g_marker_screen_scale_location[i][j] = glGetUniformLocation(g_marker_programs[i][j], "screen_scale");
#endif
		}
	}

#ifdef G_USE_CORE_PROFILE
	g_line_program = glCreateProgram();
	glAttachShader(g_line_program, g_create_shader(GL_VERTEX_SHADER, LINE_VERTEX_SHADER_SOURCE));
	glAttachShader(g_line_program, g_create_shader(GL_GEOMETRY_SHADER, LINE_GEOMETRY_SHADER_SOURCE));
	glAttachShader(g_line_program, g_create_shader(GL_FRAGMENT_SHADER, LINE_FRAGMENT_SHADER_SOURCE));
	g_link_program(g_line_program);
	g_bind_uniform_block(g_line_program, "Matrices", G_UNIFORM_MATRICES);
	g_line_stipple_location = glGetUniformLocation(g_line_program, "stipple");

	glBindBuffer(GL_UNIFORM_BUFFER, g_uniforms[G_UNIFORM_LIGHTS]);
	g_bind_uniform_block(g_lighting_program, "Lights", G_UNIFORM_LIGHTS);
#endif

	g_texture_program = g_create_program(TEXTURE_VERT_SHADER_SOURCE, TEXTURE_FRAG_SHADER_SOURCE);

	g_texture_sampler_location = glGetUniformLocation(g_texture_program, "tex");
	g_texture_color_location = glGetUniformLocation(g_texture_program, "color");
}

void g_use_program(GLuint program)
{
	if (g_current_program != program) {
		glUseProgram(program);
		g_current_program = program;
	}
}
