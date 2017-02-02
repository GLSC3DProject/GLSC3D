#include "glsc3d_private.h"

G_COLOR glsc3D_g_def_area2D[TotalDisplayNumber];
G_COLOR glsc3D_g_def_area3D[TotalDisplayNumber];

void g_area_color_2D_s(G_COLOR color)
{
	g_current_area_color_2D = color;
}

void g_area_color_3D_s(G_COLOR color)
{
	g_current_area_color_3D = color;
}

void g_area_color_2D(double r,double g,double b,double a)
{
	g_area_color_2D_s(g_color_core(r, g, b, a));
}

void g_area_color_3D(double r,double g,double b,double a)
{
	g_area_color_3D_s(g_color_core(r, g, b, a));
}

void g_def_area_3D(int id, double r, double g, double b, double a)
{
	glsc3D_g_def_area3D[id] = g_color_core(r, g, b, a);
}

void g_sel_area_3D(int id)
{
	g_area_color_3D_s(glsc3D_g_def_area3D[id]);
}

void g_def_area_2D(int id, double r, double g, double b, double a)
{
	glsc3D_g_def_area2D[id] = g_color_core(r, g, b, a);
}

void g_sel_area_2D(int id)
{
	g_area_color_2D_s(glsc3D_g_def_area2D[id]);
}
