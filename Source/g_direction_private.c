#include "glsc3d_private.h"
/***************************direction********************************************************/
G_REAL g_direction_phi(G_VECTOR v)
{
	return atan2(v.y, sqrt(v.x*v.x+v.z*v.z));
}

G_REAL g_direction_theta(G_VECTOR v)
{
	return atan2(v.z, v.x);
}
