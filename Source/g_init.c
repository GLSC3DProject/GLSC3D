#include "glsc3d_private.h"
#include <stdio.h>
#include <stdlib.h>

int             glsc3D_width =  800;
int             glsc3D_height = 800;
int             glsc3D_argc = 1;
char            **glsc3D_argv;
void            *font;

G_COLOR current_area_color_2D = {1, 1, 1, 1};
G_COLOR current_area_color_3D = {1, 1, 1, 1};
G_COLOR current_line_color = {1, 1, 1, 1};
G_COLOR current_text_color = {1, 1, 1, 1};
G_COLOR current_marker_color = {1, 1, 1, 1};

int g_enable_transparent, TRIANGLE_BUFFER_SIZE, TEMPORARY_TRIANGLE_BUFFER_SIZE;

void g_init_light_core(int lightnum,double lit_pos_x,double lit_pos_y,double lit_pos_z, double lit_pow)
{
	static GLfloat lit_amb[4]={0.3, 0.3, 0.3, 0.0};	// 環境光の強さ
	GLfloat lit_dif[4]={lit_pow, lit_pow, lit_pow, 0.0};	// 拡散光の強さ
	static GLfloat lit_spc[4]={1.0, 1.0, 1.0, 0.0};	// 鏡面反射光の強さ
	static GLfloat lit_pos[4]={0.0, 0.0, 0.0, 0.0}; // 光源の位置
	int lightname=0;
	
	if (lightnum==0) {
		lightname = GL_LIGHT0;
	}else if (lightnum==1) {
		lightname = GL_LIGHT1;
	}else if (lightnum==2) {
		lightname = GL_LIGHT2;
	}else if (lightnum==3) {
		lightname = GL_LIGHT3;
	}else if (lightnum==4) {
		lightname = GL_LIGHT4;
	}else if (lightnum==5) {
		lightname = GL_LIGHT5;
	}else if (lightnum==6) {
		lightname = GL_LIGHT6;
	}else if (lightnum==7) {
		lightname = GL_LIGHT7;
	}

	glLightfv(lightname, GL_AMBIENT, lit_amb);
	glLightfv(lightname, GL_DIFFUSE, lit_dif);
	glLightfv(lightname, GL_SPECULAR, lit_spc);
	lit_pos[0] = lit_pos_x;
	lit_pos[1] = lit_pos_y;
	lit_pos[2] = lit_pos_z;
	glLightfv(lightname, GL_POSITION, lit_pos);

	glEnable(lightname);
	glEnable(GL_LIGHTING);
}
void g_init_light(int lightnum,double lit_pos_x,double lit_pos_y,double lit_pos_z)
{
	g_init_light_core(lightnum,lit_pos_x,lit_pos_y,lit_pos_z, 1.0);
}

void g_init_core(
					const char *WindowName,int width,int height,
					int pos_x,int pos_y,
					double r,double g,double b,
					int g_enable_transparent_out,
					int TEMPORARY_TRIANGLE_BUFFER_SIZE_out,
					int TRIANGLE_BUFFER_SIZE_out
				)
{
	glsc3D_width = width;
	glsc3D_height = height;
	
	glsc3D_argc = 1;
	glsc3D_argv = NULL;
	glutInit(&glsc3D_argc,glsc3D_argv);
	glutInitWindowPosition(pos_x,pos_y);
	glutInitWindowSize(glsc3D_width,glsc3D_height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | (g_enable_transparent ? 0 : GLUT_DEPTH));
	glutCreateWindow(WindowName);

	g_input_init();
	g_text_init();
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	
	glMatrixMode(GL_PROJECTION);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
//    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);

	g_init_light(0, 1, 1, 1);

	g_enable_transparent = g_enable_transparent_out;

	if(g_enable_transparent)
	{
		glEnable(GL_BLEND);
		
		if(TEMPORARY_TRIANGLE_BUFFER_SIZE_out == 0)
		{
			TEMPORARY_TRIANGLE_BUFFER_SIZE = 1 << 10;
		}
		else
		{
			TEMPORARY_TRIANGLE_BUFFER_SIZE = TEMPORARY_TRIANGLE_BUFFER_SIZE_out;
		}
		if(TRIANGLE_BUFFER_SIZE_out == 0)
		{
			TRIANGLE_BUFFER_SIZE = 1 << 20;
		}
		else
		{
			TRIANGLE_BUFFER_SIZE = TRIANGLE_BUFFER_SIZE_out;
		}
		
		g_triangle_buffer_init();
	}
	
	g_vertex_buffer_init();

	g_def_scale_3D_core(0, -1,1,-1,1,-1,1,0,0,width,height,1,1,1,0,0,1,0);
	/*glEnd();
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  gluPerspective(45,
	  width/height,
	  1,10);
	  glMatrixMode(GL_MODELVIEW);
	  glLoadIdentity();
	  gluLookAt(2, 2, 2,
	  0, 0, 0,
	  0,1,0);
	  glViewport(0, 0,
	  width, height);
	 */
//	if(!g_enable_transparent)
//	{
//		glEnable(GL_DEPTH_TEST);
//	}

	g_scr_color(r,g,b);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_finish();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	font = GLUT_BITMAP_HELVETICA_10;
	
	if(WindowName == G_OFF_SCREEN)
	{
		g_init_off_screen_rendering();
		g_begin_off_screen_rendering();
		glutHideWindow();
	}
}

void g_init (const char *WindowName,int width,int height)
{
	g_init_core(WindowName,width,height,50,50,0,0,0,0,0,0);
}
