#include<stdio.h>

#include "glsc3d_3.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
    g_init("Sample_g_circle.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);
    g_def_scale_2D(0, -2, 2, -2, 2,0, 0, WINDOW_SIZE_X/2, WINDOW_SIZE_Y);
    g_def_scale_3D(1, -2, 2, -2, 2, -2, 2, WINDOW_SIZE_X/2, 0, WINDOW_SIZE_X/2, WINDOW_SIZE_Y, 4, 4, 4, 1);
    
    
    int i_time;
    for(i_time = 0;;i_time++)
    {
        g_cls();
        g_sel_scale(0);
        g_circle_2D(1, 1, 1, G_YES, G_NO);

        g_circle_2D(-1, -1, 1, G_NO, G_YES);

        g_sel_scale(1);
        g_circle_3D(0, 0, 0, 1, 0.01*i_time, 0, G_NO, G_YES);
        g_circle_3D(0, 0, 0, 1, -0.01*i_time, 0, G_YES, G_NO);
        
        g_finish();
    }
    return 0;
}
