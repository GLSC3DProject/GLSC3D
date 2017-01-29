#include "glsc3d_private.h"

G_LIGHT g_make_light_core(GLenum light_id, G_COLOR ambient, G_COLOR diffuse, G_COLOR specular, G_POSITION position, G_POSITION direction, float exponent, float cutoff, float constant_attenuation, float linear_attenuation, float quadratic_attenuation)
{
	G_LIGHT light = {light_id, ambient, diffuse, specular, position, direction, exponent, cutoff, constant_attenuation, linear_attenuation, quadratic_attenuation};
	return light;
}

G_LIGHT g_make_light(GLenum light_id, G_COLOR ambient_and_diffuse, G_COLOR specular, G_POSITION position)
{
	return g_make_light_core(light_id, g_color_core(ambient_and_diffuse.r*0.25, ambient_and_diffuse.g*0.25, ambient_and_diffuse.b*0.25, ambient_and_diffuse.a), ambient_and_diffuse, specular, position, g_position(-position.x, -position.y, -position.z), 0, 180, 1, 0, 0);
}

G_LIGHT g_make_light_easy(GLenum light_id, G_POSITION position)
{
	return g_make_light(light_id, g_color(1, 1, 1), g_color(1, 1, 1), position);
}

void g_set_light(G_LIGHT l)
{
	const float ambient[] = {l.ambient.r, l.ambient.g, l.ambient.b, l.ambient.a};
	glLightfv(l.light_id, GL_AMBIENT, ambient);

	const float diffuse[] = {l.diffuse.r, l.diffuse.g, l.diffuse.b, l.diffuse.a};
	glLightfv(l.light_id, GL_DIFFUSE, diffuse);

	const float specular[] = {l.specular.r, l.specular.g, l.specular.b, l.specular.a};
	glLightfv(l.light_id, GL_SPECULAR, specular);

	const float position[] = {l.position.x, l.position.y, l.position.z, l.position.w};
	glLightfv(l.light_id, GL_POSITION, position);

	const float direction[] = {l.direction.x, l.direction.y, l.direction.z, l.direction.w};
	glLightfv(l.light_id, GL_SPOT_DIRECTION, direction);

	glLightf(l.light_id, GL_SPOT_EXPONENT,         l.exponent);
	glLightf(l.light_id, GL_SPOT_CUTOFF,           l.cutoff);
	glLightf(l.light_id, GL_CONSTANT_ATTENUATION,  l.constant_attenuation);
	glLightf(l.light_id, GL_LINEAR_ATTENUATION,    l.linear_attenuation);
	glLightf(l.light_id, GL_QUADRATIC_ATTENUATION, l.quadratic_attenuation);

	glEnable(l.light_id);
}

void g_remove_light(G_LIGHT light)
{
	glDisable(light.light_id);
}

