#include "glsc3d_private.h"

void g_box_2D(double x, double y,
		double width, double height,
		G_WIREFILL WireFill)
{
	int i;
	G_VECTOR r[4],center;
	center = g_vector2(x,y);
	r[0] = g_vector2(-width/2, -height/2);
	r[1] = g_vector2(width/2, -height/2);
	r[2] = g_vector2(width/2, height/2);
	r[3] = g_vector2(-width/2, height/2);
	if(WireFill == 1)
	{
		g_begin_triangle_fan();
		for(i=0;i<4;i++)
		{
			g_emit_vertex(g_plus(center,r[i]));
		}
	}
	if(WireFill == 0)
	{
		g_begin_line_loop();
		for(i=0;i<4;i++)
		{
			g_emit_vertex(g_plus(center,r[i]));
		}
	}
}

void g_box_3D_core(double x, double y, double z,
		double width, double height, double depth,
		int DivideLevel, G_WIREFILL WireFill)
{
	G_VECTOR r0, r1, r2, r3, r4, r5, r6, r7;

	r0 = g_vector3(x-width/2., y-height/2., z-depth/2.);
	r1 = g_vector3(x+width/2., y-height/2., z-depth/2.);
	r2 = g_vector3(x-width/2., y+height/2., z-depth/2.);
	r3 = g_vector3(x+width/2., y+height/2., z-depth/2.);
	r4 = g_vector3(x-width/2., y-height/2., z+depth/2.);
	r5 = g_vector3(x+width/2., y-height/2., z+depth/2.);
	r6 = g_vector3(x-width/2., y+height/2., z+depth/2.);
	r7 = g_vector3(x+width/2., y+height/2., z+depth/2.);


	if(WireFill == 1)
	{
		g_triangle_3D_core(r0.x, r0.y, r0.z,
				r1.x, r1.y, r1.z,
				r4.x, r4.y, r4.z,
				DivideLevel, WireFill);
		g_triangle_3D_core(r1.x, r1.y, r1.z,
				r5.x, r5.y, r5.z,
				r4.x, r4.y, r4.z,
				DivideLevel, WireFill);

		g_triangle_3D_core(r6.x, r6.y, r6.z,
				r2.x, r2.y, r2.z,
				r4.x, r4.y, r4.z,
				DivideLevel, WireFill);
		g_triangle_3D_core(r4.x, r4.y, r4.z,
				r2.x, r2.y, r2.z,
				r0.x, r0.y, r0.z,
				DivideLevel, WireFill);

		g_triangle_3D_core(r2.x, r2.y, r2.z,
				r7.x, r7.y, r7.z,
				r3.x, r3.y, r3.z,
				DivideLevel, WireFill);
		g_triangle_3D_core(r6.x, r6.y, r6.z,
				r7.x, r7.y, r7.z,
				r2.x, r2.y, r2.z,
				DivideLevel, WireFill);

		g_triangle_3D_core(r1.x, r1.y, r1.z,
				r7.x, r7.y, r7.z,
				r5.x, r5.y, r5.z,
				DivideLevel, WireFill);
		g_triangle_3D_core(r1.x, r1.y, r1.z,
				r3.x, r3.y, r3.z,
				r7.x, r7.y, r7.z,
				DivideLevel, WireFill);

		g_triangle_3D_core(r0.x, r0.y, r0.z,
				r2.x, r2.y, r2.z,
				r1.x, r1.y, r1.z,
				DivideLevel, WireFill);
		g_triangle_3D_core(r2.x, r2.y, r2.z,
				r3.x, r3.y, r3.z,
				r1.x, r1.y, r1.z,
				DivideLevel, WireFill);

		g_triangle_3D_core(r6.x, r6.y, r6.z,
				r4.x, r4.y, r4.z,
				r7.x, r7.y, r7.z,
				DivideLevel, WireFill);
		g_triangle_3D_core(r5.x, r5.y, r5.z,
				r7.x, r7.y, r7.z,
				r4.x, r4.y, r4.z,
				DivideLevel, WireFill);
	}
	if(WireFill == 0)
	{
		g_move_3D(r0.x,r0.y,r0.z);
		g_plot_3D(r1.x,r1.y,r1.z);
		g_plot_3D(r3.x,r3.y,r3.z);
		g_plot_3D(r2.x,r2.y,r2.z);
		g_plot_3D(r0.x,r0.y,r0.z);

		g_move_3D(r4.x,r4.y,r4.z);
		g_plot_3D(r5.x,r5.y,r5.z);
		g_plot_3D(r7.x,r7.y,r7.z);
		g_plot_3D(r6.x,r6.y,r6.z);
		g_plot_3D(r4.x,r4.y,r4.z);

		g_move_3D(r2.x,r2.y,r2.z);
		g_plot_3D(r6.x,r6.y,r6.z);

		g_move_3D(r3.x,r3.y,r3.z);
		g_plot_3D(r7.x,r7.y,r7.z);

		g_move_3D(r5.x,r5.y,r5.z);
		g_plot_3D(r1.x,r1.y,r1.z);

		g_move_3D(r0.x,r0.y,r0.z);
		g_plot_3D(r4.x,r4.y,r4.z);
	}
}
void g_box_3D(double x, double y, double z,
		double width, double height, double depth)
{
	g_box_3D_core(x, y, z, width, height, depth, 0, G_FILL);
}
