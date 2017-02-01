#include "glsc3d_private.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

G_SCREEN g_make_screen(int x, int y, int width, int height)
{
	G_SCREEN screen = {x, y, width, height};
	return screen;
}

G_CAMERA g_make_camera_core(
		double fovy,    double near,    double far,
		double eyeX,    double eyeY,    double eyeZ, 
		double centerX, double centerY, double centerZ, 
		double upX,     double upY,     double upZ)
{
	double dot = (eyeX - centerX)*upX + (eyeY - centerY)*upY + (eyeZ - centerZ)*upZ;
	dot *= 1/((eyeX - centerX)*(eyeX - centerX) + (eyeY - centerY)*(eyeY - centerY) + (eyeZ - centerZ)*(eyeZ - centerZ));
	G_CAMERA camera = {fovy, NAN, near, far, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX - dot*(eyeX - centerX), upY - dot*(eyeY - centerY), upZ - dot*(eyeZ - centerZ), 1};
    /* この500という数字だが，ハードウェアの仕様上，この数字より極端に大きな数字にすると，表示がおかしく見える．ただし，将来的に，GPUによる倍精度計算が一般化すれば，この制限は撤廃，或いは緩和される可能性がある． */
    if(far/near > 500)
    {
        fprintf(stderr, 
                "For near and far in G_CAMERA, far/near is too big!\n"
                "GLSC3D will abort¥n");
        
        exit(1);
    }
    
	return camera;
}

G_CAMERA g_make_camera(
		double eyeX,    double eyeY,    double eyeZ, 
		double centerX, double centerY, double centerZ)
{
	return g_make_camera_core(45, 0.1, 10, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 0, 1);
}

G_CAMERA g_make_camera_2D(double X, double Y)
{
	return g_make_camera_core(45, 0.1, 10, X, Y, 1, X, Y, 0, 0, 1, 0);
}

G_CAMERA g_make_camera_like_scale_3D_core(double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far, double direction_x, double direction_y, double direction_z, double r, double up_x, double up_y, double up_z)
{
	double centerX = (x_left + x_right)*0.5, centerY = (y_bottom + y_top)*0.5, centerZ = (z_near + z_far)*0.5;
	double d_l = sqrt(direction_x*direction_x + direction_y*direction_y + direction_z*direction_z);
	double sphere_r = sqrt((x_right - x_left)*(x_right - x_left) + (y_top - y_bottom)*(y_top - y_bottom) + (z_near - z_far)*(z_near - z_far))/2;
	double R = (r > sphere_r*1.4) ? r : sphere_r*1.4;
	double eyeX = centerX + R*direction_x/d_l, eyeY = centerY + R*direction_y/d_l, eyeZ = centerZ + R*direction_z/d_l;
	double fovy = 2*asin(sphere_r/R)/(2*M_PI)*360;

	return g_make_camera_core(fovy, R*0.1, R + sphere_r, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, up_x, up_y, up_z);
}

G_CAMERA g_make_camera_like_scale_3D(double x_left, double x_right, double y_bottom, double y_top, double z_near, double z_far, double direction_x, double direction_y, double direction_z, double r)
{
	return g_make_camera_like_scale_3D_core(x_left, x_right, y_bottom, y_top, z_near, z_far, direction_x, direction_y, direction_z, r, 0, 1, 0);
}

G_CAMERA g_make_camera_like_scale_2D(double x_left, double x_right, double y_bottom, double y_top)
{
	G_CAMERA c = g_make_camera_core(2*atan2((y_top - y_bottom)/2, 1)/(2*M_PI)*360, 0.1, 2, (x_right + x_left)/2, (y_top + y_bottom)/2, 1, (x_left + x_right)/2, (y_top + y_bottom)/2, 0, 0, 1, 0);
	c.mirror *= ((x_left > x_right || y_bottom > y_top) && !(x_left > x_right && y_bottom > y_top)? -1 : 1);
	c.aspect = fabs((x_right - x_left)/(y_top - y_bottom));
	return c;
}

void g_set_camera(G_CAMERA c, G_SCREEN s)
{
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(c.fovy, (c.aspect == c.aspect ? c.aspect : s.width/(double)s.height)*c.mirror, c.near, c.far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(c.eyeX, c.eyeY, c.eyeZ, c.centerX, c.centerY, c.centerZ, c.upX, c.upY, c.upZ);
	glViewport(s.x, glsc3D_height - s.height - s.y, s.width, s.height);
}

void g_camera_rotate(G_CAMERA *cam, G_ROTATE_DIRECTION dire, G_ROTATE_ANCHOR anc, double theta)
{
	G_VECTOR up = g_vector3(cam->upX, cam->upY, cam->upZ);
	G_VECTOR base, satellite, n;

	switch(anc)
	{
		case G_EYE:
			base = g_vector3(cam->eyeX, cam->eyeY, cam->eyeZ);
			satellite = g_vector3(cam->centerX, cam->centerY, cam->centerZ);
			break;

		case G_CENTER:
			base = g_vector3(cam->centerX, cam->centerY, cam->centerZ);
			satellite = g_vector3(cam->eyeX, cam->eyeY, cam->eyeZ);
			break;

		default:
			return;
	}

	switch(dire)
	{
		case G_PITCH:
			n = g_multi(1/g_norm(g_cross(g_minus(satellite, base), up)), g_cross(g_minus(satellite, base), up));
			satellite = g_plus(base, Rn(g_minus(satellite, base), n, theta));
			up = Rn(up, n, theta);
			break;

		case G_YAW:
			n = g_multi(1/g_norm(up), up);
			satellite = g_plus(base, Rn(g_minus(satellite, base), n, theta));
			break;

		case G_ROLL:
			n = g_multi(1/g_norm(g_minus(satellite, base)), g_minus(satellite, base));
			up = Rn(up, n, theta);
			break;

		default:
			return;
	}

	switch(anc)
	{
		case G_EYE:
			cam->centerX = satellite.x;
			cam->centerY = satellite.y;
			cam->centerZ = satellite.z;
			break;

		case G_CENTER:
			cam->eyeX = satellite.x;
			cam->eyeY = satellite.y;
			cam->eyeZ = satellite.z;
			break;

	}

	cam->upX = up.x;
	cam->upY = up.y;
	cam->upZ = up.z;
			
}
