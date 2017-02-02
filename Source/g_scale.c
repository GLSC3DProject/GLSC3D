#include "glsc3d_private.h"
#include <stdio.h>

int             get_scale_id_number;
G_DIMENSION     g_scale_dim_flag;

G_CAMERA glsc3D_inner_camera[TotalDisplayNumber];
G_SCREEN glsc3D_inner_screen[TotalDisplayNumber];
G_CAMERA current_camera;

int g_get_scale_id(G_DIMENSION *g_dim)
{
	*g_dim = g_scale_dim_flag;
	return get_scale_id_number;
}

void g_def_scale_2D(int id,                                                       //id
					double x_left, double x_right, double y_bottom, double y_top, //仮想座標系
					double x_left_std, double y_top_std,                            //ウィンドウの位置
					double width_std, double height_std)                          //ウィンドウのサイズ
{
	glsc3D_inner_camera[id] = g_make_camera_like_scale_2D(x_left, x_right, y_bottom, y_top);
	glsc3D_inner_screen[id] = g_make_screen(x_left_std, y_top_std, width_std, height_std);
	if(id >= 100){
		fprintf(stderr,"too large id number\n");
		exit(0);
	}
}

void g_sel_scale_2D(int id)
{
	g_vertex_buffer_flush();
	g_triangle_buffer_flush();

	glDisable(GL_DEPTH_TEST);
	
	g_scale_dim_flag = G_2D;
	get_scale_id_number = id;

	g_set_camera(glsc3D_inner_camera[id], glsc3D_inner_screen[id]);
}

void g_def_scale_3D(int id,
					double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far, //仮想座標系
					double x_left_std, double y_top_std,                          //ウィンドウの位置
					double width_std, double height_std,                          //ウィンドウのサイズ
					double direction_x, double direction_y, double direction_z,
					double r)                                                     //視点位置
{
	g_def_scale_3D_core(id,x_left,x_right,y_bottom,y_top,z_near,z_far,x_left_std,y_top_std,width_std,height_std,direction_x,direction_y,direction_z,r,0,1,0);
}

void g_def_scale_3D_core(int id,
						 double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far,
						 double x_left_std, double y_top_std,
						 double width_std, double height_std,
						 double direction_x, double direction_y, double direction_z,
						 double r,
						 double up_x, double up_y, double up_z)
{
	glsc3D_inner_camera[id] = g_make_camera_like_scale_3D_core(x_left, x_right, y_bottom, y_top, z_near, z_far, direction_x, direction_y, direction_z, r, up_x, up_y, up_z);
	glsc3D_inner_screen[id] = g_make_screen(x_left_std, y_top_std, width_std, height_std);
	if(id >= 100){
		fprintf(stderr,"too large id number\n");
		exit(0);
	}
}

void g_sel_scale_3D(int id)
{
	g_vertex_buffer_flush();
	g_triangle_buffer_flush();
	
	glEnable(GL_DEPTH_TEST);
	
	g_scale_dim_flag = G_3D;
	get_scale_id_number = id;
	
	g_set_camera(glsc3D_inner_camera[id], glsc3D_inner_screen[id]);
}

