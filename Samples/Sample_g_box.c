#include<stdio.h>
#include "glsc3d.h"

#define WINDOW_SIZE_X     (1000)
#define WINDOW_SIZE_Y     (500)

int main()
{
	g_init("Sample_g_box.c", WINDOW_SIZE_X, WINDOW_SIZE_Y);
	
	g_def_scale_3D(0,-2, 2, -2, 2, -2, 2,WINDOW_SIZE_X/2, 0,WINDOW_SIZE_X/2, WINDOW_SIZE_Y,1, 1, 1,1);
	g_def_scale_2D(1,-2, 2, -2, 2,0, 0,WINDOW_SIZE_X/2, WINDOW_SIZE_Y);
	
	int i_time;
	for(i_time = 0;;i_time++)
	{
		g_cls();
		g_sel_scale_2D(1);
		g_box_2D(1,1,1,1,G_NO, G_YES);
		g_box_2D(-1,-1,1,1,G_YES, G_NO);
		
		g_sel_scale_3D(0);
		g_box_3D(0,0,0,3,3,3,G_YES, G_YES);
		
		g_finish();
	}
	return 0;
}
