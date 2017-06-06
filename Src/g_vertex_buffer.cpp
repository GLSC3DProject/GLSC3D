#include "glsc3d_3_private.h"

static const size_t VERTEX_BUFFER_SIZE = 3 << 14;

static const G_VECTOR g_vector_zero(0, 0, 0);

GLenum g_primitive_mode;

float g_current_dummy_size = 1; // dummy

G_COLOR	*g_current_color_ptr;
float	*g_current_size_ptr;

int g_vertex_data_count;

G_VERTEX *g_vertex_data;

#define BUFFER_OFFSET_COLOR  ((void *)(sizeof(float) * 4))
#define BUFFER_OFFSET_NORMAL ((void *)(sizeof(float) * 8))

GLuint g_vertex_array_id, g_vertex_buffer_id;

bool g_inside_glbegin;

void g_vertex_buffer_init()
{
#ifdef G_USE_CORE_PROFILE
	g_vertex_data = (G_VERTEX *)malloc(VERTEX_BUFFER_SIZE * sizeof(G_VERTEX));

	if (g_vertex_data == nullptr) {
		fprintf(stderr, "failed to allocate memory\a\n");
		fprintf(stderr, "GLSC3D will abort\n");
		g_quit();
	}

	g_vertex_data_count = 0;

	glGenVertexArrays(1, &g_vertex_array_id);
	glBindVertexArray(g_vertex_array_id);

	glGenBuffers(1, &g_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(G_VERTEX), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
#endif
}

void g_vertex_buffer_append(G_VERTEX vertex)
{
#ifdef G_USE_CORE_PROFILE
	g_vertex_data[g_vertex_data_count] = vertex;

	g_vertex_data_count++;

	if (g_vertex_data_count == VERTEX_BUFFER_SIZE)
		g_vertex_buffer_flush();
#else
	glColor4fv((float *)&vertex.color);
	glNormal3fv((float *)&vertex.normal);
	glVertex3fv((float *)&vertex.position);
#endif
}

void g_emit_vertex(G_VECTOR position)
{
#ifdef G_USE_CORE_PROFILE
	g_vertex_buffer_append({position, *g_current_size_ptr, *g_current_color_ptr, g_vector_zero, 0});
#else
	glColor4fv((float *)g_current_color_ptr);
	glVertex4f(position.x, position.y, position.z, *g_current_size_ptr);
#endif
}

void g_emit_line(G_VECTOR p, G_VECTOR q)
{
	g_emit_vertex(p);
	g_emit_vertex(q);
}

void g_emit_triangle(G_VECTOR p, G_VECTOR q, G_VECTOR r)
{
	g_emit_vertex(p);
	g_emit_vertex(q);
	g_emit_vertex(r);
}

void g_vertex_buffer_flush()
{
#ifdef G_USE_CORE_PROFILE
	if (g_vertex_data_count == 0) return;

	glBindVertexArray(g_vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, g_vertex_data_count * sizeof(G_VERTEX), g_vertex_data);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_COLOR);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_NORMAL);

	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);

	g_vertex_data_count = 0;
#else
	if (g_inside_glbegin) {
		glEnd();
		g_inside_glbegin = false;
	}
#endif
}

typedef void (*g_prepare_func_type)(void);

void g_set_primitive_mode(GLenum mode, g_prepare_func_type prepare_func)
{
	if (g_primitive_mode != mode || !g_inside_glbegin) {
		g_vertex_buffer_flush();
		prepare_func();

#ifndef G_USE_CORE_PROFILE
		glBegin(mode);
#endif
		g_primitive_mode = mode;
		g_inside_glbegin = true;
	}
}

void g_prepare_points()
{
	g_current_color_ptr = &g_current_marker_color;
	g_current_size_ptr = &g_current_marker_size;
	g_use_program(g_marker_programs[g_current_marker_size_type][g_current_marker_type]);

#ifndef G_USE_CORE_PROFILE
	GLint pixel_scale_location = g_marker_pixel_scale_location[g_current_marker_size_type][g_current_marker_type];
	if (pixel_scale_location >= 0)
		glUniform1f(pixel_scale_location, g_current_pixel_scale);
#endif
}

void g_prepare_lines()
{
#ifdef G_USE_CORE_PROFILE
	if (g_current_program != g_line_program)
		g_need_line_stipple_updated = true;
#endif

	g_current_color_ptr = &g_current_line_color;

#ifdef G_USE_CORE_PROFILE
	g_current_size_ptr = &g_current_line_size;
	g_use_program(g_line_program);

	if (g_need_line_stipple_updated) {
		g_vertex_buffer_flush();
		glUniform1i(g_line_stipple_location, g_current_line_stipple);
		g_need_line_stipple_updated = false;
	}
#else
	g_current_size_ptr = &g_current_dummy_size;
	g_use_program(0);
	glDisable(GL_LIGHTING);
#endif
}

void g_prepare_triangles()
{
	if (g_scale_dim_flag == G_3D) {
		g_current_color_ptr = &g_current_area_color_3D;

#ifdef G_USE_CORE_PROFILE
		g_use_program(g_lighting_program);
#else
		if (!g_inside_glbegin) {
			g_use_program(0);
			glEnable(GL_LIGHTING);
		}
#endif
	} else {
		g_current_color_ptr = &g_current_area_color_2D;

#ifdef G_USE_CORE_PROFILE
		g_use_program(g_constant_program);
#else
		if (!g_inside_glbegin) {
			g_use_program(0);
			glDisable(GL_LIGHTING);
		}
#endif
	}
	g_current_size_ptr = &g_current_dummy_size;
}

void g_begin_points()
{
	g_set_primitive_mode(GL_POINTS, g_prepare_points);
}

void g_begin_lines()
{
	g_set_primitive_mode(GL_LINES, g_prepare_lines);
}

void g_begin_line_strip()
{
	g_vertex_buffer_flush();
	g_set_primitive_mode(GL_LINE_STRIP, g_prepare_lines);
}

void g_begin_line_loop()
{
	g_vertex_buffer_flush();
	g_set_primitive_mode(GL_LINE_LOOP, g_prepare_lines);
}

void g_begin_triangles()
{
	g_set_primitive_mode(GL_TRIANGLES, g_prepare_triangles);
}

void g_begin_triangle_fan()
{
	g_vertex_buffer_flush();
	g_set_primitive_mode(GL_TRIANGLE_FAN, g_prepare_triangles);
}
