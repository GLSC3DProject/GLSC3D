#include "glsc3d_private.h"

/***************************My_glBegin**************************************************************/
void g_line_loop(void)
{
    glEnd();
    glBegin(GL_LINE_LOOP);
}
void g_line_strip(void)
{
    glEnd();
    glBegin(GL_LINE_STRIP);
}
void g_lines(void)
{
    glEnd();
    glBegin(GL_LINES);
}
void g_polygon(void)
{
    glEnd();
    glBegin(GL_POLYGON);
}
void g_triangles(void)
{
    glEnd();
    glBegin(GL_TRIANGLES);
}
void g_triangle_strip(void)
{
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
}
void g_triangle_fan(void)
{
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
}
void g_quads(void)
{
    glEnd();
    glBegin(GL_QUADS);
}
void g_quad_strip(void)
{
    glEnd();
    glBegin(GL_QUAD_STRIP);
}

void g_points(void)
{
    glEnd();
    glBegin(GL_POINTS);
}
/*****************************glNormal********************************************/
void glNormals(G_VECTOR u)
{
    glNormal3f(u.x, u.y, u.z);
}
/******************************glVertex******************************************/
void glVertexs(G_VECTOR u)
{
    glVertex4f(u.x, u.y, u.z, u.w);
}
