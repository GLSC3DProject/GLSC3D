#include "glsc3d_3_private.h"

static const G_VECTOR g_vector_zero(0, 0, 0);

enum struct G_PRIMITIVE_MODE { POINT, LINE, TRIANGLE, UNDEFINED = -1 };
G_PRIMITIVE_MODE g_primitive_mode = G_PRIMITIVE_MODE::UNDEFINED;

#ifdef G_USE_CORE_PROFILE

#define BUFFER_OFFSET_COLOR  ((void *)(sizeof(float) * 4))
#define BUFFER_OFFSET_NORMAL ((void *)(sizeof(float) * 8))

class G_VERTEX_BUFFER
{
	G_VERTEX *data = nullptr;
	G_UINT size, count = 0;
	GLuint vertex_array_id = 0, vertex_buffer_id = 0;
	GLuint shader_program = 0;
	G_PRIMITIVE_MODE primitive_mode;

public:
	G_VERTEX_BUFFER(G_PRIMITIVE_MODE primitive_mode, G_UINT size) : primitive_mode(primitive_mode), size(size) {}

	void init()
	{
		data = (G_VERTEX *)malloc(size * sizeof(G_VERTEX));

		if (data == nullptr) {
			fprintf(stderr, "failed to allocate memory\a\n");
			fprintf(stderr, "GLSC3D will abort\n");
			g_quit();
		}

		glGenVertexArrays(1, &vertex_array_id);
		glBindVertexArray(vertex_array_id);

		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(G_VERTEX), NULL, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

	void set_shader_program(GLuint program) { shader_program = program; }

	bool is_empty() { return count == 0; }

	void append(const G_VERTEX &vertex)
	{
		if (data == nullptr) init();

		data[count] = vertex;
		count++;

		if (count == size)
			flush();
	}

	void flush()
	{
		if (count == 0) return;

		assert(g_primitive_mode != G_PRIMITIVE_MODE::UNDEFINED);

		g_use_program(shader_program);

		if (primitive_mode == G_PRIMITIVE_MODE::LINE) {
			glUniform1i(g_line_stipple_location, g_current_line_stipple);
		}

		glBindVertexArray(vertex_array_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(G_VERTEX), data);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_COLOR);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_NORMAL);

		GLenum opengl_primitive_modes[] = { GL_POINTS, GL_LINES, GL_TRIANGLES };

		glDrawArrays(opengl_primitive_modes[(int)primitive_mode], 0, count);

		count = 0;
	}
};

// all buffers can contain (1 << 14) primitives

G_VERTEX_BUFFER g_vertex_buffer_points(G_PRIMITIVE_MODE::POINT, 1 << 14);
G_VERTEX_BUFFER g_vertex_buffer_lines(G_PRIMITIVE_MODE::LINE, 2 << 14);
G_VERTEX_BUFFER g_vertex_buffer_triangles(G_PRIMITIVE_MODE::TRIANGLE, 3 << 14);

// actual depth value is given by (1.f - (float)g_current_2d_depth / G_2D_DEPTH_DIVISOR)
// must be greater than or equal to (3 << 14), (the number of buffers) * (the number of primitives per buffer)
// must be less than or equal to (1 << 24), the depth buffer is 24bit
#define G_2D_DEPTH_DIVISOR (1 << 16)

G_UINT g_current_2d_depth;

#endif

// for 3D triangle
void g_vertex_buffer_append(const G_VERTEX &vertex)
{
#ifdef G_USE_CORE_PROFILE
	//assert(g_primitive_mode == G_PRIMITIVE_MODE::TRIANGLE);
	g_vertex_buffer_triangles.append(vertex);
#else
	glColor4fv((float *)&vertex.color);
	glNormal3fv((float *)&vertex.normal);
	glVertex3fv((float *)&vertex.position);
#endif
}

// for point, line and 2D triangle
void g_emit_vertex(const G_VECTOR &position)
{
#ifdef G_USE_CORE_PROFILE
	G_VERTEX vertex;
	vertex.position = position;

	if (!g_current_scale_ptr->is_3D)
		vertex.position.z = 1.f - (float)g_current_2d_depth / G_2D_DEPTH_DIVISOR;

	vertex.normal = g_vector_zero;
	vertex.pad = 0;

	switch (g_primitive_mode) {
	case G_PRIMITIVE_MODE::POINT:
		vertex.size = g_current_marker_size;
		vertex.color = g_current_marker_color;
		g_vertex_buffer_points.append(vertex);
		break;
	case G_PRIMITIVE_MODE::LINE:
		vertex.size = g_current_line_size;
		vertex.color = g_current_line_color;
		g_vertex_buffer_lines.append(vertex);
		break;
	case G_PRIMITIVE_MODE::TRIANGLE:
		vertex.size = 1;
		vertex.color = g_current_area_color;
		g_vertex_buffer_triangles.append(vertex);
		break;
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

#ifdef G_USE_CORE_PROFILE
	g_current_2d_depth++;
#endif
}

void g_emit_line(G_VECTOR p, G_VECTOR q)
{
	g_emit_vertex(p);
	g_emit_vertex(q);

#ifdef G_USE_CORE_PROFILE
	g_current_2d_depth++;
#endif
}

void g_emit_triangle(G_VECTOR p, G_VECTOR q, G_VECTOR r)
{
	g_emit_vertex(p);
	g_emit_vertex(q);
	g_emit_vertex(r);

#ifdef G_USE_CORE_PROFILE
	g_current_2d_depth++;
#endif
}

void g_vertex_buffer_flush()
{
#ifdef G_USE_CORE_PROFILE
	if (g_vertex_buffer_points.is_empty() &&
		g_vertex_buffer_lines.is_empty() &&
		g_vertex_buffer_triangles.is_empty()) return;

	g_vertex_buffer_triangles.flush();
	g_vertex_buffer_lines.flush();
	g_vertex_buffer_points.flush();

	g_current_2d_depth = 0;

	if (g_current_scale_ptr != nullptr && !g_current_scale_ptr->is_3D) {
		G_SCREEN viewport = g_current_scale_ptr->viewport();
		glEnable(GL_SCISSOR_TEST);
		glScissor(viewport.x, viewport.y, viewport.width, viewport.height);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);
	}
#else
	if (g_primitive_mode != G_PRIMITIVE_MODE::UNDEFINED) {
		glEnd();
	}
#endif
	g_primitive_mode = G_PRIMITIVE_MODE::UNDEFINED;
}

void g_prepare_points()
{
#ifdef G_USE_CORE_PROFILE
	if (!g_current_scale_ptr->is_3D && g_current_marker_type == G_MARKER_SPHERE) {
		printf("Sphere marker cannot be used in 2D\n");
		g_quit();
	}
	g_vertex_buffer_points.set_shader_program(g_marker_programs[g_current_marker_size_type][g_current_marker_type]);
#else
	g_use_program(g_marker_programs[g_current_marker_size_type][g_current_marker_type]);
	GLint pixel_scale_location = g_marker_pixel_scale_location[g_current_marker_size_type][g_current_marker_type];
	if (pixel_scale_location >= 0)
		glUniform1f(pixel_scale_location, g_current_pixel_scale);
	GLint screen_scale_location = g_marker_screen_scale_location[g_current_marker_size_type][g_current_marker_type];
	if (screen_scale_location >= 0)
		glUniform1f(screen_scale_location, g_screen_scale_factor);
#endif
}

void g_prepare_lines()
{
#ifdef G_USE_CORE_PROFILE
	g_vertex_buffer_lines.set_shader_program(g_line_program);
#else
	g_use_program(0);
	glDisable(GL_LIGHTING);
#endif
}

void g_prepare_triangles()
{
#ifdef G_USE_CORE_PROFILE
	if (g_current_scale_ptr->is_3D) {
		g_vertex_buffer_triangles.set_shader_program(g_lighting_program);
	} else {
		g_vertex_buffer_triangles.set_shader_program(g_constant_program);
	}
#else
	g_use_program(0);
	if (g_current_scale_ptr->is_3D) {
		glEnable(GL_LIGHTING);
	} else {
		glDisable(GL_LIGHTING);
	}
#endif
}

void g_set_primitive_mode(G_PRIMITIVE_MODE mode)
{
	if (g_current_scale_ptr == nullptr) {
		printf("Call g_sel_scale before drawing\n");
		g_quit();
	}

	if (g_primitive_mode == mode) return;

#ifdef G_USE_CORE_PROFILE
	switch (mode) {
	case G_PRIMITIVE_MODE::POINT:
		g_prepare_points();
		break;
	case G_PRIMITIVE_MODE::LINE:
		g_prepare_lines();
		break;
	case G_PRIMITIVE_MODE::TRIANGLE:
		g_prepare_triangles();
		break;
	}
#else
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
