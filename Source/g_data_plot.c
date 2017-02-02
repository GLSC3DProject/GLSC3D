#include "glsc3d_private.h"
#define     data(i, j)      data[(j)*N_y+(i)]

void g_data_plot_f_3D(double x0, double x1,
		double y0, double y1,
		int N_x, int N_y,
		double *data)
{
	int i, j;
	double dx=(x1-x0)/N_x, dy=(y1-y0)/N_x;

	glEnd();
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);

	glColor4fv(&g_current_marker_color.r);

	for (j = 0; j < N_y; ++j)
		for (i = 0; i < N_x; ++i)
			glVertex3d(x0 + dx*i, y0 + dy*j, data(i, j));

	glEnd();
}
void g_data_plot_2D(double x_left, double x_right,
		double *yy, int n)
{
	double dx=(x_right-x_left)/n;
	int i;
	glEnd();
	glDisable(GL_LIGHTING);
	g_line_strip();

	glColor4fv(&g_current_line_color.r);

	for(i=0;i<n;i++)
		glVertex2d(x_left+i*dx, yy[i]);

	glColor4fv(&g_current_marker_color.r);

	for(i=0;i<n;i++)
		g_marker_2D(x_left+i*dx, yy[i]);

	glEnd();
}

void g_data_plot_3D(double x0, double x1,
		double y0, double y1,
		int N_x, int N_y,
		double data[N_x][N_y])
{
	g_data_plot_f_3D(x0, x1, y0, y1, N_y, N_x, &data[0][0]);
}


