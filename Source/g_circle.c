#include "glsc3d_private.h"
void g_circle_2D(double center_x, double center_y,
		double radius, G_WIREFILL WireFill)
{
	int i,STEP=100;
	double dtheta=2*PI/STEP;

	if(WireFill == 1)
	{
		glEnd();
		glDisable(GL_LIGHTING);
		g_triangle_fan();
		for(i=0; i<=STEP; i++)
		{
			glColor4d(current_area_color_2D.r, current_area_color_2D.g, current_area_color_2D.b, current_area_color_2D.a);
			glVertex2d(center_x + radius*cos(i*dtheta),center_y + radius*sin(i*dtheta));
		}
		glEnd();
	}
	if(WireFill == 0)
	{
		glEnd();
		glDisable(GL_LIGHTING);
		g_line_strip();
		for(i=0; i<=STEP; i++)
		{
			glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
			glVertex2d(center_x + radius*cos(i*dtheta),center_y + radius*sin(i*dtheta));
		}
		glEnd();
	}
}
void g_circle_3D_core(double center_x, double center_y, double center_z,
		double radius,
		double theta, double phi,
		int N, int DivideLevel, G_WIREFILL WireFill)
{
	int i,STEP=N;
	double dtheta=2*PI/STEP;
	G_VECTOR r = g_vector3(radius,0,0);
	G_VECTOR center = g_vector3(center_x,center_y,center_z), r0, r1, r2;

	r0 = g_plus(center,Rz(Rx(r,theta),phi));
	if(WireFill==1)
	{
		//glEnable(GL_LIGHTING);
		for(i=0; i<STEP; i++)
		{
			r1 = g_plus(center,Rz(Rx(Ry(r,i*dtheta),theta),phi));
			r2 = g_plus(center,Rz(Rx(Ry(r,(i+1)*dtheta),theta),phi));
			g_triangle_3D_core(center.x,center.y,center.z,
					r1.x,r1.y,r1.z,
					r2.x,r2.y,r2.z,
					DivideLevel, WireFill);
		}
		//glEnd();
	}    
	if(WireFill==0)
	{
		glEnd();
		glDisable(GL_LIGHTING);
		g_line_strip();
		for(i=0; i<=STEP; i++)
		{
			glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
			glVertexs(g_plus(center,Rz(Rx(Ry(r,i*dtheta),theta),phi)));
		}
		glEnd();
	}

}

void g_circle_3D(double center_x, double center_y, double center_z,
		double radius,
		double theta, double phi,
		G_WIREFILL WireFill)
{
	g_circle_3D_core(center_x, center_y, center_z, radius, theta, phi, 100, 0, WireFill);
}


