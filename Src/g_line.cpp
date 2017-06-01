#include "glsc3d_3_private.h"

struct G_LINE_APPEARANCE
{
	G_COLOR color;
	float width;
	G_UINT type;

	void select()
	{
		g_line_color_s(color);
		g_line_width(width);
		g_line_type(type);
	}
};

GLint g_stipple_type[] = {
	0b11111111,
	0b01010101,
	0b01110111,
	0b00001111,
	0b00010001,
	0b01011111,
	0b01010111,
	0b01111111,
	0b00110111,
};

G_COLOR	g_current_line_color;
float	g_current_line_size;
int		g_current_line_stipple;
//bool	g_need_line_stipple_updated;

G_LINE_APPEARANCE glsc3D_g_def_line[TotalDisplayNumber];

void g_line_color_s(G_COLOR color)
{
	g_current_line_color = color;
}

void g_line_color(float r, float g, float b, float a)
{
	g_current_line_color = G_COLOR(r, g, b, a);
}

void g_line_width(float size)
{
	g_current_line_size = size;
	g_vertex_buffer_flush();
	glLineWidth(size);
}

void g_line_type(G_UINT type)
{
	if (type > 8) {
		printf("Invlid line type.\n");
		return;
	}

	int stipple = g_stipple_type[type];

	if (g_current_line_stipple != stipple) {
		g_current_line_stipple = stipple;
		g_vertex_buffer_flush();
		if (type != 0) {
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(4, stipple << 8 | stipple);
		}
		else {
			glDisable(GL_LINE_STIPPLE);
		}
//		g_need_line_stipple_updated = true;
	}
}

void g_def_line(int id, float r, float g, float b, float a, float width, G_UINT type)
{
	glsc3D_g_def_line[id].color = G_COLOR(r, g, b, a);
	glsc3D_g_def_line[id].width = width;
	glsc3D_g_def_line[id].type = type;
}

void g_sel_line(int id)
{
	glsc3D_g_def_line[id].select();
}
