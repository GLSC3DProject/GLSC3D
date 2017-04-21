#include<stdio.h>
#include<math.h>

#include "glsc3d_3.h"

#define INTV              (1)
#define WINDOW_SIZE_X     (700)
#define WINDOW_SIZE_Y     (700)
#define WINDOW_POS_X      (50)
#define WINDOW_POS_Y      (50)

int main()
{
    g_init_core("Sample_g_init.c", WINDOW_SIZE_X, WINDOW_SIZE_Y,WINDOW_POS_X,WINDOW_POS_Y, 0.9, 0.9, 0.9, 1, 1<<20, 1<<20);
    
    for(int i=0; ;i++)
    {
        g_scr_color(cos(0.01*i)*sin(0.01*i),cos(0.01*i),sin(0.01*i));
        g_cls();
        
        g_finish();
    }
    return 0;
}
