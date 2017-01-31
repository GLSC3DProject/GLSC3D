//
//  g_vertex_buffer.c
//  GLSC3D
//
//  Created by student on 2017/01/31.
//  Copyright © 2017年 student. All rights reserved.
//

#include <stdio.h>
#include "glsc3d_private.h"

#define VERTEX_BUFFER_SIZE (3 << 10)

G_VECTOR_F *g_vertex_position_data;

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

void g_vertex_buffer_data_init()
{
	g_vertex_position_data = (G_VECTOR_F *)g_malloc(VERTEX_BUFFER_SIZE * sizeof(G_VECTOR_F));
}

