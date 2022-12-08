#include "glsc3d_3_private.h"

static const G_VECTOR g_vector_zero(0, 0, 0);

enum struct G_PRIMITIVE_MODE { POINT, LINE, TRIANGLE, UNDEFINED = -1 };
G_PRIMITIVE_MODE g_primitive_mode = G_PRIMITIVE_MODE::UNDEFINED;

struct G_FLOAT4 {
	G_VECTOR xyz;
	float w;
};

#ifdef G_USE_VERTEX_BUFFERS

//#define G_USE_METAL_BUFFER

#if defined(G_USE_METAL) && !defined(G_USE_METAL_BUFFER)
#define G_MAX_VERTEX_COUNT (1 << 10)
#else
#define G_MAX_VERTEX_COUNT (1 << 16)
#endif

// Maximum supported number of 2D primitives: (1 << 24)
// Actual 2D depth is calculated by 1.f - (float)g_current_2d_depth / G_2D_DEPTH_DIVISOR
#ifdef G_USE_METAL
#define G_2D_DEPTH_DIVISOR (1 << 24) // Valid depth range is [ 0, 1]
#else
#define G_2D_DEPTH_DIVISOR (1 << 23) // Valid depth range is [-1, 1]
#endif

#ifdef G_USE_METAL

#ifdef G_USE_METAL_BUFFER
MTL::Buffer *g_vertex_buffers[3];
#endif

MTL::PrimitiveType g_metal_primitive_modes[] = {
	MTL::PrimitiveTypePoint,
	MTL::PrimitiveTypeLine,
	MTL::PrimitiveTypeTriangle,
};

void g_vertex_buffer_init()
{
#ifdef G_USE_METAL_BUFFER
	for (auto &buffer : g_vertex_buffers)
		buffer = g_device->newBuffer(sizeof(G_FLOAT4) * G_MAX_VERTEX_COUNT, MTL::StorageModeManaged);
#endif
}

#else

#define BUFFER_OFFSET_COLOR  ((void *)(sizeof(float) * 4))
#define BUFFER_OFFSET_NORMAL ((void *)(sizeof(float) * 8))

GLuint g_vertex_array_id;
GLuint g_vertex_buffer_ids[3];

GLenum g_opengl_primitive_modes[] = { GL_POINTS, GL_LINES, GL_TRIANGLES };

void g_vertex_buffer_init()
{
	glGenVertexArrays(1, &g_vertex_array_id);
	glBindVertexArray(g_vertex_array_id);

	glGenBuffers(3, g_vertex_buffer_ids);
	for (GLuint vb_id : g_vertex_buffer_ids) {
		glBindBuffer(GL_ARRAY_BUFFER, vb_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(G_VECTOR4) * G_MAX_VERTEX_COUNT, nullptr, GL_STREAM_DRAW);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);
}

#endif

G_PROGRAM_TYPE g_get_shader_program(G_PRIMITIVE_MODE mode)
{
	switch (mode) {
	case G_PRIMITIVE_MODE::POINT:
		return g_marker_programs[g_current_marker_size_type][g_current_marker_type];
	case G_PRIMITIVE_MODE::LINE:
		return g_line_program;
	case G_PRIMITIVE_MODE::TRIANGLE:
		return g_current_scale_ptr->is_3D ? g_lighting_program : g_constant_program;
	default:
		return g_constant_program;
	}
}

class G_VERTEX_BUFFER
{
	G_PRIMITIVE_MODE primitive_mode;
	G_UINT capacity;
	G_FLOAT4 *positions;
	G_COLOR *colors;
	G_FLOAT4 *normals;
	G_UINT count = 0;

public:
	G_VERTEX_BUFFER(G_PRIMITIVE_MODE primitive_mode, G_UINT capacity)
	: primitive_mode(primitive_mode), capacity(capacity)
	{
		positions = g_malloc<G_FLOAT4>(capacity);
		colors = g_malloc<G_COLOR>(capacity);

		if (primitive_mode == G_PRIMITIVE_MODE::TRIANGLE)
			normals = g_malloc<G_FLOAT4>(capacity);
	}

	bool is_empty() { return count == 0; }

	void append(G_VECTOR position, float size, G_COLOR color)
	{
		positions[count] = {position, size};
		colors[count] = color;
		count++;

		if (count == capacity)
			g_vertex_buffer_flush();
	}

	void append(const G_VERTEX &vertex)
	{
		normals[count] = {vertex.normal, 0};
		append(vertex.position, vertex.size, vertex.color);
	}

	void flush()
	{
		if (count == 0) return;

		G_PROGRAM_TYPE program = g_get_shader_program(primitive_mode);
		g_use_program(program);

#ifdef G_USE_METAL
		size_t size = sizeof(float) * 4 * count;

#ifdef G_USE_METAL_BUFFER
		memcpy(g_vertex_buffers[0]->contents(), positions, size);
		memcpy(g_vertex_buffers[1]->contents(), colors, size);
		g_vertex_buffers[0]->didModifyRange(NS::Range(0, size));
		g_vertex_buffers[1]->didModifyRange(NS::Range(0, size));
		g_command_encoder->setVertexBuffer(g_vertex_buffers[0], 0, 0);
		g_command_encoder->setVertexBuffer(g_vertex_buffers[1], 0, 1);
		g_current_scale_ptr->select();

		if (primitive_mode == G_PRIMITIVE_MODE::TRIANGLE && g_current_scale_ptr->is_3D) {
			memcpy(g_vertex_buffers[2]->contents(), normals, size);
			g_vertex_buffers[2]->didModifyRange(NS::Range(0, size));
			g_command_encoder->setVertexBuffer(g_vertex_buffers[2], 0, 2);
			g_update_enabled_lights();
		}
#else
		g_command_encoder->setVertexBytes(positions, size, G_BUFFER_POSITIONS);
		g_command_encoder->setVertexBytes(colors, size, G_BUFFER_COLORS);

		if (primitive_mode == G_PRIMITIVE_MODE::TRIANGLE && g_current_scale_ptr->is_3D) {
			g_command_encoder->setVertexBytes(normals, size, G_BUFFER_NORMALS);
		}
#endif

		if (program == g_line_program) {
			g_command_encoder->setFragmentBytes(&g_current_line_stipple, sizeof(G_UCHAR), G_BUFFER_STIPPLE);
			g_command_encoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, 0UL, 4UL, count >> 1);
		}
		else {
			g_command_encoder->drawPrimitives(g_metal_primitive_modes[(int)primitive_mode], 0UL, count);
		}
#else
		if (program == g_line_program)
			glUniform1ui(g_line_stipple_location, g_current_line_stipple);

		glBindVertexArray(g_vertex_array_id);

		glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ids[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(G_FLOAT4), positions);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(G_FLOAT4), nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ids[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(G_COLOR), colors);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(G_COLOR), nullptr);

		if (primitive_mode == G_PRIMITIVE_MODE::TRIANGLE && g_current_scale_ptr->is_3D) {
			glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ids[2]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(G_FLOAT4), normals);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(G_FLOAT4), nullptr);
			glEnableVertexAttribArray(2);
		}
		else {
			glDisableVertexAttribArray(2);
		}

		glDrawArrays(g_opengl_primitive_modes[(int)primitive_mode], 0, count);
#endif

		count = 0;
	}
};

G_VERTEX_BUFFER g_vertex_buffer_points(G_PRIMITIVE_MODE::POINT, G_MAX_VERTEX_COUNT);
G_VERTEX_BUFFER g_vertex_buffer_lines(G_PRIMITIVE_MODE::LINE, G_MAX_VERTEX_COUNT);
G_VERTEX_BUFFER g_vertex_buffer_triangles(G_PRIMITIVE_MODE::TRIANGLE, G_MAX_VERTEX_COUNT / 3 * 3);

G_UINT g_current_2d_depth;

#endif

// for 3D triangle
void g_vertex_buffer_append(const G_VERTEX &vertex)
{
#ifdef G_USE_VERTEX_BUFFERS
	//assert(g_primitive_mode == G_PRIMITIVE_MODE::TRIANGLE);
	g_vertex_buffer_triangles.append(vertex);
#else
	glColor4fv((float *)&vertex.color);
	glNormal3fv((float *)&vertex.normal);
	glVertex3fv((float *)&vertex.position);
#endif
}

// for point, line and 2D triangle
void g_emit_vertex(G_VECTOR position)
{
#ifdef G_USE_VERTEX_BUFFERS
	if (!g_current_scale_ptr->is_3D)
		position.z = 1.f - (float)g_current_2d_depth / G_2D_DEPTH_DIVISOR;

	switch (g_primitive_mode) {
	case G_PRIMITIVE_MODE::POINT:
		g_vertex_buffer_points.append(position, g_current_marker_size, g_current_marker_color);
		break;
	case G_PRIMITIVE_MODE::LINE:
		g_vertex_buffer_lines.append(position, g_current_line_size, g_current_line_color);
		break;
	case G_PRIMITIVE_MODE::TRIANGLE:
		g_vertex_buffer_triangles.append(position, 1, g_current_area_color);
		break;
	case G_PRIMITIVE_MODE::UNDEFINED:
		fprintf(stderr, "Internal error: g_primitive_mode is UNDEFINED\n");
		g_quit();
	}
#else
	switch (g_primitive_mode) {
	case G_PRIMITIVE_MODE::POINT:
		glColor4fv((float *)&g_current_marker_color);
		glVertex4f(position.x, position.y, position.z, g_current_marker_size);
		break;
	case G_PRIMITIVE_MODE::LINE:
		glColor4fv((float *)&g_current_line_color);
		glVertex3fv((float *)&position);
		break;
	case G_PRIMITIVE_MODE::TRIANGLE:
		glColor4fv((float *)&g_current_area_color);
		glVertex3fv((float *)&position);
		break;
	}
#endif
}

void g_emit_point(G_VECTOR p)
{
	g_emit_vertex(p);

#ifdef G_USE_VERTEX_BUFFERS
	g_current_2d_depth++;
#endif
}

void g_emit_line(G_VECTOR p, G_VECTOR q)
{
	g_emit_vertex(p);
	g_emit_vertex(q);

#ifdef G_USE_VERTEX_BUFFERS
	g_current_2d_depth++;
#endif
}

void g_emit_triangle(G_VECTOR p, G_VECTOR q, G_VECTOR r)
{
	g_emit_vertex(p);
	g_emit_vertex(q);
	g_emit_vertex(r);

#ifdef G_USE_VERTEX_BUFFERS
	g_current_2d_depth++;
#endif
}

void g_vertex_buffer_flush()
{
#ifdef G_USE_VERTEX_BUFFERS
	if (g_vertex_buffer_points.is_empty()) {
		if (g_vertex_buffer_lines.is_empty() && g_vertex_buffer_triangles.is_empty()) return;
	}
	else if (g_current_marker_type == G_MARKER_SPHERE && !g_current_scale_ptr->is_3D) {
		printf("Sphere marker cannot be used in 2D\n");
		g_quit();
	}

	if (g_current_scale_ptr == nullptr) {
		printf("Select a scale before drawing\n");
		g_quit();
	}

	g_vertex_buffer_triangles.flush();
	g_vertex_buffer_lines.flush();
	g_vertex_buffer_points.flush();

//	g_current_2d_depth = 0;

//	if (!g_current_scale_ptr->is_3D) {
//		G_SCREEN viewport = g_current_scale_ptr->viewport();
//		glEnable(GL_SCISSOR_TEST);
//		glScissor(viewport.x, viewport.y, viewport.width, viewport.height);
//		glClear(GL_DEPTH_BUFFER_BIT);
//		glDisable(GL_SCISSOR_TEST);
//	}
#else
	if (g_primitive_mode != G_PRIMITIVE_MODE::UNDEFINED) {
		glEnd();
		g_primitive_mode = G_PRIMITIVE_MODE::UNDEFINED;
	}
#endif
}

void g_reset_primitive_mode(void)
{
	g_primitive_mode = G_PRIMITIVE_MODE::UNDEFINED;
}

#ifndef G_USE_VERTEX_BUFFERS

void g_prepare_points()
{
	g_use_program(g_marker_programs[g_current_marker_size_type][g_current_marker_type]);
	GLint pixel_scale_location = g_marker_pixel_scale_location[g_current_marker_size_type][g_current_marker_type];
	if (pixel_scale_location >= 0)
		glUniform1f(pixel_scale_location, g_current_pixel_scale);
	GLint screen_scale_location = g_marker_screen_scale_location[g_current_marker_size_type][g_current_marker_type];
	if (screen_scale_location >= 0)
		glUniform1f(screen_scale_location, g_screen_scale_factor);
}

void g_prepare_lines()
{
	g_use_program(0);
	glDisable(GL_LIGHTING);
}

void g_prepare_triangles()
{
	g_use_program(0);
	if (g_current_scale_ptr->is_3D) {
		glEnable(GL_LIGHTING);
	} else {
		glDisable(GL_LIGHTING);
	}
}

#endif

void g_set_primitive_mode(G_PRIMITIVE_MODE mode)
{
	if (g_primitive_mode == mode) return;

	if (g_current_scale_ptr == nullptr) {
		printf("Call g_sel_scale before drawing\n");
		g_quit();
	}

#ifndef G_USE_VERTEX_BUFFERS
	g_vertex_buffer_flush();
	switch (mode) {
	case G_PRIMITIVE_MODE::POINT:
		g_prepare_points();
		glBegin(GL_POINTS);
		break;
	case G_PRIMITIVE_MODE::LINE:
		g_prepare_lines();
		glBegin(GL_LINES);
		break;
	case G_PRIMITIVE_MODE::TRIANGLE:
		g_prepare_triangles();
		glBegin(GL_TRIANGLES);
		break;
	}
#endif

	g_primitive_mode = mode;
}

void g_begin_points()
{
	g_set_primitive_mode(G_PRIMITIVE_MODE::POINT);
}

void g_begin_lines()
{
	g_set_primitive_mode(G_PRIMITIVE_MODE::LINE);
}

void g_begin_triangles()
{
	g_set_primitive_mode(G_PRIMITIVE_MODE::TRIANGLE);
}
