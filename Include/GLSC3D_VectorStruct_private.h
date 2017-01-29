#ifndef _GLSC3D_VECTORSTRUCT_PRIVATE_H
#define _GLSC3D_VECTORSTRUCT_PRIVATE_H

#include <math.h>
#include <float.h>

#define			MyPrintf3D(name)	printf("%s.x = %f %s.y = %f %s.z = %f \n",#name,name.x,#name,name.y,#name,name.z);
#define			MyPrintf2D(name)	printf("%s.x = %f %s.y = %f \n",#name,name.x,#name,name.y);
#define			MyPrintf1D(name)	printf("%s.x = %f\n",#name,name.x);

typedef struct
{
	double          x;
}               Vector1D;
typedef struct
{
	int             i;
}               Vector1DInt;
typedef struct
{
	int             i;
	double          x;
}               Vector1DMix;
typedef struct
{
	double          x, y;
}               Vector2D;
typedef struct
{
	int             i, j;
}               Vector2DInt;
typedef struct
{
	int             i, j;
	double          x, y;
}               Vector2DMix;
typedef struct
{
	double          x, y, z;
}               Vector3D;
typedef struct
{
	int             i, j, k;
}               Vector3DInt;
typedef struct
{
	int             i, j, k;
	double          x, y, z;
}               Vector3DMix;


/********************************** 3D ****************************************/
static inline Vector3D Subs (double x,double y,double z)
{
	Vector3D w =
    {
        x,
        y,
        z
    };
    
	return w;
}
static inline Vector3D Plus (Vector3D u,Vector3D v)
{
	Vector3D w =
    {
        u.x + v.x,
        u.y + v.y,
        u.z + v.z
    };
    
	return w;
}
static inline Vector3D Minus (Vector3D u,Vector3D v)
{
	Vector3D w =
    {
        u.x - v.x,
        u.y - v.y,
        u.z - v.z
    };
    
	return w;
}
static inline Vector3D Multi (double a,Vector3D u)
{
	Vector3D w =
	{
        a * u.x,
        a * u.y,
        a * u.z
    };
    
	return w;
}
static inline Vector3D Cross (Vector3D u,Vector3D v)
{
	Vector3D w =
	{
        u.y * v.z - v.y * u.z,
        u.z * v.x - v.z * u.x,
        u.x * v.y - v.x * u.y
    };
    
	return w;
}
static inline double Dot (Vector3D u,Vector3D v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}
static inline double Norm (Vector3D u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}
static  const   Vector3D    Zero3D = {0.0,0.0,0.0};



static inline Vector2D Subs2D (double x,double y)
{
	Vector2D w =
    {
        x,
        y
    };
    
	return w;
}
static inline Vector2D Plus2D (Vector2D u,Vector2D v)
{
	Vector2D w =
    {
        u.x + v.x,
        u.y + v.y
    };
    
	return w;
}
static inline Vector2D Minus2D (Vector2D u,Vector2D v)
{
	Vector2D w =
    {
        u.x - v.x,
        u.y - v.y
    };
    
	return w;
}
static inline Vector2D Multi2D (double a,Vector2D u)
{
	Vector2D w =
	{
        a * u.x,
        a * u.y
    };
    
	return w;
}
static inline double Dot2D (Vector2D u,Vector2D v)
{
	return u.x * v.x + u.y * v.y;
}
static inline double Norm2D (Vector2D u)
{
	return sqrt(u.x * u.x + u.y * u.y);
}
static  const  Vector2D    Zero2D = {0.0,0.0};
#endif /*_GLSC3D_VECTORSTRUCT_PRIVATE_H*/


