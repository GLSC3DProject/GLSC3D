#include "glsc3d_private_ext.h"

static const size_t VERTEX_BUFFER_SIZE = 3 << 12;

#define BUFFER_OFFSET_NORMAL ((void *)sizeof(G_VECTOR))
#define BUFFER_OFFSET_COLOR  ((void *)(sizeof(G_VECTOR) * 2))

GLenum g_primitive_mode;

int g_vertex_data_count;

GLuint g_vertex_array_id, g_vertex_buffer_id;

G_VERTEX *g_vertex_data;

static const char * ErrorNames[] = {
	"GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION",
	"GL_STACK_OVERFLOW", "GL_STACK_UNDERFLOW", "GL_OUT_OF_MEMORY"
};

void g_check_opengl_error(int checkpoint)
{
	if (GLenum error = glGetError())
		printf("OpenGL Error : %s Checkpoint: %d\n",
			ErrorNames[error - GL_INVALID_ENUM], checkpoint);
}

void* g_malloc(size_t size)
{
	void* ptr = malloc(size);
	
	if (!ptr) {
		fprintf(stderr, "failed to allocate memory\a\n");
		fprintf(stderr, "GLSC3D will abort\n");
		g_quit();
	}
	
	return ptr;
}

void g_vertex_buffer_init()
{
	glGenVertexArrays(1, &g_vertex_array_id);
	glBindVertexArray(g_vertex_array_id);

	glGenBuffers(1, &g_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(G_VERTEX), NULL, GL_STREAM_DRAW);

#ifdef G_USE_CORE_PROFILE
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
#endif
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	g_vertex_data = (G_VERTEX *)g_malloc(VERTEX_BUFFER_SIZE * sizeof(G_VERTEX));
	g_vertex_data_count = 0;
}

void g_vertex_buffer_append(G_VERTEX vertex)
{
	g_vertex_data[g_vertex_data_count] = vertex;
	
	g_vertex_data_count++;
	
	if (g_vertex_data_count == VERTEX_BUFFER_SIZE)
		g_vertex_buffer_flush();
}

void g_vertex_buffer_append_position(G_VECTOR position)
{
	g_vertex_buffer_append(g_make_vertex(position, g_vector_zero, g_current_color));
}

void g_vertex_buffer_append_line(G_VECTOR a, G_VECTOR b)
{
	g_vertex_buffer_append(g_make_vertex(a, g_vector_zero, g_current_line_color));
	g_vertex_buffer_append(g_make_vertex(b, g_vector_zero, g_current_line_color));
}

void g_vertex_buffer_append_triangle_2D(G_VECTOR a, G_VECTOR b, G_VECTOR c)
{
	g_vertex_buffer_append(g_make_vertex(a, g_vector_zero, g_current_area_color_2D));
	g_vertex_buffer_append(g_make_vertex(b, g_vector_zero, g_current_area_color_2D));
	g_vertex_buffer_append(g_make_vertex(c, g_vector_zero, g_current_area_color_2D));
}

#ifdef G_USE_CORE_PROFILE

void g_vertex_buffer_flush()
{
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, g_vertex_data_count * sizeof(G_VERTEX), g_vertex_data);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), 0);
	
	if (g_lighting_enabled) {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_NORMAL);
	}
	
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(G_VERTEX), BUFFER_OFFSET_COLOR);
	
	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	g_vertex_data_count = 0;
}

#else

void g_vertex_buffer_flush()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, g_vertex_data_count * sizeof(G_VERTEX), g_vertex_data);
	
	glVertexPointer(3, GL_FLOAT, sizeof(G_VERTEX), 0);
	
	if (g_lighting_enabled) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		
		glNormalPointer(GL_FLOAT, sizeof(G_VERTEX), BUFFER_OFFSET_NORMAL);
	}
	
	glColorPointer(4, GL_FLOAT, sizeof(G_VERTEX), BUFFER_OFFSET_COLOR);
	
	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	if (g_lighting_enabled) {
		glDisable(GL_COLOR_MATERIAL);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	g_vertex_data_count = 0;
}

#endif

void g_set_primitive_mode(GLenum mode)
{
	if (g_primitive_mode != mode && g_vertex_data_count != 0)
		g_vertex_buffer_flush();
	
	g_primitive_mode = mode;
}

void g_begin_points()
{
	g_set_primitive_mode(GL_POINTS);
	
	g_disable_lighting();
}

void g_begin_lines()
{
	g_set_primitive_mode(GL_LINES);
	
	g_current_color = g_current_line_color;
	g_disable_lighting();
}

void g_begin_triangles()
{
	g_set_primitive_mode(GL_TRIANGLES);
	
	if (g_scale_dim_flag == G_3D) {
		g_current_color = g_current_area_color_3D;
		g_enable_lighting();
	} else {
		g_current_color = g_current_area_color_2D;
		g_disable_lighting();
	}
}
