#include "glsc3d_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

G_SCREEN g_make_screen(int x, int y, int width, int height)
{
	G_SCREEN screen = {x, y, width, height};
	return screen;
}

G_CAMERA g_make_camera_3D_core(G_VECTOR lower, G_VECTOR upper, G_VECTOR direction, float r, float aspect, G_VECTOR up)
{
	G_VECTOR center = 0.5f * (lower + upper);
	float sphere_r = g_norm(upper - lower) / 2;
	float R = (r > sphere_r*1.4) ? r : sphere_r*1.4;
	G_VECTOR eye = center + R * direction;

	G_CAMERA c;
	c.proj = G_MATRIX::Perspective(2*asin(sphere_r/R), aspect, R*0.25f, R + sphere_r);
	c.view = G_MATRIX::LookAt(eye, center, up);
	return c;
}

G_CAMERA g_make_camera_2D(float x_left, float x_right, float y_bottom, float y_top)
{
	G_CAMERA c;
	c.proj = G_MATRIX::Ortho(x_left, x_right, y_bottom, y_top, 1, -1);
	c.view = G_MATRIX::Identity();
	return c;
}

/*
void g_camera_rotate(G_CAMERA *cam, G_ROTATE_DIRECTION dire, G_ROTATE_ANCHOR anc, float theta)
{
	G_VECTOR up = cam->up;
	G_VECTOR base, satellite, n;

	switch(anc)
	{
		case G_EYE:
			base = cam->eye;
			satellite = cam->center;
			break;

		case G_CENTER:
			base = cam->center;
			satellite = cam->eye;
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
			cam->center = satellite;
			break;

		case G_CENTER:
			cam->eye = satellite;
			break;

	}

	cam->up = up;
}*/
