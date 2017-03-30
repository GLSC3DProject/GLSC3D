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
#define         normal(i, j)                normal[(j)*N_x+(i)]

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
	G_VECTOR n[4], rc_n;
    
    G_VECTOR *normal;
    normal = (G_VECTOR *)malloc(sizeof(G_VECTOR ) * N_x * N_y);
    
	for(j = 0;j < N_y - 1;j ++)
	{
		for(i = 0;i < N_x - 1;i ++)
		{
			//3 2 
			//0 1
            r_corner[0] = G_VECTOR(dx * (i + 0) + x0,dy * (j + 0) + y0,data((i + 0), (j + 0)));
			r_corner[1] = G_VECTOR(dx * (i + 1) + x0,dy * (j + 0) + y0,data((i + 1), (j + 0)));
			r_corner[2] = G_VECTOR(dx * (i + 1) + x0,dy * (j + 1) + y0,data((i + 1), (j + 1)));
			r_corner[3] = G_VECTOR(dx * (i + 0) + x0,dy * (j + 1) + y0,data((i + 0), (j + 1)));
            rc = G_VECTOR(0.,0.,0.);
            
			for(k = 0;k < 4;k ++) rc += r_corner[k] / 4;
			
            if (i != 0 && i != N_x - 2 && j != 0 && j != N_y - 2)
                normal(i,j) = G_VECTOR(
                                              -(data((i + 1), (j + 0)) - data((i - 1), (j + 0))) / (2*dx),
                                              -(data((i + 0), (j + 1)) - data((i - 0), (j - 1))) / (2*dy),
                                              1);
            else if (i == 0 && j != N_y - 1) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + 1), (j + 0)) - data((i + 0), (j + 0))) / (dx),
                                              -(data((i + 0), (j + 1)) - data((i - 0), (j - 1))) / (2*dy),
                                              1);
            }
            else if (i == N_x - 1 && j != N_y - 1) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + N_x - 1), (j + 0)) - data((i + N_x - 2), (j + 0))) / (dx),
                                              -(data((i + 0), (j + 1)) - data((i - 0), (j - 1))) / (2*dy),
                                              1);
            }
            else if (i != N_x - 1 && j == 0) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + 1), (j + 0)) - data((i - 1), (j + 0))) / (2*dx),
                                              -(data((i + 0), (j + 1)) - data((i - 0), (j + 0))) / (dy),
                                              1);
            }
            else if (i != N_x - 1 && j == N_y - 1) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + 1), (j + 0)) - data((i - 1), (j + 0))) / (2*dx),
                                              -(data((i + 0), (j + N_y - 1)) - data((i - 0), (j + N_y - 2))) / (dy),
                                              1);
            }
            else if (i == 0 && j == 0) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + 1), (j + 0)) - data((i + 0), (j + 0))) / (dx),
                                              -(data((i + 0), (j + 1)) - data((i - 0), (j + 0))) / (dy),
                                              1);
            }
            else if (i == N_x - 1 && j == 0) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + N_x - 1), (j + 0)) - data((i + N_x - 2), (j + 0))) / (dx),
                                              -(data((i + 0), (j + 1)) - data((i - 0), (j + 0))) / (dy),
                                              1);
            }
            else if (i == 0 && j == N_y - 1) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + 1), (j + 0)) - data((i + 0), (j + 0))) / (dx),
                                              -(data((i + 0), (j + N_y - 1)) - data((i - 0), (j + N_y - 2))) / (dy),
                                              1);
            }
            else if (i == N_x - 1 && j == N_y - 1) {
                normal(i,j) = G_VECTOR(
                                              -(data((i + N_x - 1), (j + 0)) - data((i + N_x - 2), (j + 0))) / (dx),
                                              -(data((i + 0), (j + N_y - 1)) - data((i - 0), (j + N_y - 2))) / (dy),
                                              1);
            }
            
            
			//  2  
			//3   1
			//	0
            for (k=0; k<4; k++) {
                n[k] = g_calc_normal(rc, r_corner[k], r_corner[(k+1)%4]);
                //rc_n += n[k]/4;
                //rc_n = rc_n/
            }

            
            
			if(WireFill ==1) for(k = 0; k < 4; k++) g_triangle_3D_core_smooth(rc, r_corner[k], r_corner[(k+1) % 4], n[k], n[k], n[k], 0);
            
			if(WireFill == 0)
			{
				g_begin_line_loop();
				for(k=0; k<4; k++)
				{
					g_emit_vertex(r_corner[k]);
				}
			}
		}
	}
    
    free(normal);
}

#undef data
