#include<stdlib.h>
#include<stdio.h>

#include "glsc3d.h"

#define WINDOW_SIZE_X     (500)
#define WINDOW_SIZE_Y     (500)

int main()
{
    g_init("GRAPH", WINDOW_SIZE_X, WINDOW_SIZE_Y);
    
    g_def_scale_2D(0,-2, 2, -2, 2,0, 0,WINDOW_SIZE_X, WINDOW_SIZE_Y);
    
    g_def_marker(0,1,0,0,1,0,10);
    g_def_marker(1,0,1,0,1,1,15);
    g_def_marker(2,0,0,1,1,0,20);
    g_def_marker(3,1,1,0,1,1,25);
    g_def_marker(4,1,0,1,1,0,30);
    g_def_marker(5,0,1,1,1,1,35);
    g_def_marker(6,1,1,1,1,0,40);
    
    int i_time;
    for(i_time = 0;;i_time++)
    {
        g_cls();
        g_sel_scale_2D(0);
        g_sel_marker(0);
        g_marker_2D(0,-1.5);
        
        g_sel_marker(1);
        g_marker_2D(0,-1);
        
        g_sel_marker(2);
        g_marker_2D(0,-0.5);
        
        g_sel_marker(3);
        g_marker_2D(0,0);
        
        g_sel_marker(4);
        g_marker_2D(0,0.5);
        
        g_sel_marker(5);
        g_marker_2D(0,1);
        
        g_sel_marker(6);
        g_marker_2D(0,1.5);
        
        g_finish();
    }
    return 0;
}
