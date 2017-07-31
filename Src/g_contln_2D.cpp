#include "glsc3d_3_private.h"
#include <stdio.h>
#include <time.h>
#define data2D(i, j)     data2D[(i)*N_y+(j)]
#define			MyPrintf2D(name)	printf("%s.x = %f %s.y = %f \n",#name,name.x,#name,name.y);

/********************************** 2D ****************************************/
typedef struct
{
	double          x, y;
}               Vector2D;

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
//const   static  Vector2D    Zero2D = {0.0,0.0};
/********************************** 2D ****************************************/
#define SQ(i)		((i) * (i))
#include <float.h>
void g_move_2Ds(Vector2D u){g_move_2D(u.x,u.y);}
void g_plot_2Ds(Vector2D u){g_plot_2D(u.x,u.y);}
void g_arrow_2Ds(Vector2D u, Vector2D d, double arrow_size, double head_size, int type){
	g_arrow_2D(u.x, u.y, d.x, d.y, arrow_size, head_size, type);
}
typedef struct
{
	int flag;
	Vector2D r,n;
//	Vector2D r;
} Edge_2D_struct;

Edge_2D_struct do_contln_2D_Edge(Vector2D ra,Vector2D rb,double fa,double fb) {
	Edge_2D_struct ans;
	if(fa * fb > 0) ans.flag = 0;
	else
	{
		ans.flag = 1;
		double s = fb / (fb - fa);
		Vector2D r = rb + s * (ra - rb);
		ans.r = r;
	}
	return  ans;
//	double eps = DBL_EPSILON;
//	Edge_2D_struct ans;
//	if(fa * fb > 0) ans.flag = 0;
//	else
//	{
//		ans.flag = 1;
//		Vector2D nabla_f;
//		double s = fb / (fb - fa);
//		Vector2D r = rb + s * (ra - rb);
//		ans.r = r;
//		Vector2D a = ra - r;
//		Vector2D b = rb - r;
//		Vector2D pm = {1, 1};
//		if(a.x < b.x) pm.x = -1;
//		if(a.y < b.y) pm.y = -1;
//
//		Vector2D ex = {pm.x, 0};
//		Vector2D ey = {0, pm.y};
//		Vector2D ax = (a * ex) * ex;
//		Vector2D ay = (a * ey) * ey;
//		Vector2D bx = (b * ex) * ex;
//		Vector2D by = (b * ey) * ey;
//
//		double U = ~ax + ~bx;//U := 1 / u
//		double V = ~ay + ~by;//V := 1 / u
//
//		if(U < eps) nabla_f = {0, (fa - fb) / V};
//		else if(V < eps) nabla_f = {(fa - fb) / U, 0};
//		else
//		{
//			double fax = (a * ax) / (a * a) * fa;
//			double fay = (a * ay) / (a * a) * fa;
//			double fbx = (b * bx) / (b * b) * fb;
//			double fby = (b * by) / (b * b) * fb;
//			nabla_f = {(fax - fbx) / U, (fay - fby) / V};
//		}
//		nabla_f = {pm.x * nabla_f.x, pm.y * nabla_f.y};
//		ans.n = nabla_f;
//	}
	return ans;
}
void do_contln_2D(Vector2D r0,Vector2D r1,Vector2D r2,
				  double f0,double f1,double f2
				  )
{
	Edge_2D_struct Edge_2D[3];
	Edge_2D[0] = do_contln_2D_Edge(r0,r1,f0,f1);
	Edge_2D[1] = do_contln_2D_Edge(r1,r2,f1,f2);
	Edge_2D[2] = do_contln_2D_Edge(r2,r0,f2,f0);

	if (Edge_2D[0].flag == 1)
	{
		g_move_2Ds(Edge_2D[0].r);
		if (Edge_2D[1].flag == 1) g_plot_2Ds(Edge_2D[1].r);
		if (Edge_2D[2].flag == 1) g_plot_2Ds(Edge_2D[2].r);

	}
	if (Edge_2D[1].flag == 1)
	{
		g_move_2Ds(Edge_2D[1].r);
		if (Edge_2D[2].flag == 1) g_plot_2Ds(Edge_2D[2].r);

	}
//	if (Edge_2D[0].flag == 1)
//	{
//		p0 = Edge_2D[0].r;
//		g_move_2Ds(p0);
//		if (Edge_2D[1].flag == 1)
//		{
//			p1 = Edge_2D[1].r;
//			g_plot_2Ds(p1);
//		}
//		else if(Edge_2D[2].flag == 1)
//		{
//			p1 = Edge_2D[2].r;
//			g_plot_2Ds(p1);
//		}
//	}
//	else if (Edge_2D[1].flag == 1)
//	{
//		p0 = Edge_2D[1].r;
//		g_move_2Ds(p0);
//		p1 = Edge_2D[2].r;
//		g_plot_2Ds(p1);
//	}

//	Vector2D n;
//	if (Edge_2D[0].flag == 1)
//	{
//		p0 = Edge_2D[0].r;
//		n = Edge_2D[0].n;
//		g_arrow_2Ds(p0, n, 0.3, 0.02, 2);
//	}
//	if (Edge_2D[1].flag == 1)
//	{
//		p0 = Edge_2D[1].r;
//		n = Edge_2D[1].n;
//		g_arrow_2Ds(p0, n, 0.3, 0.02, 2);
//	}
//	if (Edge_2D[2].flag == 1)
//	{
//		p0 = Edge_2D[2].r;
//		n = Edge_2D[2].n;
//		g_arrow_2Ds(p0, n, 0.3, 0.02, 2);
//	}
}

void g_contln_f_2D(
				   double x_left, double x_right,
				   double y_bottom, double y_top,
				   int N_x, int N_y,
				   double *data2D,
				   double level)
{
	double xl = x_right-x_left,yl = y_top-y_bottom;
	Vector2D d = {xl/N_x, yl/N_y};
	for(int i = 0;i < N_x - 1;i ++)
	{
		for(int j = 0;j < N_y - 1;j ++)
		{
			Vector2D r[5];
			r[0] = {x_left + (i + 0.5) * d.x,y_bottom + (j + 0.5) * d.y};
			r[1] = {x_left + (i + 1.5) * d.x,y_bottom + (j + 0.5) * d.y};
			r[2] = {x_left + (i + 1.5) * d.x,y_bottom + (j + 1.5) * d.y};
			r[3] = {x_left + (i + 0.5) * d.x,y_bottom + (j + 1.5) * d.y};
			r[4] = (r[0] + r[1] + r[2] + r[3]) / 4;
			double f[5];
			f[0] = data2D(i + 0, j + 0) - level;
			f[1] = data2D(i + 1, j + 0) - level;
			f[2] = data2D(i + 1, j + 1) - level;
			f[3] = data2D(i + 0, j + 1) - level;
			f[4] = (f[0] + f[1] + f[2] + f[3]) / 4;
			do_contln_2D(r[0],r[1],r[4],
						 f[0],f[1],f[4]);
			do_contln_2D(r[1],r[2],r[4],
						 f[1],f[2],f[4]);
			do_contln_2D(r[2],r[3],r[4],
						 f[2],f[3],f[4]);
			do_contln_2D(r[3],r[0],r[4],
						 f[3],f[0],f[4]);
		}
	}
}
