#include "glsc3d_3_private.h"

G_COLOR g_current_area_color;

G_COLOR glsc3D_g_def_area[TotalDisplayNumber];

void g_area_color_s(G_COLOR color)
{
	g_current_area_color = color;
}

void g_area_color(float r, float g, float b, float a)
{
	g_current_area_color = G_COLOR(r, g, b, a);
}

void g_def_area(int id, float r, float g, float b, float a)
{
	glsc3D_g_def_area[id] = G_COLOR(r, g, b, a);
}

void g_sel_area(int id)
{
	g_current_area_color = glsc3D_g_def_area[id];
}
