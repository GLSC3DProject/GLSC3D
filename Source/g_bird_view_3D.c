#include <stdio.h>
#include "glsc3d_private.h"

/****************************************
 boxmember[a]
         ___________
        |   |   |   |
        |a=7|a=6|a=5|
        |___|___|___|
        |   |   |   |
        |a=8|a=0|a=4|
        |___|___|___|
        |   |   |   |
        |a=1|a=2|a=3|
        |___|___|___|
 ****************************************/


#define         data(i, j)                  data[(j)*N_x+(i)]

double g_max_f(int N_x, int N_y, double *data)//dataの最大値を返す
{
    int i;
    double max = data[0];

	for(i = 1; i < N_x*N_y; ++i)
		max = max < data[i] ? data[i] : max;

	return max;
}

double g_min_f(int N_x, int N_y, double *data)//dataの最小値を返す
{
    int i;
    double min = data[0];
	
	for(i = 1; i < N_x*N_y; ++i)
		min = min > data[i] ? data[i] : min;

	return min;
}

void g_bird_view_f_3D(double x0, double x1,    //図を表示したい範囲
					  double y0, double y1,      //図を表示したい範囲 
					  int N_x, int N_y,                   //配列のサイズ
					  double *data,                       //二次元配列
					  G_WIREFILL WireFill)
{
	int i,j,k;
	double dx = (x1 - x0) / (N_x - 1);
	double dy = (y1 - y0) / (N_y - 1);
	G_VECTOR rc;
	G_VECTOR r_corner[4];
	G_VECTOR n[4];
	
	for(j = 0;j < N_y - 1;j ++)
	{
		for(i = 0;i < N_x - 1;i ++)
		{
			//3 2 
			//0 1
			r_corner[0] = g_vector3(dx * (i + 0) + x0,dy * (j + 0) + y0,data[(j + 0) * N_x + (i + 0)]);
			r_corner[1] = g_vector3(dx * (i + 1) + x0,dy * (j + 0) + y0,data[(j + 0) * N_x + (i + 1)]);
			r_corner[2] = g_vector3(dx * (i + 1) + x0,dy * (j + 1) + y0,data[(j + 1) * N_x + (i + 1)]);
			r_corner[3] = g_vector3(dx * (i + 0) + x0,dy * (j + 1) + y0,data[(j + 1) * N_x + (i + 0)]);
			rc = g_vector3(0.0,0.0,0.0);
			for(k = 0;k < 4;k ++) rc = g_plus(rc,g_multi(0.25, r_corner[k]));
			
			//  2  
			//3   1
			//	0
			n[0] = g_cross(g_minus(r_corner[0],rc),g_minus(r_corner[1],rc));
			n[0] = g_multi(1 / g_norm(n[0]),n[0]);
			n[1] = g_cross(g_minus(r_corner[1],rc),g_minus(r_corner[2],rc));
			n[1] = g_multi(1 / g_norm(n[1]),n[1]);
			n[2] = g_cross(g_minus(r_corner[2],rc),g_minus(r_corner[3],rc));
			n[2] = g_multi(1 / g_norm(n[2]),n[2]);
			n[3] = g_cross(g_minus(r_corner[3],rc),g_minus(r_corner[0],rc));
			n[3] = g_multi(1 / g_norm(n[3]),n[3]);
			
			if(WireFill ==1)
			{
				glEnd();
				glEnable(GL_LIGHTING);
				G_VERTEX v0,v1,v2;
				G_TRIANGLE t[4];
				
				for(k = 0; k < 4; ++k)
				{
					v0 = g_make_vertex(rc,n[k],g_current_area_color_3D);
					v1 = g_make_vertex(r_corner[k],n[k],g_current_area_color_3D);
					v2 = g_make_vertex(r_corner[(k + 1) & 3],n[k],g_current_area_color_3D);
					t[k] = g_make_triangle_core(v0, v1, v2);
					g_set_triangle(t[k]);
				}
				//glEnd();
			}
			if(WireFill == 0)
			{
				glEnd();
				glDisable(GL_LIGHTING);
				g_line_loop();
				for(k=0; k<4; k++)
				{
					glColor4fv(&g_current_line_color.r);
					glVertexs(r_corner[k]);
				}
				//glEnd();
			}
		}
	}
}

//deprecated
void g_bird_view_3D(double x0, double x1,      //図を表示したい範囲
		double y0, double y1,      //図を表示したい範囲
		int N_x, int N_y,                   //配列のサイズ
		double data[N_x][N_y],              //二次元配列
		G_WIREFILL WireFill)
{
	g_bird_view_f_3D(x0, x1, y0, y1, N_y, N_x, &data[0][0], WireFill);
}
#undef data

