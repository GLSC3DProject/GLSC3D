#include "glsc3d_private.h"

G_COLOR g_current_area_color_3D;
G_COLOR g_current_area_color_2D;

G_COLOR glsc3D_g_def_area2D[TotalDisplayNumber];
G_COLOR glsc3D_g_def_area3D[TotalDisplayNumber];

void g_area_color_3D_s(G_COLOR color)
{
	g_current_area_color_3D = color;
}

void g_area_color_2D_s(G_COLOR color)
{
	g_current_area_color_2D = color;
}

void g_area_color_3D(float r, float g, float b, float a)
{
	g_current_area_color_3D = G_COLOR(r, g, b, a);
}

void g_area_color_2D(float r, float g, float b, float a)
{
	g_current_area_color_2D = G_COLOR(r, g, b, a);
}

void g_def_area_2D(int id, float r, float g, float b, float a)
{
    glsc3D_g_def_area2D[id] = G_COLOR(r, g, b, a);
}

void g_sel_area_2D(int id)
{
    g_current_area_color_2D = glsc3D_g_def_area2D[id];
}

void g_def_area_3D(int id, float r, float g, float b, float a)
{
    glsc3D_g_def_area3D[id] = G_COLOR(r, g, b, a);
}

void g_sel_area_3D(int id)
{
    g_current_area_color_3D = glsc3D_g_def_area3D[id];
}
