#include "glsc3d.h"

int main()
{
    g_init("GRAPH", 640, 480);
    
    double scale = 1;
    
    g_def_scale_3D(0, -scale, scale, -scale, scale, -scale, scale, 0, 0, 640, 480, 1, 1, 1, scale*2);
                    
    while(1)
    {
        g_cls();
        g_sleep(0.01);
        
        g_sel_scale_3D(0);
                
        g_area_color_3D(1, 0, 0, 1);
        
        g_sphere_3D(0, 0, 0, scale);

        g_finish();
    }
}
