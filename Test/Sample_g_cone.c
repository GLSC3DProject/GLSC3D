#include<stdlib.h>
#include<stdio.h>

#include "glsc3d.h"

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

int main()
{
    g_init("GRAPH", WINDOW_SIZE_X, WINDOW_SIZE_Y);
    
	g_def_scale_3D(0, -2, 2, -2, 2, -2, 2, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, 1, 1, 1, 1);
    
    int i_time;
    for(i_time = 0;;i_time++)
    {
        g_cls();
        
        g_sel_scale_3D(0);
        g_area_color_3D(1, 0, 0, 1);
		g_cone_3D(0, -1, 0, 0, 1, 0, 1.5, 3, G_NO, G_YES);
        
        g_finish();
    }
    return 0;
}
