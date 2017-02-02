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
static const size_t VERTEX_BUFFER_SIZE = 3 << 12;

GLenum g_primitive_mode;
G_BOOL g_lighting_enabled;

int g_vertex_data_count;

G_VERTEX *g_vertex_data;

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
	
	glVertexPointer(3, GL_FLOAT, sizeof(G_VERTEX), &g_vertex_data[0].position);
	glColorPointer(4, GL_FLOAT, sizeof(G_VERTEX), &g_vertex_data[0].color);
	
	if (g_lighting_enabled) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		
		glNormalPointer(GL_FLOAT, sizeof(G_VERTEX), &g_vertex_data[0].normal);
	}
	
	glDrawArrays(g_primitive_mode, 0, g_vertex_data_count);
	
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

void g_enable_lighting()
{
	glEnd();
	glEnable(GL_LIGHTING);
	g_lighting_enabled = G_TRUE;
}

void g_disable_lighting()
{
	glEnd();
	glDisable(GL_LIGHTING);
	g_lighting_enabled = G_FALSE;
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
