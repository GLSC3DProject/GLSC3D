#include "glsc3d_private.h"
void g_cone_2D(double center_x, double center_y,       //底面の中心座標
		double direction_x, double direction_y, //方向
		double radius, double head_size,        //半径、高さ
		int type)
{
	G_VECTOR n, center;
	n = g_vector2(direction_x, direction_y);
	n = g_multi(1/g_norm(n), n);
	center = g_vector2(center_x, center_y);
	double side = sqrt(head_size*head_size + radius*radius);
	if(type == 1)
	{
		glEnd();
		glDisable(GL_LIGHTING);
		g_triangles();
		glColor4d(current_area_color_2D.r, current_area_color_2D.g, current_area_color_2D.b, current_area_color_2D.a);
		glVertexs(g_plus(center, g_multi(head_size, n)));
		glVertexs(g_minus(g_plus(center, g_multi(head_size, n)), Rx2D(g_multi(side, n), atan2(radius, head_size))));
		glVertexs(g_minus(g_plus(center, g_multi(head_size, n)), Rx2D(g_multi(side, n), -atan2(radius, head_size))));
		glEnd();
	}
	if(type == 0)
	{
		glEnd();
		glDisable(GL_LIGHTING);
		g_line_loop();
		glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
		glVertexs(g_plus(center,g_multi(head_size, n)));
		glVertexs(g_minus(g_plus(center, g_multi(head_size, n)),Rx2D(g_multi(side, n), atan2(radius, head_size))));
		glVertexs(g_minus(g_plus(center, g_multi(head_size, n)),Rx2D(g_multi(side, n), -atan2(radius, head_size))));
		glEnd();
	}
	if(type == 2)
	{
		glEnd();
		glDisable(GL_LIGHTING);
		g_line_strip();
		glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
		glVertexs(g_minus(g_plus(center,g_multi(head_size, n)),Rx2D(g_multi(side, n), atan(radius/head_size))));
		glVertexs(g_plus(center,g_multi(head_size, n)));
		glVertexs(g_minus(g_plus(center,g_multi(head_size, n)),Rx2D(g_multi(side, n), -atan(radius/head_size))));
		glEnd();
	}
}

void g_cone_3D_core(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z, //方向
		double radius, double head_size,
		int FaceNumberLevel_X, int FaceNumberLevel_Y, int DivideLevel, G_WIREFILL WireFill)
{
	int i,j,n = FaceNumberLevel_X,stack = FaceNumberLevel_Y;
	G_VECTOR top,r,r0,r1,r2,r3,center;
	double alpha,dth,stack_size=1/(double)stack;
	double phi = g_direction_phi(g_vector3(direction_x, direction_y, direction_z));
	double theta = g_direction_theta(g_vector3(direction_x, direction_y, direction_z));
	dth = 2*PI/n;
	center = g_vector3(center_x,center_y,center_z);
	alpha = -atan(radius/head_size);
	top = g_vector3(head_size, 0, 0);
	r = g_vector3(0,radius,0);

	//r0 = Heiko3Ds(Ry(Rz(r,phi),theta),center);
	r0 = center;
	for(i=0;i<=n;i++)
	{
		r1 = Heiko3Ds(Ry(Rz(Rx(r,i*dth),phi),theta),center);
		r2 = Heiko3Ds(Ry(Rz(Rx(r,(i+1)*dth),phi),theta),center);
		if(WireFill==1)
		{
			g_triangle_3D_core(r0.x,r0.y,r0.z,
					r1.x,r1.y,r1.z,
					r2.x,r2.y,r2.z,
					DivideLevel,WireFill);
		}
		if(WireFill==0)
		{
			g_move_3D(r1.x,r1.y,r1.z);
			g_plot_3D(r2.x,r2.y,r2.z);
		}

	}

	for(j=0;j<stack;j++)
	{
		for(i=0;i<=n;i++)
		{
			r0 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(g_multi(j*stack_size,r),g_multi(1-j*stack_size,top)),i*dth),phi),theta),center);
			r1 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(g_multi((j+1)*stack_size,r),g_multi(1-(j+1)*stack_size,top)),i*dth),phi),theta),center);
			r2 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(g_multi(j*stack_size,r),g_multi(1-j*stack_size,top)),(i+1)*dth),phi),theta),center);
			r3 = Heiko3Ds(Ry(Rz(Rx(Heiko3Ds(g_multi((j+1)*stack_size,r),g_multi(1-(j+1)*stack_size,top)),(i+1)*dth),phi),theta),center);

			if(WireFill==1)
			{
				glEnd();
				glEnable(GL_LIGHTING);
				G_VECTOR   n0,n1,n2,n3;
				G_VERTEX   v0, v1, v2, v3;
				G_MATERIAL m = g_make_material(current_area_color_3D);
				G_TRIANGLE t0, t1;

				n0 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i-0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta));
				n0 = g_multi(1/g_norm(n0),n0);
				n1 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i-0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta));
				n1 = g_multi(1/g_norm(n1),n1);
				n2 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+1.5)*dth),phi),theta));
				n2 = g_multi(1/g_norm(n2),n2);
				n3 = g_plus(Ry(Rz(Rx(Rz(r,alpha),(i+0.5)*dth),phi),theta),Ry(Rz(Rx(Rz(r,alpha),(i+1.5)*dth),phi),theta));
				n3 = g_multi(1/g_norm(n3),n3);

				v0 = g_material_vertex(r0,n0,m);
				v1 = g_material_vertex(r1,n1,m);
				v2 = g_material_vertex(r2,n2,m);
				v3 = g_material_vertex(r3,n3,m);

				t0 = g_make_triangle_core(v0, v1, v2);
				t1 = g_make_triangle_core(v1, v3, v2);
				g_set_triangle(t0);
				g_set_triangle(t1);
				glEnd();
			}
			if(WireFill==0)
			{
				glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
				g_move_3D(r0.x,r0.y,r0.z);
				g_plot_3D(r1.x,r1.y,r1.z);
				g_move_3D(r2.x,r2.y,r2.z);
				g_plot_3D(r3.x,r3.y,r3.z);
			}
		}
	}
}

void g_cone_3D(double center_x, double center_y, double center_z,          //中心座標
		double direction_x, double direction_y, double direction_z, //方向
		double radius,double head_size)                             //半径、高さ
{
	g_cone_3D_core(center_x, center_y, center_z, direction_x, direction_y, direction_z, radius, head_size, 20, 10, 0, G_FILL);
}
