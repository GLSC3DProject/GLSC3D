#ifndef _VECTORSTRUCTOPERATOR_H
#define _VECTORSTRUCTOPERATOR_H
#include <math.h>

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

#ifdef __cplusplus

/********************************** 3D ****************************************/
inline Vector3D operator + (Vector3D u,Vector3D v)
{
	Vector3D w =
    {
        u.x + v.x,
        u.y + v.y,
        u.z + v.z
    };
    
	return w;
}
inline Vector3D& operator += (Vector3D& u,Vector3D v)
{
    u.x += v.x;
    u.y += v.y;
    u.z += v.z;
    
    return u;
}
inline Vector3D operator - (Vector3D u,Vector3D v)
{
	Vector3D w = 
    {
        u.x - v.x,
        u.y - v.y,
        u.z - v.z,
	};
    
	return w;
}
inline Vector3D& operator -= (Vector3D& u,Vector3D v)
{
    u.x -= v.x;
    u.y -= v.y;
    u.z -= v.z;
    
    return u;
}
inline Vector3D operator * (double a,Vector3D u)
{
	Vector3D w =
    {
        a * u.x,
        a * u.y,
        a * u.z,
    };
	
	return w;
}
inline Vector3D operator * (Vector3D u,double a)
{
	Vector3D w =
    {
        a * u.x,
        a * u.y,
        a * u.z,
    };
	
	return w;
}
inline Vector3D& operator - (Vector3D& u)
{	
	u.x *= - 1;
	u.y *= - 1;
    u.z *= - 1;
	
	return u;
}
inline Vector3D operator / (Vector3D u,double a)
{
	Vector3D w =
	{
        u.x / a,
        u.y / a,
        u.z / a,
    };
	
	return w;
}
inline Vector3D& operator /= (Vector3D& u,double a)
{	
	u.x /= a;
	u.y /= a;
    u.z /= a;
	
	return u;
}

/* Naiseki */
inline double operator * (Vector3D u,Vector3D v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}
inline double Dot3D (Vector3D u,Vector3D v)
{
	return u * v;
}
/* Gaiseki */
inline Vector3D operator ^ (Vector3D u,Vector3D v)
{
	Vector3D w =
	{
        u.y * v.z - v.y * u.z,
        u.z * v.x - v.z * u.x,
        u.x * v.y - v.x * u.y
    };
	
	return w;
}
inline Vector3D Cross3D (Vector3D u,Vector3D v)
{
    return u ^ v;
}
/* Norm */
inline double operator ~ (Vector3D u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}
inline double Norm3D (Vector3D u)
{
	return ~u;
}
/* Zero vec */
/*
inline Vector3D Zero3D(void)
{
	Vector3D w =
    {
        0.0,
        0.0,
        0.0
    };
	
	return w;
}
*/
const   static  Vector3D    Zero3D = {0.0,0.0,0.0};
/********************************** 2D ****************************************/
inline Vector2D operator + (Vector2D u,Vector2D v)
{
	Vector2D w =
    {
        u.x + v.x,
        u.y + v.y
    };
    
	return w;
}
inline Vector2D& operator += (Vector2D& u,Vector2D v)
{
    u.x += v.x;
    u.y += v.y;
    
    return u;
}
inline Vector2D operator - (Vector2D u,Vector2D v)
{
	Vector2D w = 
    {
        u.x - v.x,
        u.y - v.y
	};
    
	return w;
}
inline Vector2D& operator -= (Vector2D& u,Vector2D v)
{
    u.x -= v.x;
    u.y -= v.y;
    
    return u;
}
inline Vector2D operator * (double a,Vector2D u)
{
	Vector2D w =
    {
        a * u.x,
        a * u.y
    };
	
	return w;
}
inline Vector2D operator * (Vector2D u,double a)
{
	Vector2D w =
    {
        a * u.x,
        a * u.y
    };
	
	return w;
}
inline Vector2D& operator - (Vector2D& u)
{	
	u.x *= - 1;
	u.y *= - 1;
	
	return u;
}
inline Vector2D operator / (Vector2D u,double a)
{
	Vector2D w =
	{
        u.x / a,
        u.y / a
    };
	
	return w;
}
inline Vector2D& operator /= (Vector2D& u,double a)
{	
	u.x /= a;
	u.y /= a;
	
	return u;
}

/* Naiseki */
inline double operator * (Vector2D u,Vector2D v)
{
	return u.x * v.x + u.y * v.y;
}
inline double Dot2D (Vector2D u,Vector2D v)
{
	return u * v;
}
/* Cross */
inline double operator ^ (Vector2D u,Vector2D v)
{
	double  w;
	
    w = u.x * v.y - v.x * u.y;
    
	return w;
}
inline double Cross2D (Vector2D u,Vector2D v)
{
    return u ^ v;
}
/* Norm */
inline double operator ~ (Vector2D u)
{
	return sqrt(u.x * u.x + u.y * u.y);
}
inline double Norm2D (Vector2D u)
{
	return ~u;
}
/* Zero vec */
/*
inline Vector2D Zero2D(void)
{
	Vector2D w =
    {
        0.0,
        0.0
    };
	
	return w;
}
 */
const   static  Vector2D    Zero2D = {0.0,0.0};
/********************************** 1D ****************************************/
/*
inline Vector1D operator + (Vector1D u,Vector1D v)
{
	Vector1D w =
    {
        u.x + v.x
    };
    
	return w;
}
inline Vector1D& operator += (Vector1D& u,Vector1D v)
{
    u.x += v.x;
    
    return u;
}
inline Vector1D operator - (Vector1D u,Vector1D v)
{
	Vector1D w = 
    {
        u.x - v.x
	};
    
	return w;
}
inline Vector1D& operator -= (Vector1D& u,Vector1D v)
{
    u.x -= v.x;
    
    return u;
}
inline Vector1D operator * (double a,Vector1D u)
{
	Vector1D w =
    {
        a * u.x
    };
	
	return w;
}
inline Vector1D operator * (Vector1D u,double a)
{
	Vector1D w =
    {
        a * u.x
    };
	
	return w;
}
inline Vector1D& operator - (Vector1D& u)
{	
	u.x *= - 1;
	
	return u;
}
inline Vector1D operator / (Vector1D u,double a)
{
	Vector1D w =
	{
        u.x / a
    };
	
	return w;
}
inline Vector1D& operator /= (Vector1D& u,double a)
{	
	u.x /= a;
	
	return u;
}

// Naiseki
inline double operator * (Vector1D u,Vector1D v)
{
	return u.x * v.x;
}
 inline double Dot1D (Vector1D u,Vector1D v)
 {
 return u * v;
 }
// Norm
inline double operator ~ (Vector1D u)
{
	return sqrt(u.x * u.x);
}
 inline double Norm1D (Vector1D u)
 {
 return ~u;
 }

// Zero vec 
 inline Vector1D Zero1D(void)
{
	Vector1D w =
    {
        0.0
    };
	
	return w;
}
 */
const   static  Vector1D    Zero1D = {0.0};

#endif

#endif /*_VECTORSTRUCTOPERATOR_H*/

