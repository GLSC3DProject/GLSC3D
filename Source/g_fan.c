#include "glsc3d_private.h"
void g_fan_2D(double center_x, double center_y,       //中心座標
		double direction_x, double direction_y, //方向
		double radius,                          //半径
		double angle,                           //開く角度
		G_WIREFILL WireFill)
{
	int i,STEP=100;
	G_VECTOR center,direction;
	double dtheta;
	direction = g_vector2(direction_x, direction_y);
	direction = g_multi(1/g_norm(direction), direction);
	center = g_vector2(center_x, center_y);
	if(WireFill == 1)
	{
		glEnd();
		glDisable(GL_LIGHTING);
		g_triangle_fan();
		glVertexs(center);
		for(i=0; i<=STEP; i++)
		{
			dtheta = 0.5*angle/STEP;
			glColor4d(current_area_color_2D.r, current_area_color_2D.g, current_area_color_2D.b, current_area_color_2D.a);
			glVertexs(g_plus(center, Rx2D(g_multi(radius, direction),i*dtheta)));
		}
	
		g_triangle_fan();
		glVertexs(center);
		glVertexs(center);
		for(i=0; i<=STEP; i++)
		{
			dtheta = -0.5*angle/STEP;
			glColor4d(current_area_color_2D.r, current_area_color_2D.g, current_area_color_2D.b, current_area_color_2D.a);
			glVertexs(g_plus(center, Rx2D(g_multi(radius, direction),i*dtheta)));
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
			dtheta = 0.5*angle/STEP;
			glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
			glVertexs(g_plus(center, Rx2D(g_multi(radius,direction),i*dtheta)));
		}
		glVertexs(center);
		g_line_strip();
		for(i=0; i<=STEP; i++)
		{
			dtheta = -0.5*angle/STEP;
			glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
			glVertexs(g_plus(center, Rx2D(g_multi(radius, direction),i*dtheta)));
		}
		glVertexs(center);
		glEnd();
	}
}

void g_fan_3D_core(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z, //方向
		double radius,                                              //半径
		double angle, double psi,                                   //開く角度、回転角
		int FaceNumberLevel, int DivideLevel, G_WIREFILL WireFill)
{
	int i,STEP=FaceNumberLevel;
	double dtheta;
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
	G_POSITION r0,r1,r2, nx;
	r0 = g_vector3(center_x, center_y, center_z);
	nx = g_vector3(1,0,0);
	if(WireFill == 1)
	{
		for(i=0; i<STEP; i++)
		{
			dtheta = 0.5*angle/STEP;
			r1 = g_plus(r0, Ry(Rz(Rx(Ry(g_multi(radius, nx),i*dtheta),psi),phi),theta));
			r2 = g_plus(r0, Ry(Rz(Rx(Ry(g_multi(radius, nx),(i+1)*dtheta),psi),phi),theta));
			g_triangle_3D_core(r0.x,r0.y,r0.z,
					r1.x,r1.y,r1.z,
					r2.x,r2.y,r2.z,
					DivideLevel,WireFill);
		}
		for(i=0; i<STEP; i++)
		{
			dtheta = -0.5*angle/STEP;
			r1 = g_plus(r0, Ry(Rz(Rx(Ry(g_multi(radius, nx),i*dtheta),psi),phi),theta));
			r2 = g_plus(r0, Ry(Rz(Rx(Ry(g_multi(radius, nx),(i+1)*dtheta),psi),phi),theta));
			g_triangle_3D_core(r0.x,r0.y,r0.z,
					r1.x,r1.y,r1.z,
					r2.x,r2.y,r2.z,
					DivideLevel,WireFill);
		}
	}

	if(WireFill == 0)
	{	
		glEnd();
		glDisable(GL_LIGHTING);
		g_line_strip();
		for(i=0; i<=STEP; i++)
		{
			dtheta = 0.5*angle/STEP;
			glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
			glVertexs(g_plus(r0, Ry(Rz(Rx(Ry(g_multi(radius, nx),i*dtheta),psi),phi),theta)));
		}
		glVertexs(r0);
		g_line_strip();
		for(i=0; i<=STEP; i++)
		{
			dtheta = -0.5*angle/STEP;
			glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
			glVertexs(g_plus(r0, Ry(Rz(Rx(Ry(g_multi(radius, nx),i*dtheta),psi),phi),theta)));
		}
		glVertexs(r0);
		glEnd();
	}

}


void g_fan_3D(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z, //方向
		double radius,                                              //半径
		double angle, double psi,                                   //開く角度、回転角
		G_WIREFILL WireFill)
{
	g_fan_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, angle, psi, 10, 0, WireFill);
}
