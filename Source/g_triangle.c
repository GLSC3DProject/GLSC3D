#include "glsc3d_private.h"
#include <stdio.h>

//This will be use g_triangle_core.
void g_triangle_3D_core_worker(double x0, double y0, double z0,
		double x1, double y1, double z1,
		double x2, double y2, double z2,
		int DivideLevel)
{
	G_POSITION r0, r1, r2, r3, r4, r5;

	r0 = g_vector3(x0,y0,z0);
	r1 = g_vector3(x1,y1,z1);
	r2 = g_vector3(x2,y2,z2);

	if(DivideLevel >= 1)
	{
		r3 = g_multi(0.5,g_plus(r0,r1));
		r4 = g_multi(0.5,g_plus(r1,r2));
		r5 = g_multi(0.5,g_plus(r2,r0));

		g_triangle_3D_core_worker(r0.x,r0.y,r0.z,
				r3.x,r3.y,r3.z,
				r5.x,r5.y,r5.z,
				DivideLevel-1);
		g_triangle_3D_core_worker(r1.x,r1.y,r1.z,
				r4.x,r4.y,r4.z,
				r3.x,r3.y,r3.z,
				DivideLevel-1);
		g_triangle_3D_core_worker(r2.x,r2.y,r2.z,
				r5.x,r5.y,r5.z,
				r4.x,r4.y,r4.z,
				DivideLevel-1);
		g_triangle_3D_core_worker(r3.x,r3.y,r3.z,
				r5.x,r5.y,r5.z,
				r4.x,r4.y,r4.z,
				DivideLevel-1);
	}else{
		G_TRIANGLE t0;
		G_MATERIAL m = g_make_material(current_area_color_3D);
		G_VERTEX   v0, v1, v2;
		G_POSITION n;

		n = g_cross(g_minus(r1, r0),g_minus(r2, r0));

		/*if(g_norm(n) < DBL_EPSILON)
		  {
		  fprintf(stderr, "Err Occurs!! In function g_triangle_3D\a\n");
		  fprintf(stderr, "Triangle is too small.\n");
		  return;
		  }
		  else {n = g_multi(1 / g_norm(n),n);}
		 */
		n = g_multi(1 / g_norm(n),n);
		v0 = g_material_vertex(r0,n,m);/*位置,法線,マテリアル*/
		v1 = g_material_vertex(r1,n,m);
		v2 = g_material_vertex(r2,n,m);
		t0 = g_make_triangle_core(v0, v1, v2);
		g_set_triangle(t0);
	}
}

void g_triangle_3D_core(double x0, double y0, double z0,
		double x1, double y1, double z1,
		double x2, double y2, double z2,
		int DivideLevel, G_WIREFILL WireFill)
{
	if(WireFill == G_FILL)
	{
		glEnd();
		glEnable(GL_LIGHTING);
		g_triangle_3D_core_worker(x0, y0, z0,
				x1, y1, z1,
				x2, y2, z2,
				DivideLevel);
		glEnd();
	}
	if(WireFill == G_WIRE)
	{
		glEnd();
		glColor4d(current_line_color.r,current_line_color.g,current_line_color.b,current_line_color.a);
		glDisable(GL_LIGHTING);
		G_POSITION r0, r1, r2;
		r0 = g_vector3(x0,y0,z0);
		r1 = g_vector3(x1,y1,z1);
		r2 = g_vector3(x2,y2,z2);

		g_line_loop();
		glVertexs(r0);
		glVertexs(r1);
		glVertexs(r2);
		glEnd();
	}
}


void g_triangle_3D(double x0, double y0, double z0,
		double x1, double y1, double z1,
		double x2, double y2, double z2,
		G_WIREFILL WireFill)
{
	g_triangle_3D_core(x0, y0, z0,
			x1, y1, z1,
			x2, y2, z2,
			0, WireFill);
}

void g_set_material(GLenum face, G_MATERIAL mat)
{
	glMaterialfv(face, GL_AMBIENT, &mat.ambient.r);
	glMaterialfv(face, GL_DIFFUSE, &mat.diffuse.r);
	glMaterialfv(face, GL_SPECULAR, &mat.specular.r);
	glMaterialfv(face, GL_EMISSION, &mat.emission.r);
	glMaterialf(face, GL_SHININESS, mat.shininess);
}

void g_triangle_terminal(G_TRIANGLE t)
{
	glEnd();
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 3; ++i)
	{
		g_set_material(GL_FRONT, t.vertex[i].front);
		g_set_material(GL_BACK, t.vertex[i].back);
		glNormals(t.vertex[i].normal);
		glVertexs(t.vertex[i].position);
	}
	glEnd();
}

void g_triangle_2D(double x0, double y0,
                   double x1, double y1,
                   double x2, double y2,
                   G_WIREFILL WireFill)
{
    G_VECTOR r0,r1,r2;
    
    r0 = g_vector2(x0,y0);
    r1 = g_vector2(x1,y1);
    r2 = g_vector2(x2,y2);
	if(WireFill ==1)
    {
        glEnd();
        glDisable(GL_LIGHTING);
        g_triangles();
        glColor4d(current_area_color_2D.r, current_area_color_2D.g, current_area_color_2D.b, current_area_color_2D.a);
        glVertexs(r0);
        glVertexs(r1);
        glVertexs(r2);
        glEnd();
    }
    if(WireFill ==0)
    {
        glEnd();
        glDisable(GL_LIGHTING);
        glColor4d(current_line_color.r, current_line_color.g, current_line_color.b, current_line_color.a);
        g_line_loop();
        glVertexs(r0);
        glVertexs(r1);
        glVertexs(r2);
        glEnd();
    }
}


