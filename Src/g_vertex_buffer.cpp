#include "glsc3d_private.h"

static const size_t VERTEX_BUFFER_SIZE = 3 << 17;

static const G_VECTOR g_vector_zero(0, 0, 0);

GLenum g_primitive_mode;

G_COLOR	*g_current_color;
float	g_current_size;

int g_vertex_data_count;

G_VERTEX *g_vertex_data;

#ifdef G_USE_CORE_PROFILE

#define BUFFER_OFFSET_COLOR  ((void *)(sizeof(float) * 4))
#define BUFFER_OFFSET_NORMAL ((void *)(sizeof(float) * 8))

GLuint g_vertex_array_id, g_vertex_buffer_id;

#endif

void g_vertex_buffer_init()
{
	g_vertex_data = (G_VERTEX *)malloc(VERTEX_BUFFER_SIZE * sizeof(G_VERTEX));

	if (g_vertex_data == nullptr) {
		fprintf(stderr, "failed to allocate memory\a\n");
		fprintf(stderr, "GLSC3D will abort\n");
		g_quit();
	}

	g_vertex_data_count = 0;

#ifdef G_USE_CORE_PROFILE
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
	g_vertex_data[g_vertex_data_count] = vertex;
	
	g_vertex_data_count++;
	
	if (g_vertex_data_count == VERTEX_BUFFER_SIZE)
		g_vertex_buffer_flush();
}

void g_emit_vertex(G_VECTOR position)
{
	g_vertex_buffer_append({position, g_current_size, *g_current_color, g_vector_zero, 0});
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
	if (g_vertex_data_count == 0) return;

#ifdef G_USE_CORE_PROFILE

	glBindVertexArray(g_vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, g_vertex_data_count * sizeof(G_VERTEX), g_vertex_data);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_COLOR);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_NORMAL);
	
	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);

#else

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(G_VERTEX), &g_vertex_data->position);

	if (g_lighting_enabled) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		glNormalPointer(GL_FLOAT, sizeof(G_VERTEX), &g_vertex_data->normal);
	}

	glColorPointer(4, GL_FLOAT, sizeof(G_VERTEX), &g_vertex_data->color);

	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);

	if (g_lighting_enabled) {
		glDisable(GL_COLOR_MATERIAL);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

#endif

	g_vertex_data_count = 0;
}

void g_set_primitive_mode(GLenum mode, bool always_flush)
{
	if (g_primitive_mode != mode || always_flush)
		g_vertex_buffer_flush();
	
	g_primitive_mode = mode;
}

void g_prepare_points()
{
	g_current_color = &g_current_marker_color;
	g_current_size = g_current_marker_size;
	g_use_program(g_marker_programs[g_current_marker_size_type][g_current_marker_type]);
}

void g_prepare_lines()
{
	if (g_current_program != g_line_program)
		g_need_line_stipple_updated = true;

	g_current_color = &g_current_line_color;
	g_current_size = g_current_line_size;
	g_use_program(g_line_program);

	if (g_need_line_stipple_updated) {
		g_vertex_buffer_flush();
		glUniform1i(g_line_stipple_location, g_current_line_stipple);
		g_need_line_stipple_updated = false;
	}
}

void g_prepare_triangles()
{
	if (g_scale_dim_flag == G_3D) {
		g_current_color = &g_current_area_color_3D;
		g_use_program(g_lighting_program);
	} else {
		g_current_color = &g_current_area_color_2D;
		g_use_program(g_constant_program);
	}
	g_current_size = 0;
}

void g_begin_points()
{
	g_set_primitive_mode(GL_POINTS, false);
	g_prepare_points();
}

void g_begin_lines()
{
	g_set_primitive_mode(GL_LINES, false);
	g_prepare_lines();
}

void g_begin_line_strip()
{
	g_set_primitive_mode(GL_LINE_STRIP, true);
	g_prepare_lines();
}

void g_begin_line_loop()
{
	g_set_primitive_mode(GL_LINE_LOOP, true);
	g_prepare_lines();
}

void g_begin_triangles()
{
	g_set_primitive_mode(GL_TRIANGLES, false);
	g_prepare_triangles();
}

void g_begin_triangle_strip()
{
	g_set_primitive_mode(GL_TRIANGLE_STRIP, true);
	g_prepare_triangles();
}

void g_begin_triangle_fan()
{
	g_set_primitive_mode(GL_TRIANGLE_FAN, true);
	g_prepare_triangles();
}
