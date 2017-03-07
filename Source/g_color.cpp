#include "glsc3d_private.h"

G_COLOR g_current_area_color_3D(1, 1, 1, 1);
G_COLOR g_current_area_color_2D(1, 1, 1, 1);
G_COLOR g_current_line_color(1, 1, 1, 1);
G_COLOR g_current_text_color(1, 1, 1, 1);
G_COLOR g_current_marker_color(1, 1, 1, 1);

G_COLOR g_current_color;

void g_area_color_3D(double r, double g, double b, double a)
{
	g_current_area_color_3D = G_COLOR(r, g, b, a);
}

void g_area_color_2D(double r, double g, double b, double a)
{
	g_current_area_color_2D = G_COLOR(r, g, b, a);
}

void g_line_color(double r, double g, double b, double a)
{
	g_current_line_color = G_COLOR(r, g, b, a);
}

void g_text_color(double r, double g, double b, double a)
{
	g_current_text_color = G_COLOR(r, g, b, a);
}

void g_marker_color(double r,double g,double b,double a)
{
	g_current_marker_color = G_COLOR(r, g, b, a);
}