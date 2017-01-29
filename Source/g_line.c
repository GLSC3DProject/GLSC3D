#include "glsc3d_private.h"

struct
{
	double r,g,b,a, width;
	int type;
}           glsc3D_g_def_line[TotalDisplayNumber];

void g_line_color(double r,double g,double b,double a)
{
    current_line_color = g_color_core(r,g,b,a);
}

void g_line_width(double size)
{
    glEnd();
    glLineWidth(size);
}

void g_line_type(int type)
{
    glEnd();
    if (type==0) {
        glDisable(GL_LINE_STIPPLE);
    }else if (type==1) {
        glLineStipple(1, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
    }else if (type==2) {
        glLineStipple(2, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
    }else if (type==3) {
        glLineStipple(1, 0x0C0F);
        glEnable(GL_LINE_STIPPLE);
    }else if (type==4) {
        glLineStipple(3, 0x0C0F);
        glEnable(GL_LINE_STIPPLE);
    }else if (type==5) {
        glLineStipple(1, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }else if (type==6) {
        glLineStipple(2, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }else if (type==7) {
        glLineStipple(3, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }else if (type==8) {
        glLineStipple(4, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }
}

void g_def_line(int id, double r, double g, double b, double a, double width, int type)
{
    glsc3D_g_def_line[id].r = r;
    glsc3D_g_def_line[id].g = g;
    glsc3D_g_def_line[id].b = b;
    glsc3D_g_def_line[id].a = a;
    glsc3D_g_def_line[id].width = width;
    glsc3D_g_def_line[id].type = type;
}

void g_sel_line(int id)
{
    g_line_color(glsc3D_g_def_line[id].r, glsc3D_g_def_line[id].g, glsc3D_g_def_line[id].b, glsc3D_g_def_line[id].a);
    g_line_width(glsc3D_g_def_line[id].width);
    g_line_type(glsc3D_g_def_line[id].type);
}
