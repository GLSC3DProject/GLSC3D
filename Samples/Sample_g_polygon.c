#include<stdio.h>

#include "glsc3d.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
    g_init("Sample_g_polygon.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);

    g_def_scale_2D(0,-2, 2, -2, 2,0, 0,WINDOW_SIZE_X/2, WINDOW_SIZE_Y);
    g_def_scale_2D(1,-2, 2, -2, 2,WINDOW_SIZE_X/2, 0,WINDOW_SIZE_X/2, WINDOW_SIZE_Y);
    
    double x[5],y[5];
    for(int i=0;i<5;i++)
    {
        x[i]=2*sin(M_PI/3*i);
        y[i]=2*cos(M_PI/3*i);
    }

    for(int i_time = 0;;i_time++)
    {
        g_cls();
        g_sel_scale_2D(0);
        g_polygon_2D(x,y,5, G_YES, G_NO);
        
        g_sel_scale_2D(1);
        g_polygon_2D(x,y,5, G_NO, G_YES);
        
        g_finish();
    }
    return 0;
}
