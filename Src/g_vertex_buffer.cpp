#include <glsc3d_3_private.h>
#include "glsc3d_3_private.h"

static const size_t VERTEX_BUFFER_SIZE = 3 << 14;

static const G_VECTOR g_vector_zero(0, 0, 0);

GLenum g_primitive_mode;

G_COLOR	*g_current_color;
float	g_current_size;

int g_vertex_data_count;

G_VERTEX *g_vertex_data;

#define BUFFER_OFFSET_COLOR  ((void *)(sizeof(float) * 4))
#define BUFFER_OFFSET_NORMAL ((void *)(sizeof(float) * 8))

GLuint g_vertex_array_id, g_vertex_buffer_id;

bool g_inside_glbegin;

void g_vertex_buffer_init()
{
//	g_vertex_data = (G_VERTEX *)malloc(VERTEX_BUFFER_SIZE * sizeof(G_VERTEX));
//
//	if (g_vertex_data == nullptr) {
//		fprintf(stderr, "failed to allocate memory\a\n");
//		fprintf(stderr, "GLSC3D will abort\n");
//		g_quit();
//	}
//
//	g_vertex_data_count = 0;
//
//	glGenVertexArrays(1, &g_vertex_array_id);
//	glBindVertexArray(g_vertex_array_id);
//
//	glGenBuffers(1, &g_vertex_buffer_id);
//	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
//	glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(G_VERTEX), NULL, GL_STREAM_DRAW);

//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);

//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
}

void g_vertex_buffer_append(G_VERTEX vertex)
{
//	g_vertex_data[g_vertex_data_count] = vertex;
//
//	g_vertex_data_count++;
//
//	if (g_vertex_data_count == VERTEX_BUFFER_SIZE)
//		g_vertex_buffer_flush();
	glColor4fv((float *)&vertex.color);
	glNormal3fv((float *)&vertex.normal);
	glVertex3fv((float *)&vertex.position);
}

void g_emit_vertex(G_VECTOR position)
{
//	g_vertex_buffer_append({position, g_current_size, *g_current_color, g_vector_zero, 0});
	glColor4fv((float *)g_current_color);
//	glNormal3fv((float *)&vertex.normal);
//	glVertex3fv((float *)&position);
	glVertex4f(position.x, position.y, position.z, g_current_size);
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
//	if (g_vertex_data_count == 0) return;
//
//	glBindVertexArray(g_vertex_array_id);
//	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, g_vertex_data_count * sizeof(G_VERTEX), g_vertex_data);
//
//	glVertexPointer(4, GL_FLOAT, sizeof(G_VERTEX), 0);
//	glColorPointer(4, GL_FLOAT, sizeof(G_VERTEX), BUFFER_OFFSET_COLOR);
//	glNormalPointer(GL_FLOAT, sizeof(G_VERTEX), BUFFER_OFFSET_NORMAL);
//
//	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);
//
//	g_vertex_data_count = 0;
	if (g_inside_glbegin) {
		glEnd();
		g_inside_glbegin = false;
	}
}

typedef void (*g_prepare_func_type)(void);

void g_set_primitive_mode(GLenum mode, bool always_flush, g_prepare_func_type prepare_func)
{
	if (g_primitive_mode != mode || always_flush || !g_inside_glbegin) {
		g_vertex_buffer_flush();
		prepare_func();
		glBegin(mode);
		g_primitive_mode = mode;
		g_inside_glbegin = true;
	}
}

extern float g_current_pixel_scale;

void g_prepare_points()
{
	g_current_color = &g_current_marker_color;
	g_current_size = g_current_marker_size;
	GLuint program = g_marker_programs[g_current_marker_size_type][g_current_marker_type];
	if (program != g_current_program) {
		g_use_program(program);
//		glUniform1f(g_marker_pixel_scale_location[g_current_marker_size_type][g_current_marker_type], g_current_pixel_scale);
		glUniform1f(0, g_current_pixel_scale);
	}
//	g_use_program(g_marker_programs[g_current_marker_size_type][g_current_marker_type]);
}

void g_prepare_lines()
{
//	if (g_current_program != g_line_program)
//		g_need_line_stipple_updated = true;

	g_current_color = &g_current_line_color;
	g_current_size = 1;
//	g_use_program(g_constant_program);
	g_use_program(0);

//	if (g_need_line_stipple_updated) {
//		g_vertex_buffer_flush();
//		glUniform1i(g_line_stipple_location, g_current_line_stipple);
//		g_need_line_stipple_updated = false;
//	}
}

void g_prepare_triangles()
{
	if (g_scale_dim_flag == G_3D) {
		g_current_color = &g_current_area_color_3D;
//		g_use_program(g_lighting_program);
		if (!g_inside_glbegin) {
			g_use_program(g_lighting_program);
		}
	} else {
		g_current_color = &g_current_area_color_2D;
//		g_use_program(g_constant_program);
		if (!g_inside_glbegin) {
			g_use_program(0);
		}
	}
	g_current_size = 1;
}

void g_begin_points()
{
//	g_prepare_points();
	g_set_primitive_mode(GL_POINTS, false, g_prepare_points);
}

void g_begin_lines()
{
//	g_prepare_lines();
	g_set_primitive_mode(GL_LINES, false, g_prepare_lines);
}

void g_begin_line_strip()
{
//	g_prepare_lines();
	g_set_primitive_mode(GL_LINE_STRIP, true, g_prepare_lines);
}

void g_begin_line_loop()
{
//	g_prepare_lines();
	g_set_primitive_mode(GL_LINE_LOOP, true, g_prepare_lines);
}

void g_begin_triangles()
{
//	g_prepare_triangles();
	g_set_primitive_mode(GL_TRIANGLES, false, g_prepare_triangles);
}

void g_begin_triangle_strip()
{
//	g_set_primitive_mode(GL_TRIANGLE_STRIP, true);
//	g_prepare_triangles();
}

void g_begin_triangle_fan()
{
//	g_prepare_triangles();
	g_set_primitive_mode(GL_TRIANGLE_FAN, true, g_prepare_triangles);
}
