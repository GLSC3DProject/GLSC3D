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
static const size_t VERTEX_BUFFER_SIZE = 3 << 10;

G_VECTOR_F *g_vertex_data_position;
int g_vertex_data_count;
GLenum g_primitive_mode;

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
	g_vertex_data_count = 0;
}

void g_vertex_buffer_append(G_VECTOR_F vertex)
{
	g_vertex_data_position[g_vertex_data_count++] = vertex;
	
	if (g_vertex_data_count == VERTEX_BUFFER_SIZE)
		g_vertex_buffer_draw();
}

void g_vertex_buffer_draw()
{
	G_COLOR current_color;
	
	if (g_primitive_mode == GL_POINTS)
		current_color = current_marker_color;
	else if (g_primitive_mode == GL_LINES || g_primitive_mode == GL_LINE_STRIP || g_primitive_mode == GL_LINE_LOOP)
		current_color = current_line_color;
	else if (g_scale_dim_flag == G_2D)
		current_color = current_area_color_2D;
	else
		current_color = current_area_color_3D;
	
	glColor4fv(&current_color.r);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, 0, g_vertex_data_position);
	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	g_vertex_data_count = 0;
}

void g_set_primitive_mode(GLenum mode)
{
	if (g_primitive_mode != mode && g_vertex_data_count != 0)
		g_vertex_buffer_draw();
	
	g_primitive_mode = mode;
}
