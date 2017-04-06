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

#define     data(i, j)      data[(j)*N_x+(i)]

#define     data_ext(i, j)  data_ext[(j)*(N_x+2)+(i)]
#define     normal(i, j)    normal[(j)*N_x+(i)]

#define		U2(i,j)     u2[(j) * (imax) + (i)]
#define		U2_EXT(i,j) u2_ext[(j) * ((imax) + 2) + (i)]

// Bird_view用に境界条件を設定しているバルク部分はコピー, 境界部分は片側差分
void Extend2Dim(int imax, int jmax, double *u2, double *u2_ext)
{
    int i, j;
    
    for (j = 0; j < jmax; j++)
        for (i = 0; i < imax; i++)
            U2_EXT(i + 1, j + 1) = U2(i, j);
    
    for (j = 1; j < jmax + 1; j++)
    {
        U2_EXT(0, j) = 2 * U2_EXT(1, j) - U2_EXT(2, j);
        U2_EXT(imax + 1, j) = 2 * U2_EXT(imax, j) - U2_EXT(imax - 1, j);
    }
    
    for (i = 0; i < imax + 2; i++)
    {
        U2_EXT(i, 0) = 2 * U2_EXT(i, 1) - U2_EXT(i, 2);
        U2_EXT(i, jmax + 1) = 2 * U2_EXT(i, jmax) - U2_EXT(i, jmax - 1);
    }
}

void g_bird_view_f_3D(
    double x0, double x1,    //図を表示したい範囲
    double y0, double y1,      //図を表示したい範囲
    int N_x, int N_y,                   //配列のサイズ
    double *data,                       //二次元配列
    G_BOOL WIRE, G_BOOL FILL)
{
    int i,j,k;
    double dx = (x1 - x0) / (N_x - 1);
    double dy = (y1 - y0) / (N_y - 1);
    
    G_VECTOR rc;
    G_VECTOR r_corner[4];
    G_VECTOR rc_normal;
    G_VECTOR normal_k[4];
    
    int Data_Size_normal = (N_x * N_y);
    int Data_Size_ext = (N_x + 2) * (N_y + 2);
    int Data_Size = Data_Size_ext * sizeof(double) + Data_Size_normal * sizeof(G_VECTOR);
    
    GLSC3D_Data_Buffer = GLSC3D_Array_Buffer(Data_Size);
    G_VECTOR *normal = (G_VECTOR *)GLSC3D_Data_Buffer;
    
    double *data_ext = (double *)(normal + Data_Size_normal);
    
    Extend2Dim(N_x, N_y, data, data_ext);
    
    for(j = 0; j < N_y; j ++)
        for(i = 0;i < N_x; i ++)
            normal(i, j) = G_VECTOR(-(data_ext((i + 1 + 1), (j + 0 + 1)) - data_ext((i - 1 + 1), (j + 0 + 1))) / (2*dx), -(data_ext((i + 0 + 1), (j + 1 + 1)) - data_ext((i - 0 + 1), (j - 1 + 1))) / (2*dy), 1);
    
    for(j = 0; j < N_y - 1; j ++)
    {
        for(i = 0;i < N_x - 1; i ++)
        {
            //3 2
            //0 1
            r_corner[0] = G_VECTOR(dx * (i + 0) + x0,dy * (j + 0) + y0,data_ext((i + 0 + 1), (j + 0 + 1)));
            r_corner[1] = G_VECTOR(dx * (i + 1) + x0,dy * (j + 0) + y0,data_ext((i + 1 + 1), (j + 0 + 1)));
            r_corner[2] = G_VECTOR(dx * (i + 1) + x0,dy * (j + 1) + y0,data_ext((i + 1 + 1), (j + 1 + 1)));
            r_corner[3] = G_VECTOR(dx * (i + 0) + x0,dy * (j + 1) + y0,data_ext((i + 0 + 1), (j + 1 + 1)));
            rc = G_VECTOR(0.,0.,0.);
            
            for(k = 0;k < 4;k ++) rc += r_corner[k] / 4;
            
            //  2
            //3   1
            //	0
            rc_normal = G_VECTOR(0.,0.,0.);
            for (k=0; k<4; k++) rc_normal += g_calc_normal(rc, r_corner[k], r_corner[(k+1)%4])/4;
            
            normal_k[0] = normal(i,j);
            normal_k[1] = normal(i+1,j);
            normal_k[2] = normal(i+1,j+1);
            normal_k[3] = normal(i,j+1);
            
            if(FILL) for(k = 0; k < 4; k++) g_triangle_3D_smooth_worker(rc, r_corner[k], r_corner[(k+1) % 4], g_normalize(rc_normal), normal_k[k], normal_k[(k+1)%4], 0);
            
            if(WIRE)
            {
                g_move_s(r_corner[3]);
                for(k=0; k<4; k++) g_plot_s(r_corner[k]);
            }
        }
    }
}
#undef U2
#undef U2_EXT
