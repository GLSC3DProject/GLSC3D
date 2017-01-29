#include "glsc3d_private.h"

G_COLOR glsc3D_g_def_area2D[TotalDisplayNumber];
G_COLOR glsc3D_g_def_area3D[TotalDisplayNumber];

void g_area_color_3D(double r,double g,double b,double a)
{
    current_area_color_3D = g_color_core(r, g, b, a);
}

void g_area_color_2D(double r,double g,double b,double a)
{
    current_area_color_2D = g_color_core(r, g, b, a);
}

void g_def_area_3D(int id, double r, double g, double b, double a)
{
    glsc3D_g_def_area3D[id].r = r;
    glsc3D_g_def_area3D[id].g = g;
    glsc3D_g_def_area3D[id].b = b;
    glsc3D_g_def_area3D[id].a = a;
}

void g_sel_area_3D(int id)
{
    g_area_color_3D(glsc3D_g_def_area3D[id].r,glsc3D_g_def_area3D[id].g,glsc3D_g_def_area3D[id].b,glsc3D_g_def_area3D[id].a);
}

void g_def_area_2D(int id, double r, double g, double b, double a)
{
    glsc3D_g_def_area2D[id].r = r;
    glsc3D_g_def_area2D[id].g = g;
    glsc3D_g_def_area2D[id].b = b;
    glsc3D_g_def_area2D[id].a = a;
}

void g_sel_area_2D(int id)
{
    g_area_color_2D(glsc3D_g_def_area2D[id].r,glsc3D_g_def_area2D[id].g,glsc3D_g_def_area2D[id].b,glsc3D_g_def_area2D[id].a);
}
