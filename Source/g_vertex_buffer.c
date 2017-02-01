//
//  g_vertex_buffer.c
//  GLSC3D
//
//  Created by student on 2017/01/31.
//  Copyright © 2017年 student. All rights reserved.
//

#include <stdio.h>
#include "glsc3d_private.h"

//#define VERTEX_BUFFER_SIZE (3 << 10)
static const size_t VERTEX_BUFFER_SIZE = 3 << 5;

GLenum g_primitive_mode;
G_COLOR current_color;

int g_vertex_data_count;
G_VECTOR_F *g_vertex_data_position;
G_COLOR *g_vertex_data_color;

void* g_malloc(size_t size)
{
	void* ptr = malloc(size);
	
	if (!ptr) {
		fprintf(stderr, "failed to allocate memory\a\n");
		fprintf(stderr, "GLSC3D will abort\n");
		abort();
	}
	
	return ptr;
}

void g_vertex_buffer_init()
{
	g_vertex_data_position = (G_VECTOR_F *)g_malloc(VERTEX_BUFFER_SIZE * sizeof(G_VECTOR_F));
	g_vertex_data_color = (G_COLOR *)g_malloc(VERTEX_BUFFER_SIZE * sizeof(G_COLOR));
	
	g_vertex_data_count = 0;
}

void g_vertex_buffer_append(G_VECTOR_F vertex)
{
	g_vertex_data_position[g_vertex_data_count] = vertex;
	g_vertex_data_color[g_vertex_data_count] = current_color;
	
	g_vertex_data_count++;
	
	if (g_vertex_data_count == VERTEX_BUFFER_SIZE)
		g_vertex_buffer_draw();
}

void g_vertex_buffer_append_line(G_VECTOR_F a, G_VECTOR_F b)
{
	g_vertex_buffer_append(a);
	g_vertex_buffer_append(b);
}

void g_vertex_buffer_append_triangle(G_VECTOR_F a, G_VECTOR_F b, G_VECTOR_F c)
{
	g_vertex_buffer_append(a);
	g_vertex_buffer_append(b);
	g_vertex_buffer_append(c);
}

void g_vertex_buffer_draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(4, GL_FLOAT, 0, g_vertex_data_position);
	glColorPointer(4, GL_FLOAT, 0, g_vertex_data_color);
	
	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	g_vertex_data_count = 0;
}

void g_set_primitive_mode(GLenum mode)
{
	if (g_primitive_mode != mode && g_vertex_data_count != 0)
		g_vertex_buffer_draw();
	
	g_primitive_mode = mode;
}

void g_begin_points()
{
	if (g_scale_dim_flag == G_3D) {
		glEnd();
		glDisable(GL_LIGHTING);
	}
	current_color = current_marker_color;
	
	g_set_primitive_mode(GL_POINTS);
}

void g_begin_lines()
{
	if (g_scale_dim_flag == G_3D) {
		glEnd();
		glDisable(GL_LIGHTING);
	}
	current_color = current_line_color;
	
	g_set_primitive_mode(GL_LINES);
}

void g_begin_triangles()
{
	if (g_scale_dim_flag == G_3D) {
		glEnd();
		glEnable(GL_LIGHTING);
		
		current_color = current_area_color_3D;
	} else {
		current_color = current_area_color_2D;
	}
	
	g_set_primitive_mode(GL_TRIANGLES);
}
