#include "glsc3d_private.h"
void g_polyline_2D(double *xx, double *yy, int n)
{
	glEnd();
	glDisable(GL_LIGHTING);
	g_line_strip();
	int i;
	for(i=0;i<n;i++)
	{
		glColor4fv(&g_current_line_color.r);
		glVertex2d(xx[i], yy[i]);
	}
	glEnd();
}

void g_polyline_3D(double *xx, double *yy, double *zz, int n)
{
	glEnd();
	glDisable(GL_LIGHTING);
	g_line_strip();
	int i;
	for(i=0;i<n;i++)
	{
		glColor4fv(&g_current_line_color.r);
		glVertex3d(xx[i], yy[i], zz[i]);
	}
	glEnd();
}

