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
			glColor4d(current_area_color_2D.r, current_area_color_2D.g, current_area_color_2D.b, current_area_color_2D.a);
			glVertex2d(xx[i], yy[i]);
		}
	}
	if(WireFill == 0)
	{
		g_line_loop();
		int i;
		for(i=0;i<n;i++)
		{
			glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
			glVertex2d(xx[i], yy[i]);
		}
	}
	glEnd();
}

