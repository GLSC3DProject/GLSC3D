/****************************************************************************************
						2011/08/27 Masakazu Akiyama wrote
								libbasic.a ver 7.4
			Copyright (c) Masakazu Akiyama 2006/6/x~ All rights reserved.
														Thanks Ryo & Makoto Matsumoto.
****************************************************************************************/
#ifndef __MASAKAZU_BASIC_H__
#define __MASAKAZU_BASIC_H__

#ifdef __cplusplus
extern          "C"
{
#endif
    double          * Data2FFTCore(int n,double *Data,int *n_ext);
    void            Data2FFTPrint(int n,double *Data,int mode);
    double          * File2FFTCore(char file_name[256],int *n_ext);
    void            File2FFTPrint(char file_name[256],int mode);

    int             CG_Solve(int n,double *A,double *x,double *b,int PrintFlag);
	void            Dtpr(int n, double *u, double *v, double *norm);
	void            Euler(int n, double *u, double dt, double (*func) (double x, double t));

	/* Embedded_Explicit_Runge_Kutta_Multiple */
	void            Dormand_Prince54_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
	double         *Dormand_Prince54_n_dim_Initial_Setting(int n);
	void            Dormand_Prince54_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Bogaki_Shampine32_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Bogaki_Shampine32_n_dim_Initial_Setting(int n);
    void            Bogaki_Shampine32_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Verner98_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Verner98_n_dim_Initial_Setting(int n);
    void            Verner98_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Verner87_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Verner87_n_dim_Initial_Setting(int n);
    void            Verner87_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Verner76_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Verner76_n_dim_Initial_Setting(int n);
    void            Verner76_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Verner65_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Verner65_n_dim_Initial_Setting(int n);
    void            Verner65_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Bogacki_Shampine54_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Bogacki_Shampine54_n_dim_Initial_Setting(int n);
    void            Bogacki_Shampine54_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Wolfram43_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Wolfram43_n_dim_Initial_Setting(int n);
    void            Wolfram43_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Wolfram32_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Wolfram32_n_dim_Initial_Setting(int n);
    void            Wolfram32_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Wolfram21_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Wolfram21_n_dim_Initial_Setting(int n);
    void            Wolfram21_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    void            Multiple_n_dim_core(int p, int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
    double         *Multiple_n_dim_Initial_Setting(int p, int n);
    void            Multiple_n_dim(int p, int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time));
	/* Other function */

	void            ExtendNeumann1Dim(int imax, double *u1, double *u1_ext);
	void            ExtendNeumann2Dim(int imax, int jmax, double *u2, double *u2_ext);
	void            ExtendNeumann3Dim(int imax, int jmax, int kmax, double *u3, double *u3_ext);
    void            ExtendDirichlet1Dim(int imax, double *u1, double *u1_ext);
    void            ExtendDirichlet2Dim(int imax, int jmax, double *u2, double *u2_ext);
    void            ExtendDirichlet3Dim(int imax, int jmax, int kmax, double *u3, double *u3_ext);
	void            ExtendPeriodic1Dim(int imax, double *u1, double *u1_ext);
	void            ExtendPeriodic2Dim(int imax, int jmax, double *u2, double *u2_ext);
	void            ExtendPeriodic3Dim(int imax, int jmax, int kmax, double *u3, double *u3_ext);
    
    double          * FitData(int mode,int n,double *Data,int s0_c1_sc2,int PrintFlags,int GraphicsFlag);
    double          * FitFile(int mode,char file_name[256],int s0_c1_sc2,int PrintFlags,int GraphicsFlag);

	void            GenMatrix(int n, double *c, double *D, double *array, double dx, int mode);
	void            GetParam(int argc, char **argv, int max_va_num,...);
    void            GetParam2(int argc, char **argv, int max_va_num,...);
	void            InitializeReal(int n, double *u);
	void            InitializeInt(int n, int *u);
	void            LuDcmp(int n, double *array);
	void            LuSolve(int n, double *array, double *u, double *r);
	void            Mean(int n, double *u, double *heikin);
	void            MxmnCheck(int n, double *u, double *max, double *min);
	void            QuickSort(double *u, int start, int end);
	void            Rainbow(double h, double s, double v, double *r, double *g, double *b);
	void            Runge(int n, double *u, double dt, double (*func) (double x, double t));
	void            Runge_n_dim(int dim, double *x, double time, double dt, double *work, void (*my_function) (int dim, double *u_dot, double *u, double time));

	double          Sign(double x);
	void            Sum(int n, double *u, double *wa);

	/* mt */
	void            init_genrand(unsigned long s);
	void            init_by_array(unsigned long init_key[], int key_length);
	unsigned long   genrand_int32(void);
	long            genrand_int31(void);
	double          genrand_real1(void);
	double          genrand_real2(void);
	double          genrand_real3(void);
	double          genrand_res53(void);

	/* mt64 */
	void            init_genrand64(unsigned long long seed);
	void            init_by_array64(unsigned long long init_key[], unsigned long long key_length);
	unsigned long long genrand64_int64(void);
	long long       genrand64_int63(void);
	double          genrand64_real1(void);
	double          genrand64_real2(void);
	double          genrand64_real3(void);

#ifdef __cplusplus
}
#endif

#endif				/* __MASAKAZU_BASIC_H__ */
