#include "glsc3d_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

G_SCREEN g_make_screen(int x, int y, int width, int height)
{
	G_SCREEN screen = {x, y, width, height};
	return screen;
}

//G_CAMERA g_make_camera_core(
//		float fovy,    float near,    float far,,
//		float eyeX,    float eyeY,    float eyeZ, 
//		float centerX, float centerY, float centerZ, 
//		float upX,     float upY,     float upZ)
//{
//	G_CAMERA camera = {fovy, NAN, near, far, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ};
//    if(far/near > 500)
//    {
//        fprintf(stderr, 
//                "For near and far in G_CAMERA, far/near is too big!\n"
//                "GLSC3D will abort¥n");
//        
//        exit(1);
//    }
//    
//	return camera;
//}

//G_CAMERA g_make_camera(
//		float eyeX,    float eyeY,    float eyeZ, 
//		float centerX, float centerY, float centerZ)
//{
//	return g_make_camera_core(45, 0.1, 10, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 0, 1);
//}

G_CAMERA g_make_camera_3D_core(G_VECTOR lower, G_VECTOR upper, G_VECTOR direction, float r, float aspect, G_VECTOR up)
{
	G_VECTOR center = 0.5f * (lower + upper);
	float sphere_r = g_norm(upper - lower) / 2;
	float R = (r > sphere_r*1.4) ? r : sphere_r*1.4;
	G_VECTOR eye = center + R * direction;

	G_CAMERA c;
	c.proj = G_MATRIX::Perspective(2*asin(sphere_r/R), aspect, R*0.5f, R + sphere_r);
	c.view = G_MATRIX::LookAt(eye, center, up);
	c.eye = eye;
	return c;
}

G_CAMERA g_make_camera_2D(float x_left, float x_right, float y_bottom, float y_top)
{
	G_CAMERA c;
	//c.fovy = 2*atan((y_top - y_bottom)/2);
	//c.aspect = fabs((x_right - x_left)/(y_top - y_bottom));
	//c.near = 0.1f;
	//c.far = 2;
	//c.eye = g_vector((x_right + x_left)/2, (y_top + y_bottom)/2, 1);
	//c.center = g_vector((x_left + x_right)/2, (y_top + y_bottom)/2, 0);
	//c.up = g_vector_unit_y;
	c.proj = G_MATRIX::Ortho(x_left, x_right, y_bottom, y_top, 1, -1);
	c.view = G_MATRIX::Identity();
	c.eye = G_VECTOR(0, 0, 1);
	return c;
}

//void g_camera_rotate(G_CAMERA *cam, G_ROTATE_DIRECTION dire, G_ROTATE_ANCHOR anc, float theta)
//{
//	G_VECTOR up = cam->up;
//	G_VECTOR base, satellite, n;
//
//	switch(anc)
//	{
//		case G_EYE:
//			base = cam->eye;
//			satellite = cam->center;
//			break;
//
//		case G_CENTER:
//			base = cam->center;
//			satellite = cam->eye;
//			break;
//
//		default:
//			return;
//	}
//
//	switch(dire)
//	{
//		case G_PITCH:
//			n = g_multi(1/g_norm(g_cross(g_minus(satellite, base), up)), g_cross(g_minus(satellite, base), up));
//			satellite = g_plus(base, Rn(g_minus(satellite, base), n, theta));
//			up = Rn(up, n, theta);
//			break;
//
//		case G_YAW:
//			n = g_multi(1/g_norm(up), up);
//			satellite = g_plus(base, Rn(g_minus(satellite, base), n, theta));
//			break;
//
//		case G_ROLL:
//			n = g_multi(1/g_norm(g_minus(satellite, base)), g_minus(satellite, base));
//			up = Rn(up, n, theta);
//			break;
//
//		default:
//			return;
//	}
//
//	switch(anc)
//	{
//		case G_EYE:
//			cam->center = satellite;
//			break;
//
//		case G_CENTER:
//			cam->eye = satellite;
//			break;
//
//	}
//
//	cam->up = up;
//}
