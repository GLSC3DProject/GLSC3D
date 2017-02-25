//
//  g_vertex_buffer.c
//  GLSC3D
//
//  Created by student on 2017/01/31.
//  Copyright © 2017年 student. All rights reserved.
//

#include "glsc3d_private.h"

static const size_t VERTEX_BUFFER_SIZE = 3 << 12;

#define BUFFER_OFFSET_NORMAL ((void *)sizeof(G_VECTOR))
#define BUFFER_OFFSET_COLOR  ((void *)(sizeof(G_VECTOR) * 2))

GLenum g_primitive_mode;

int g_vertex_data_count;

GLuint g_vertex_buffer_id;

G_VERTEX *g_vertex_data;

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
	glGenBuffers(1, &g_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE * sizeof(G_VERTEX), NULL, GL_STREAM_DRAW);
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
	
	g_disable_lighting();
}

void g_begin_triangles()
{
	g_set_primitive_mode(GL_TRIANGLES);
	
	if (g_scale_dim_flag == G_3D) {
		g_enable_lighting();
	} else {
		g_disable_lighting();
	}
}
