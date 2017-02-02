#include "glsc3d_private.h"
void g_polygon_2D(double *xx, double *yy, int n, G_WIREFILL WireFill)
{
	glEnd();
	glDisable(GL_LIGHTING);
	if(WireFill == 1)
	{
		g_polygon();
		int i;
		for(i=0;i<n;i++)
		{
			glColor4fv(&g_current_area_color_2D.r);
			glVertex2d(xx[i], yy[i]);
		}
	}
	if(WireFill == 0)
	{
		g_line_loop();
		int i;
		for(i=0;i<n;i++)
		{
			glColor4fv(&g_current_line_color.r);
			glVertex2d(xx[i], yy[i]);
		}
	}
	glEnd();
}

