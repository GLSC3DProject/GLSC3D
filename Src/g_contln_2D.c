#include "glsc3d_private.h"
#include <stdio.h>
#include <time.h>

#define boxmember3D(i,j) (boxmember[(i)*N_y+(j)])
#define trimember3D(i,j) (trimember[(i)*N_y+(j)])
#define boxmember2D(i,j) (boxmember2D[(i)*N_y+(j)])
#define trimember2D(i,j) (trimember2D[(i)*N_y+(j)])
#define flag(i,j)        (flag[(i)*N_y+(j)])
#define data2D(i, j)     data2D[(i)*N_y+(j)]

void g_contln_f_2D(
	double x_left, double x_right,
	double y_bottom, double y_top,
	int N_x, int N_y,
	double *data2D,
	double level)
{
	int i,j,k,l;
	double xl = x_right-x_left,yl = y_top-y_bottom;
	double s;
	double dxx = xl/N_x, dyy = yl/N_y;

	G_VECTOR trimember2D[3];
	G_VECTOR trimember[4][3];
	int flag[3], dflag;

	for(i=0; i<N_x-1; i++)
	{
		for(j=0; j<N_y-1; j++)
		{
			//三角格子の各頂点に値を代入
			trimember[0][0] = g_vector3(x_left+(i+0.5)*dxx, y_bottom+(j+0.5)*dyy, data2D(i, j));
			trimember[1][0] = g_vector3(x_left+(i+1.5)*dxx, y_bottom+(j+0.5)*dyy, data2D(i+1, j));
			trimember[2][0] = g_vector3(x_left+(i+1.5)*dxx, y_bottom+(j+1.5)*dyy, data2D(i+1, j+1));
			trimember[3][0] = g_vector3(x_left+(i+0.5)*dxx, y_bottom+(j+1.5)*dyy, data2D(i, j+1));

			trimember[0][1] = trimember[1][0];
			trimember[1][1] = trimember[2][0];
			trimember[2][1] = trimember[3][0];
			trimember[3][1] = trimember[0][0];

			trimember[0][2] = g_multi(0.25,g_plus(g_plus(trimember[0][0], trimember[1][0]),g_plus(trimember[2][0], trimember[3][0])));
			trimember[1][2] = trimember[0][2];
			trimember[2][2] = trimember[0][2];
			trimember[3][2] = trimember[0][2];
			for(k=0; k<4; k++)
			{
				//各頂点の(x,y)座標
				for(l=0; l<3; l++)
					trimember2D[l] = g_vector2(trimember[k][l].x,trimember[k][l].y);

				dflag = 0;
				//フラグを立てる
				for(l=0; l<3; l++)
				{

					if(trimember[k][l].z == level)
					{
						flag[l] = 0;
					}else if(trimember[k][l].z > level)
					{
						flag[l] = 1;
						dflag = dflag == 0 ? 1 : dflag == 3 ? 2 : dflag;
					}else if(trimember[k][l].z < level)
					{
						flag[l] = -1;
						dflag = dflag == 0 ? 3 : dflag == 1 ? 2 : dflag;
					}else
					{
						fprintf(stderr, "given data has invalid data.\nyour code may have any bug!\n");
						return;
					}
				}
				if(dflag == 2)
				{
					g_begin_lines();
					//等高線描写
					if(flag[0]*flag[1] < 0)
					{
						s = (level - trimember[k][0].z)/(trimember[k][1].z - trimember[k][0].z);
						g_emit_vertex(g_plus(g_multi(1 - s, trimember2D[0]), g_multi(s, trimember2D[1])));
						if(flag[2] == 0)
						{
							g_emit_vertex(trimember2D[2]);
							continue;
						}
					}
					if(flag[1]*flag[2] < 0)
					{
						s = (level - trimember[k][1].z)/(trimember[k][2].z - trimember[k][1].z);
						g_emit_vertex(g_plus(g_multi(1 - s, trimember2D[1]), g_multi(s, trimember2D[2])));
						if(flag[0] == 0)
						{
							g_emit_vertex(trimember2D[0]);
							continue;
						}
					}
					if(flag[2]*flag[0] < 0)
					{
						s = (level - trimember[k][2].z)/(trimember[k][0].z - trimember[k][2].z);
						g_emit_vertex(g_plus(g_multi(1 - s, trimember2D[2]), g_multi(s, trimember2D[0])));
						if(flag[1] == 0)
						{
							g_emit_vertex(trimember2D[1]);
							continue;
						}
					}
				}

			}
		}
	}
}
