#include<stdio.h>
#include<math.h>

#include "glsc3d.h"

#define INTV              (1)
#define WINDOW_SIZE_X     (700)
#define WINDOW_SIZE_Y     (700)

int main()
{
	g_init(G_OFF_SCREEN, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_capture_set("");
	printf("GLSC3D : begin off screen redering\n");
	fflush(stdout);

	for(int i = 0; i < 20; i++)
	{
		g_scr_color(cos(0.1*i)*sin(0.1*i),cos(0.1*i),sin(0.1*i));
		g_cls();
		
		g_finish();
		g_capture();
	}

	printf("GLSC3D : end off screen redering\n");
	printf("please check GLSC3D/TestProgram/Sample_off_screen/FramesXXX/XXX.png\n");
	return 0;
}
