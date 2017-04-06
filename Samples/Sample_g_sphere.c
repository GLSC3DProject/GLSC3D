#include "glsc3d.h"

int main(void)
{
    g_init("Sample_g_sphere.c", 640, 480);
    
    double scale = 1;
    g_def_scale_3D(0, -scale, scale, -scale, scale, -scale, scale, 0, 0, 320, 480, 1, 1, 1, scale*2);
    g_def_scale_3D(1, -scale, scale, -scale, scale, -scale, scale, 320, 0, 320, 480, 1, 1, 1, scale*2);
                    
    while(1)
    {
        g_cls();

        g_sel_scale_3D(0);
        g_sphere_3D(0, 0, 0, scale, G_YES, G_NO);

        g_sel_scale_3D(1);
        g_sphere_3D_core(0, 0, 0, scale, 10, 2, G_NO, G_YES);

        g_finish();
    }
    return 0;
}
