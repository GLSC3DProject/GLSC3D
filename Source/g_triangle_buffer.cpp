#include <stdio.h>
#include <stdlib.h>
#include "glsc3d_private.h"

void g_triangle_buffer_reset();

void g_set_triangle(G_TRIANGLE t);

void g_triangle_quick(int *index, float *u, int start, int end);
void g_triangle_merge(int *index_s0, int *index_s1, int *index_d, float *u, unsigned int length0, unsigned int length1);

void g_triangle_buffer_merge();

G_TRIANGLE *triangle_buffer;
float *triangle_r;
int *camera_id;

int *aligned_index;
int aligned_index_index;

int *final_index;
int final_index_index;

int *merge_buffer;

//temporary buffer
int temp_index;

//triangle buffer
int buffer_index;

int current_index[TotalDisplayNumber];

int *triangle_index;

//#define OUTPUT_Length_FILE

#ifdef OUTPUT_Length_FILE
FILE *gp;
#endif

void g_triangle_buffer_init()
{
#ifdef OUTPUT_Length_FILE
    gp = popen("gnuplot","w");
#endif

    DBG_WRITE("enter g_triangle_buffer_init");
    
    if(g_enable_transparent)
    {
        /* 可視化処理のための三角形バッファ */
        if(!(triangle_buffer = (G_TRIANGLE*)malloc(TRIANGLE_BUFFER_SIZE*sizeof(G_TRIANGLE))))
        {
            fprintf(stderr, "failed to allocate memory\a\n");
            fprintf(stderr, "GLSC3D will abort\n");
            abort();
        }
        
        if(!(triangle_r = (float *)malloc(TRIANGLE_BUFFER_SIZE*sizeof(float))))
        {
            fprintf(stderr, "failed to allocate memory\a\n");
            fprintf(stderr, "GLSC3D will abort\n");
            abort();
        }
        
        if(!(camera_id = (int*)malloc(TRIANGLE_BUFFER_SIZE*sizeof(int))))
        {
            fprintf(stderr, "failed to allocate memory\a\n");
            fprintf(stderr, "GLSC3D will abort\n");
            abort();
        }
        
        if(!(aligned_index = (int*)malloc(2*TRIANGLE_BUFFER_SIZE*sizeof(int))))
        {
            fprintf(stderr, "failed to allocate memory\a\n");
            fprintf(stderr, "GLSC3D will abort\n");
            abort();
        }
        
        DBG_WRITE("initial aligned input : %p", aligned_index);
        
        if(!(final_index = (int*)malloc(TRIANGLE_BUFFER_SIZE*sizeof(int))))
        {
            fprintf(stderr, "failed to allocate memory\a\n");
            fprintf(stderr, "GLSC3D will abort\n");
            abort();
        }
        
        if(!(merge_buffer = (int*)malloc(TRIANGLE_BUFFER_SIZE*sizeof(int))))
        {
            fprintf(stderr, "failed to allocate memory\a\n");
            fprintf(stderr, "GLSC3D will abort\n");
            abort();
        }

        if(!(triangle_index = (int*)malloc(TEMPORARY_TRIANGLE_BUFFER_SIZE*sizeof(int))))
        {
            fprintf(stderr, "failed to allocate memory\a\n");
            fprintf(stderr, "GLSC3D will abort\n");
            abort();
        }
        /*
        char *mem = (char*)triangle_buffer;
        for(unsigned int i = 0; i < TRIANGLE_BUFFER_SIZE*sizeof(G_TRIANGLE); ++i)
            mem[i] = 0;
        */
        g_triangle_buffer_reset();
    }
}

void g_triangle_buffer_reset()
{
    DBG_WRITE("reset");
    for(int i = 0; i < TotalDisplayNumber; ++i)
        current_index[i] = -1;
    
    final_index_index = aligned_index_index = temp_index = buffer_index = 0;
    
    aligned_index[aligned_index_index++] = -1;
}

void g_triangle_buffer_append(G_TRIANGLE t)
{
	if(buffer_index == TRIANGLE_BUFFER_SIZE)
	{
		fprintf(stderr, "too many triangles for triangle buffer\n"
				"triangle buffer can recieve less than %d\n", TRIANGLE_BUFFER_SIZE);
		exit(0);
	}
	
	if(temp_index >= TEMPORARY_TRIANGLE_BUFFER_SIZE)
		g_triangle_buffer_flush();
	
	G_MATRIX c = glsc3D_inner_scale[g_current_scale_id].camera.view;
	G_VECTOR g = (t.vertex[0].position + t.vertex[1].position + t.vertex[2].position) / 3;
	
	float z = g.x * c.x.z + g.y * c.y.z + g.z * c.z.z;
	
	triangle_buffer[buffer_index] = t;
	
	triangle_r[buffer_index] = -z;
	
	camera_id[buffer_index] = g_current_scale_id;
	
	current_index[g_current_scale_id] = buffer_index++;
	
	temp_index++;
}

void g_triangle_buffer_flush()
{
    if(g_enable_transparent)
    {
        for(int i = 0; i < temp_index; ++i)
            triangle_index[i] = buffer_index - 1 - i;
        DBG_WRITE("before quick sort");
        DBG_WRITE("right : %d", temp_index - 1);
        if(temp_index > 0) g_triangle_quick(triangle_index, triangle_r, 0, temp_index - 1);
        DBG_WRITE("after quick sort");
        DBG_WRITE("temporary index     : %d", temp_index);
        DBG_WRITE("aligned index index : %d", aligned_index_index);
        DBG_WRITE("aligned index       : %p", aligned_index);
        for(int i = 0; i < temp_index; ++i)
            aligned_index[aligned_index_index++] = triangle_index[i];
        
        aligned_index[aligned_index_index++] = -1;
        DBG_WRITE("aligned index length : %d", aligned_index_index);
        temp_index = 0;

		//free(index);
    }
}

void g_triangle_quick(int *index, float *u, int start, int end)
{
    double          key;
    int wk;
    int             i, j;
    key = u[index[(start + end) / 2]];
    i = start - 1;
    j = end + 1;
    while (1)
    {
        while (u[index[++i]] < key)
            ;
        while (u[index[--j]] > key)
            ;
        if (i >= j)
            break;
        
        wk = index[i];
        index[i] = index[j];
        index[j] = wk;
    }
    
    if (start < i - 1)
        g_triangle_quick(index, u, start, i - 1);
    if (j + 1 < end)
        g_triangle_quick(index, u, j + 1, end);
}

void g_triangle_merge(int *index_s0, int *index_s1, int *index_d, float *u, unsigned int length0, unsigned int length1)
{
    if(length0 == 0 && length1 == 0)
        return;
    
    unsigned int i0 = 0, i1 = 0, id = 0;
    while((i0 < length0) && (i1 < length1))
    {
        if(u[index_s0[i0]] < u[index_s1[i1]])
            index_d[id++] = index_s0[i0++];
        else
            index_d[id++] = index_s1[i1++];
    }
    
    if(!(length0 - i0))
        for(unsigned int i = 0; i < length1 - i1; ++i)
            index_d[id++] = index_s1[i1 + i];
    else 
        for(unsigned int i = 0; i < length0 - i0; ++i)
            index_d[id++] = index_s0[i0 + i];
}

void g_triangle_buffer_draw()
{
    if(g_enable_transparent)
    {
        g_triangle_buffer_flush();
        g_triangle_buffer_merge();

        int current_camera_id = -1;
        for(unsigned int i = 0; i < final_index_index; ++i)
        {
            unsigned int id = final_index_index - 1 - i;
            if(current_camera_id != camera_id[id])
            {
                DBG_WRITE("cc id : %d c id : %d", current_camera_id, camera_id[id]);
                current_camera_id = camera_id[id];
                g_sel_scale_3D(current_camera_id);
            }
            g_triangle_terminal(triangle_buffer[final_index[id]]);
        }

#ifdef OUTPUT_Length_FILE
        /*
        for(int j = 0;j < final_index_index;j ++)
            fprintf(gp,"%f \n",triangle_r[final_index[final_index_index - 1 - j]]);
         */
        double tmp = 0;
        for(int j = 0;j < final_index_index - 1;j ++)
        {
            tmp += triangle_r[final_index[j + 1]] - triangle_r[final_index[j]];
        }
        tmp /= final_index_index;

        //fprintf(gp,"set yrange [0:%lf] \n",tmp);
        fprintf(gp,"plot '-' w l \n");
        for(int j = 0;j < final_index_index;j ++)
        {
            tmp = triangle_r[final_index[j]];
            fprintf(gp,"%f \n",tmp);
        }
        /*
        for(int j = 0;j < final_index_index - 1;j ++)
        {
            tmp = triangle_r[final_index[j + 1]] - triangle_r[final_index[j]];
            fprintf(gp,"%f \n",log(tmp));
        }
         */
        fprintf(gp,"e\n");
        fflush(gp);
        //usleep(1000000);
#endif
        g_triangle_buffer_reset();
    }
}

void g_triangle_buffer_merge()
{
    aligned_index[aligned_index_index++] = -2;
    int i_length = 0;
    
    DBG_WRITE("enter g_triangle_merge");
    
    for(int i = 0; i < TotalDisplayNumber; ++i)
    {
        DBG_WRITE("display id : %d", i);
        if(current_index[i] != -1)
        {
			unsigned int merge_buffer_length = 0;
            for(int j = 0; j < aligned_index_index; ++j)
            {
                DBG_WRITE("aligned_index_index:%d j:%d", aligned_index_index, j);
                if(aligned_index[j] == -1)
                {
                    DBG_WRITE("aligned_index : %d", aligned_index[j]);
					unsigned int length = 0;
                    while(aligned_index[j + ++length] != -1)
                        if(aligned_index[j + length] == -2)
                            break;
                    length -= 1;
                    DBG_WRITE("length : %d", length);
                    if(length)
                        if(camera_id[aligned_index[j + 1]] == i)
                        {
                            
                            g_triangle_merge(merge_buffer, aligned_index + j + 1, final_index + i_length, triangle_r, merge_buffer_length, length);
                            for(int k = 0; k < merge_buffer_length + length; ++k)
                                merge_buffer[k] = (final_index + i_length)[k];
                            merge_buffer_length += length;
                            DBG_WRITE("merge_buffer_length : %d", merge_buffer_length);
                        }
                    j += length;
                }
            }
            
            for(int j = 0; j < merge_buffer_length; ++j)
                (final_index + i_length)[j] = merge_buffer[j];
            i_length += merge_buffer_length;
            DBG_WRITE("i_length : %d", i_length);
        }
    }
    final_index_index = i_length;
    DBG_WRITE("final_index length : %d", final_index_index);
}
