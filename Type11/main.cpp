#include <glsc3d_3.h>
#include "VectorStructOperator.h"
#include <stdio.h>
#include <stdlib.h>
#include "basic.h"
#include <float.h>
#include "timer.h"
#include <time.h>
#include <sys/time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define	MyDef(size,name)            double	*(name);(name) = (double *) malloc (sizeof(double) * (size));
#define	MyDef2D(size,name)            Vector2D	*(name);(name) = (Vector2D *) malloc (sizeof(Vector2D) * (size));

#define PI					(3.14159265358979)
#define INTV				(10)
#define WINDOW_SIZE_X		(1300)
#define WINDOW_SIZE_Y		(1000)

#define XLEN				(13.5)
#define YLEN				(14.5)
#define ZLEN				(2)
#define SQ(i)				((i) * (i))
#define FurrowNumber		(6)

double	UseSeed = -1;
double	DrawFurrowArea = 1.0;
double  Capture = 2;//0 no: 1 capture every times:2 capture once time
double	tolerance = 1.0e-8;
double	Paraml = 0.5;
double	Paramlx = 0.5;
double	Paramg = 0.0;
double	mu = 3 * PI / 2;
double	kappa = 0.0;
double	muDs = 3 * PI / 2;
double	kappaDs = 0.25;
double	StopMarkerThreshold = 1.0e-7;

double	Furrow_A_haba_x = 2 * 0.1875;
double	Furrow_A_haba_y = 4 * 0.1875;
double	Furrow_A_deepness = 6 * 0.1875;
double	Furrow_A_n = 4;
double	Furrow_A_theta = 0;

double	Furrow_M_haba_x = 8 * 0.1875;
double	Furrow_M_haba_y = 24 * 0.1875;
double	Furrow_M_deepness = 10 * 0.1875;
double	Furrow_M_n = 4;
double	Furrow_M_theta = 0;

double	Furrow_P_haba_x = 8 * 0.1875;
double	Furrow_P_haba_y = 24 * 0.1875;
double	Furrow_P_deepness = 10 * 0.1875;
double	Furrow_P_n = 4;
double	Furrow_P_theta = PI / 6;

double	dImax = 72;
double	dJmax = (int)(2 / sqrt(3.0)  * dImax * YLEN / XLEN);
double	StopMarkerValue;
double	UseVertical = 1.0;

int		Imax;
int		Jmax;
double	dx;
double	dy;
double	*theta,*theta_old,*theta_work;

int		N;
double	*work_u, *u;

typedef struct{
	Vector2D r;
	int neighbor[7];
	double phi;
	double psi;
	double gx;
}GeometryStruct;
GeometryStruct *geometry;

typedef struct{
	Vector2D r;//Center Position
	Vector2D haba;//haba
	double theta;//kakudo
	double deepness;//Fukasa
	int n;//order
}FurrowStruct;
FurrowStruct *Furrow;

#define theta(i,j)		theta[(j) * Imax + (i)]
#define u(i,j)			u[(j) * Imax + (i)]
#define theta_work(i,j)		theta_work[(j) * (Imax) + (i)]
#define theta_old(i,j)		theta_old[(j) * (Imax) + (i)]
#define geometry(i,j)		geometry[(j) * (Imax) + (i)]

double theta_cut_off(double theta)
{
	while (theta < 0) theta += 2 * PI;
	while (2 * PI < theta )theta -= 2 * PI;

//	if(theta < 0 || 2 * PI < theta)printf("Error Occur in theta_cut_off\n");

	return theta;
}

double my_pow(double a,int n)
{
	int i;
	double ans = 1;
	for(i = 0;i < n;i ++) ans *= a;
	return ans;
}
double FurrowFunction(FurrowStruct f,Vector2D r)
{
	double ans;
	Vector2D p = r - f.r;
	double X = p.x * cos(f.theta) + p.y * sin(f.theta);
	double Y = p.x * sin(f.theta) - p.y * cos(f.theta);
	X /= f.haba.x;
	Y /= f.haba.y;
	ans = -f.deepness * exp(-(my_pow(X, f.n) + my_pow(Y, f.n)));
	return ans;
}

Vector2D NabalaFurrowFunction(FurrowStruct f,Vector2D r)
{
	Vector2D ans;
	Vector2D p = r - f.r;
	double X = p.x * cos(f.theta) + p.y * sin(f.theta);
	double Y = p.x * sin(f.theta) - p.y * cos(f.theta);
	X /= f.haba.x;
	Y /= f.haba.y;
	ans.x =
	-f.n * (my_pow(X, f.n - 1) * cos(f.theta) / f.haba.x)
	-f.n * (my_pow(Y, f.n - 1) * sin(f.theta) / f.haba.y);
	ans.y =
	 f.n * (my_pow(Y, f.n - 1) * cos(f.theta) / f.haba.y)
	-f.n * (my_pow(X, f.n - 1) * sin(f.theta) / f.haba.x);
	return ans * FurrowFunction(f,r);
}

double xy2theta(Vector2D r)
{
	return  theta_cut_off(atan2(r.y, r.x));
}

void g_circle_2Ds(Vector2D u,double radius,int edge, int fill)
{
	g_circle_2D(u.x, u.y, radius,edge, fill);
}
void g_move_2Ds(Vector2D u)
{
	g_move_2D(u.x, u.y);
}
void g_plot_2Ds(Vector2D u)
{
	g_plot_2D(u.x, u.y);
}
void g_triangle_2Ds(Vector2D r0,Vector2D r1,Vector2D r2,int edge, int fill)
{
	g_triangle_2D(r0.x, r0.y, r1.x, r1.y, r2.x, r2.y, edge, fill);
}

void g_hexagon(Vector2D r0, double h, int edge, int fill)
{
	double xx[6], yy[6];
	for(int i = 0; i < 6; ++i)
	{
		if((int)UseVertical)
		{
			xx[i] = r0.x + h * cos(PI / 2 + PI / 3 * i);
			yy[i] = r0.y + h * sin(PI / 2 + PI / 3 * i);
		}
		else
		{
			xx[i] = r0.x + h * cos(PI / 3 * i);
			yy[i] = r0.y + h * sin(PI / 3 * i);
		}
	}

	g_polygon_2D(xx, yy, 6, edge, fill);
}

Vector2D subsVec(double x,double y)
{
	Vector2D u;
	u.x = x;
	u.y = y;
	return u;
}

Vector2D theta2vec(double theta)
{
	Vector2D u;

	u.x = cos(theta);
	u.y = sin(theta);

	return u;
}

void
kansu_f(int n, double *q, double *q_dot, double time)
{
	int i,j,k;

	double tmp;
	for(i = 0;i < n;i ++)
	{
		theta_work[i] = q[i];
		q_dot[i] = 0;
	}
	for(i = 0;i < n;i ++)
	{
		tmp = 0;
		j = 0;
		while (geometry[i].neighbor[j] != -1) {
			k = geometry[i].neighbor[j++];
			tmp += (Paraml + Paramlx) * sin(q[k] - q[i]);
		}
		tmp += Paramg * sin(geometry[i].phi - q[i]);
		tmp += geometry[i].gx * sin(geometry[i].psi - q[i]);

		q_dot[i] = tmp;
	}
}

int
JudgeStop(double eps)
{
	int i;
	int ans = 1;
	double wa;
	double tmp,hoge;

	wa = 0;
	for(i = 0;i < N;i ++)
	{
		tmp = theta_cut_off(theta[i]);
		hoge = theta_cut_off(theta_old[i]);
		wa += fabs(tmp - hoge);
	}
	wa /= N;
	StopMarkerValue = wa;
	//printf("StopMarkerValue = %15.15e\n",StopMarkerValue);

	if(wa < eps)
		ans = 0;
	return ans;
}

double rvm_core (double mean, double k)
{
	double result = 0.0;
	double a = 1.0 + sqrt(1 + 4.0 * (k * k));
	double b = (a - sqrt(2.0 * a))/(2.0 * k);
	double r = (1.0 + b * b)/(2.0 * b);

	while (1)
	{
		double U1 = genrand64_real2();
		double z = cos(PI * U1);
		double f = (1.0 + r * z)/(r + z);
		double c = k * (r - f);
		double U2 = genrand64_real2();

		if (c * (2.0 - c) - U2 > 0.0)
		{
			double U3 = genrand64_real2();
			double sign = 0.0;
			if (U3 - 0.5 < 0.0)
				sign = -1.0;
			if (U3 - 0.5 > 0.0)
				sign = 1.0;
			result = sign * acos(f) + mean;
			while (result >= 2.0 * PI)
				result -= 2.0 * PI;
			break;
		}
		else
		{
			if(log(c/U2) + 1.0 - c >= 0.0)
			{
				double U3 = genrand64_real2();
				double sign = 0.0;
				if (U3 - 0.5 < 0.0)
					sign = -1.0;
				if (U3 - 0.5 > 0.0)
					sign = 1.0;
				result = sign * acos(f) + mean;
				while (result >= 2.0 * PI)
					result -= 2.0 * PI;
				break;
			}
		}
	}

	return result;
}
double rvm_core_zero (void)
{
	double result = 0.0;
	double sign = 1.0;

	if (genrand64_real2() < 0.5) sign = -1.0;

	result = sign * PI * genrand64_real2();

	return result;
}

double rvm (double mean, double k)
{
	double result = 0.0;

	if(k < 0)
	{
		printf("Err!\a\n. Function rvm Calling!!\n");
		exit(0);
	}
	else if (k < DBL_EPSILON)
	{
		result = rvm_core_zero();
	}
	else
	{
		result = rvm_core(mean, k);
	}
	return theta_cut_off(result);
}

int main(int argc, char *argv[])
{
	double dummy;
	GetParam2(argc, argv, 30,
			  &(UseSeed),				//p0
			  &(DrawFurrowArea),		//p1
			  &(Capture),				//p2
			  &(tolerance),				//p3
			  &(Paraml),				//p4
			  &(Paramlx),				//p5
			  &(Paramg),				//p6
			  &(mu),					//p7
			  &(kappa),					//p8
			  &(muDs),					//p9
			  &(kappaDs),				//p10
			  &(StopMarkerThreshold),	//p11
			  &(Furrow_A_haba_x),		//p12
			  &(Furrow_A_haba_y),		//p13
			  &(Furrow_A_deepness),		//p14
			  &(Furrow_A_n),			//p15
			  &(Furrow_A_theta),		//p16
			  &(Furrow_M_haba_x),		//p17
			  &(Furrow_M_haba_y),		//p18
			  &(Furrow_M_deepness),		//p19
			  &(Furrow_M_n),			//p20
			  &(Furrow_M_theta),		//p21
			  &(Furrow_P_haba_x),		//p22
			  &(Furrow_P_haba_y),		//p23
			  &(Furrow_P_deepness),		//p24
			  &(Furrow_P_n),			//p25
			  &(Furrow_P_theta),		//p26
			  &(dummy),
			  NULL);
	Imax = (int) dImax;
	Jmax = (int) dJmax;
	N = Imax * Jmax;
	dx = XLEN / Imax;
	dy = YLEN / Jmax;
	int				ERKSolverNumber = 5;
	int				ERKUnknownNumber = N;
	int				StopMarker = 1;
	double          Atol = tolerance;
	double          Rtol = tolerance;
	double			t = 0, dt = 0.01;
	Vector2D		r;
	Vector2D		MacroChaetae[4];
	double theta_color,red,green,blue;
	u = (double *) malloc (sizeof(double) * (ERKUnknownNumber));
	theta = (double *) malloc (sizeof(double) * (ERKUnknownNumber));
	theta_old = (double *) malloc (sizeof(double) * (ERKUnknownNumber));
	theta_work = (double *) malloc (sizeof(double) * (ERKUnknownNumber));
	geometry = (GeometryStruct *) malloc (sizeof(GeometryStruct) * (ERKUnknownNumber));
	work_u = Multiple_n_dim_Initial_Setting(ERKSolverNumber,ERKUnknownNumber);
	Furrow = (FurrowStruct *)malloc(sizeof(FurrowStruct) * FurrowNumber);

	//SetArea(Area, (int) FurrowType);
	struct timeval myTime;
	struct tm *time_st;
	gettimeofday(&myTime, NULL);
	time_st = localtime(&myTime.tv_sec);
	char	FolderName[1024];
	sprintf(FolderName,"%d%02d%02d%02d%02d%02d%02d%06d",     // 現在時刻
			time_st->tm_year+1900,     // 年
			time_st->tm_mon+1,         // 月
			time_st->tm_mday,          // 日
			time_st->tm_wday,			// 曜日
			time_st->tm_hour,          // 時
			time_st->tm_min,           // 分
			time_st->tm_sec,           // 秒
			myTime.tv_usec            // マイクロ秒
			);

	g_enable_highdpi();
	g_set_antialiasing(2);
	g_init_core("GRAPH", WINDOW_SIZE_X, WINDOW_SIZE_Y, 4000, 0, 1, 1, 1,G_FALSE,1<<20,1<<20);
	if ((int)Capture == 1 || (int)Capture == 2)
		g_capture_set(FolderName);
	g_def_scale_2D(0, -XLEN / 2 - dx, XLEN / 2 + dx, -YLEN / 2 - dx, YLEN / 2 + dx, 20, 20, 780 * (XLEN / YLEN), 780);
	g_def_scale_2D(1, -2.2, 2.2, -1.2, 1.2, 780 * (XLEN / YLEN) / 2 - 180, 800, 400, 200);

	////////////////// Initial Condition ///////////////////////////////////////
	unsigned long long seed = time(NULL) + myTime.tv_usec;
	int		i_time;
	if(UseSeed > 0)
		seed = (int)UseSeed;
	init_genrand64(seed);
	int i,j;

	//Set MacroChaetae Position
	i = 0;
	MacroChaetae[i].x = - XLEN / 2 + 3 * XLEN / 13.5;
	MacroChaetae[i].y = - YLEN / 2 + 5 * YLEN / 14.5;
	i = 1;
	MacroChaetae[i].x = - XLEN / 2 + 3 * XLEN / 13.5;
	MacroChaetae[i].y = - YLEN / 2 + 2 * YLEN / 14.5;
	i = 2;
	MacroChaetae[i].x = - MacroChaetae[1].x;
	MacroChaetae[i].y = - YLEN / 2 + 2 * YLEN / 14.5;
	i = 3;
	MacroChaetae[i].x = - MacroChaetae[0].x;
	MacroChaetae[i].y = - YLEN / 2 + 5 * YLEN / 14.5;

	//A
	i = 0;
	Furrow[i].r.x = -XLEN / 2 + 14 * XLEN / 40;
	Furrow[i].r.y = -YLEN / 2 + 18 * YLEN / 20;
	Furrow[i].haba.x = Furrow_A_haba_x;
	Furrow[i].haba.y = Furrow_A_haba_y;
	Furrow[i].deepness = Furrow_A_deepness;
	Furrow[i].n = (int)Furrow_A_n;
	Furrow[i].theta = Furrow_A_theta;

	//M
	i = 1;
	Furrow[i].r.x = -XLEN / 2 +  4 * XLEN / 40;
	Furrow[i].r.y = -YLEN / 2 + 15 * YLEN / 20;
	Furrow[i].haba.x = Furrow_M_haba_x;
	Furrow[i].haba.y = Furrow_M_haba_y;
	Furrow[i].deepness = Furrow_M_deepness;
	Furrow[i].n = (int)Furrow_M_n;
	Furrow[i].theta = Furrow_M_theta;

	//P
	i = 2;
	Furrow[i].r.x = -XLEN / 2 + 4 * XLEN / 40;
	Furrow[i].r.y = -YLEN / 2 + 8 * YLEN / 20;
	Furrow[i].haba.x = Furrow_P_haba_x;
	Furrow[i].haba.y = Furrow_P_haba_y;
	Furrow[i].deepness = Furrow_P_deepness;
	Furrow[i].n = (int)Furrow_P_n;
	Furrow[i].theta = Furrow_P_theta;

	for(i = 3;i < 6;i ++)
	{
		Furrow[i].r.x = -Furrow[i - 3].r.x;
		Furrow[i].r.y =  Furrow[i - 3].r.y;
		Furrow[i].haba.x = Furrow[i - 3].haba.x;
		Furrow[i].haba.y = Furrow[i - 3].haba.y;
		Furrow[i].deepness = Furrow[i - 3].deepness;
		Furrow[i].n = Furrow[i - 3].n;
		Furrow[i].theta = 2 * PI - Furrow[i - 3].theta;
	}

	// Initialize Geometry
	for(j = 0;j < Jmax; ++j)
	{
		for(i = 0; i < Imax; ++i)
		{
			if((int)UseVertical)
			{
				geometry(i,j).r = subsVec(
										  (i + 0.5 * (0.5 + (j & 1 ? 1 : 0)) - Imax / 2.0) * dx,
										  (j - (Jmax - 1) / 2.0) * (sqrt(3) / 2) * dx
										  );
			}
			else
			{
				geometry(i,j).r = subsVec(
										  (i - (Imax - 1) / 2.0) * (sqrt(3) / 2) * dx,
										  (j + 0.5 * (0.5 + (i & 1 ? 1 : 0)) - Jmax / 2.0) * dx
										  );
			}
		}
	}
	for(int n = 0; n < N; ++n)
	{
		for(i = 0; i < 7; ++i)
			geometry[n].neighbor[i] = -1;

		int index = 0;
		for(int m = 0; m < N; ++m)
			if(n == m)
				continue;
			else if(~(geometry[n].r - geometry[m].r) < 2 / sqrt(3) * (dx * 1.01))
				geometry[n].neighbor[index ++] = m;
	}
	//Initialize phi
	for(j = 0;j < Jmax;j ++)
	{
		for(i = 0;i < Imax;i ++)
		{
			theta(i,j) = rvm(mu, kappa);
			Vector2D NabalaFurrow = Zero2D;
			for(int k = 0;k < FurrowNumber; k++)
			{
				Vector2D tmp = NabalaFurrowFunction(Furrow[k], geometry(i, j).r);
				NabalaFurrow += (~tmp) * tmp;
				geometry(i,j).gx = (~NabalaFurrow);
				geometry(i,j).psi = xy2theta(NabalaFurrow);
			}
			geometry(i,j).phi = rvm(muDs, kappaDs);
			u(i,j) = theta(i,j);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	for(i_time = 0;StopMarker;i_time ++)
	{
		for(j = 0;j < Jmax;j ++)
			for(i = 0;i < Imax;i ++)
				theta_old(i,j) = theta(i,j);
		double t_old = t;
		Multiple_n_dim_core(ERKSolverNumber,ERKUnknownNumber, u, &t, &dt,work_u, Atol, Rtol, kansu_f);
		for(j = 0;j < Jmax;j ++)
			for(i = 0;i < Imax;i ++)
				theta(i,j) = u(i,j);

		//JudgeStop
		if(t > t_old) StopMarker = JudgeStop(StopMarkerThreshold);

		if(i_time % INTV == 0)
		{
			g_cls();
			//Text
			g_text_color(0, 0, 0, 1);
			//g_text_font(4);
			g_text_size(24);
			i = 0;
			double text_x = 770;
			g_text_standard(text_x, 50 + 24 * i++, "Time is %f",t);
			g_text_standard(text_x, 50 + 24 * i++, "dt = %15.15e",dt);
			g_text_standard(text_x, 50 + 24 * i++, "ERKSolverNumber = %d",ERKSolverNumber);
			g_text_standard(text_x, 50 + 24 * i++, "CellNumber = %d",N);

			g_text_standard(text_x, 50 + 24 * i++, "seed = %d",seed);
			g_text_standard(text_x, 50 + 24 * i++, "DrawFurrowArea = %f",DrawFurrowArea);
			g_text_standard(text_x, 50 + 24 * i++, "Capture = %f",Capture);
			g_text_standard(text_x, 50 + 24 * i++, "tolerance = %g",tolerance);
			g_text_standard(text_x, 50 + 24 * i++, "Paraml = %f",Paraml);
			g_text_standard(text_x, 50 + 24 * i++, "Paramlx = %f",Paramlx);
			g_text_standard(text_x, 50 + 24 * i++, "Paramg = %f",Paramg);
			g_text_standard(text_x, 50 + 24 * i++, "mu* = %f",360 * mu / (2 * PI));
			g_text_standard(text_x, 50 + 24 * i++, "kappa = %f",kappa);
			g_text_standard(text_x, 50 + 24 * i++, "muDs* = %f",360 * muDs / (2 * PI));
			g_text_standard(text_x, 50 + 24 * i++, "kappaDs = %f",kappaDs);
			g_text_standard(text_x, 50 + 24 * i++, "StopMarkerValue = %g",StopMarkerValue);
			g_text_standard(text_x, 50 + 24 * i++, "StopMarkerThreshold = %g",StopMarkerThreshold);

			g_text_standard(text_x, 50 + 24 * i++, "Furrow_A_haba_x = %f",Furrow_A_haba_x);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_A_haba_y = %f",Furrow_A_haba_y);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_A_deepness = %f",Furrow_A_deepness);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_A_n = %f",Furrow_A_n);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_A_theta = %f",Furrow_A_theta);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_M_haba_x = %f",Furrow_M_haba_x);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_M_haba_y = %f",Furrow_M_haba_y);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_M_deepness = %f",Furrow_M_deepness);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_M_n = %f",Furrow_M_n);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_M_theta = %f",Furrow_M_theta);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_P_haba_x = %f",Furrow_P_haba_x);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_P_haba_y = %f",Furrow_P_haba_y);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_P_deepness = %f",Furrow_P_deepness);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_P_n = %f",Furrow_P_n);
			g_text_standard(text_x, 50 + 24 * i++, "Furrow_P_theta = %f",Furrow_P_theta);

			g_text_standard(text_x, 50 + 24 * i++, "dummy = %f",dummy);

			//Cell
			g_sel_scale_2D(0);
			g_line_width(10);
			g_line_color(0, 0, 0, 1);
			g_line_type(0);
			g_line_width(1);
			g_box_2D(0, 0, XLEN, YLEN, 1, 0);

			for(j = 0;j < Jmax;j ++)
			{
				r.y = (j + 0.5) * dy - YLEN / 2;
				for(i = 0;i < Imax;i ++)
				{
					r.x = (i + 0.5) * dx - XLEN / 2;

					//DrawMacroChaetae
					g_area_color_2D(0.5, 0.5, 0.5, 1);
					for(int k = 0;k < 4;k ++)
						g_circle_2D(MacroChaetae[k].x, MacroChaetae[k].y, dx, 0,1);
					//Cell
					theta_color = theta_cut_off(theta(i, j));
					double theta_calc = 1.5 * (theta_color / (2 * PI) + 0.75);
					if(geometry(i, j).r.x < 0)
						theta_calc = 1.5 * (1 - theta_color / (2 * PI) + 0.25);
					Rainbow(theta_calc, 1.0, 1.0, &red, &green, &blue);
					g_area_color_2D(red,green,blue,1);
					g_line_color(red,green,blue,1);
					g_hexagon(geometry(i, j).r, dx / sqrt(3), 0,1);
				}
			}

			if(DrawFurrowArea > 0)
			{
				//Furrow Area
				g_line_color(1, 1, 1, 1);
				g_line_width(3);
				for(int k = 0;k < FurrowNumber; k++)
				{
					g_arrow_2D(Furrow[k].r.x,Furrow[k].r.y,
							   cos(Furrow[k].theta + PI / 2),
							   sin(Furrow[k].theta + PI / 2),
							   Furrow[k].haba.y, 0.5 * dx, 2);
					g_arrow_2D(Furrow[k].r.x,Furrow[k].r.y,
							   -cos(Furrow[k].theta + PI / 2),
							   -sin(Furrow[k].theta + PI / 2),
							   Furrow[k].haba.y, 0.5 * dx, 2);

					g_arrow_2D(Furrow[k].r.x,Furrow[k].r.y,
							   cos(Furrow[k].theta + PI),
							   sin(Furrow[k].theta + PI),
							   Furrow[k].haba.x, 0.5 * dx, 2);
					g_arrow_2D(Furrow[k].r.x,Furrow[k].r.y,
							   -cos(Furrow[k].theta + PI),
							   -sin(Furrow[k].theta + PI),
							   Furrow[k].haba.x, 0.5 * dx, 2);
				}
			}

			//Sikisoukan
			g_sel_scale_2D(1);
			g_line_width(10);
			g_line_color(0, 0, 0, 1);
			g_line_type(0);

			int	NumRose = 720;
			double RoseTheta;
			double dTheta = 2 * PI / NumRose;
			for(i = 0;i < NumRose; i++)
			{
				RoseTheta = dTheta * i;
				theta_color = RoseTheta;

				Rainbow(1.5 * (theta_color / (2 * PI) + 0.75), 1.0, 1.0, &red, &green, &blue);
				g_area_color_2D(red,green,blue,1);
				g_triangle_2D(0.8, 0.0,
							  0.75 * cos(RoseTheta - dTheta / 2) + 0.8, 0.75 * sin(RoseTheta - dTheta / 2),
							  0.75 * cos(RoseTheta + dTheta / 2) + 0.8, 0.75 * sin(RoseTheta + dTheta / 2),
							  0,1);
				Rainbow(1.5 * (1 - theta_color / (2 * PI) + 0.25), 1.0, 1.0, &red, &green, &blue);
				g_area_color_2D(red,green,blue,1);
				g_triangle_2D(-0.8, 0.0,
							  0.75 * cos(RoseTheta - dTheta / 2) - 0.8, 0.75 * sin(RoseTheta - dTheta / 2),
							  0.75 * cos(RoseTheta + dTheta / 2) - 0.8, 0.75 * sin(RoseTheta + dTheta / 2),
							  0,1);
			}

			g_finish();

			if ((int)Capture == 0)
			{
				g_sleep(0.05);
			}
			else if ((int)Capture == 1)
			{
				g_capture();
			}
		}
	}

	if ((int)Capture == 2)
	{
			g_capture();
	}
	return 0;
}

