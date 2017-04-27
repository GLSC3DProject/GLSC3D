/*******************************************************************************************************************************
 2011/09/19 Masakazu Akiyama wrote

 Wolfram2(1)            Method Number   2
 Wolfram3(2)            Method Number   3
 Bogacki_Shampine 3(2)  Method Number  10
 Wolfram4(3)            Method Number   4
 Bogacki_Shampine 5(4)  Method Number   5
 Dormand_Prince 5(4)    Method Number  11
 Verner6(5)             Method Number   6
 Verner7(6)             Method Number   7
 Verner8(7)             Method Number   8
 Verner9(8)             Method Number   9

 x'(t)_j = f(x_j,t);

 j = 0,...,n - 1

 work(0,j)      = f(x[j]                                            , t + dh * cvec[0])                   <------------- k1
 work(1,j)      = f(x[j] + A[1][0] * work(0,j) * dh                 , t + dh * cvec[1])                   <------------- k2
 .
 .
 .
 work(S-1,j)    = f(x[j] + sum_{l=0}^{S-2} A[S-1][l]*work(l,j) * dh , t + dh * cvec[S-1])                 <------------- kS
 work(S  ,j)    = x[j] + sum_{l=0}^{S-2} A[S-1][l]*work(l,j) * dh                                         <------------- New x
 work(S+1,j)    = sum_{l=0}^{S-1} evec[l]*work(l,j) * dh                                                  <------------- err

 work(S  ,j) is embedded. Therefore, we should do work(0,j) = work(S  ,j) at next time step.
 But, I don't use this method. Because calculation time is bigger.

 Copyright (c) Masakazu Akiyama 2011/9/19~ All rights reserved.
 *******************************************************************************************************************************/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define         FACMAX              (6.0)	/* 1.5 ~ 6.0 */
#define         FACMIN              (0.33)	/* < facmax  */
#define         FACSAVE             (0.9)	/* 0.8 ~ 0.9 */

#define			MAX(a, b)		(((a) > (b)) ? (a) : (b))
#define			MIN(a, b)		(((a) < (b)) ? (a) : (b))
#define         work(i,j)       work[(i) * (n) + (j)]

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P11               (5)
#define         S11               (7)

static const double A11[S11][S11] = {
	{0, 0, 0, 0, 0, 0, 0},
	{1.0 / 5, 0, 0, 0, 0, 0, 0},
	{3.0 / 40, 9.0 / 40, 0, 0, 0, 0, 0},
	{44.0 / 45, -56.0 / 15, 32.0 / 9, 0, 0, 0, 0},
	{19372.0 / 6561, -25360.0 / 2187, 64448.0 / 6561, -212.0 / 729, 0, 0, 0},
	{9017.0 / 3168, -355.0 / 33, 46732.0 / 5247, 49.0 / 176, -5103.0 / 18656, 0, 0},
	{35.0 / 384, 0, 500.0 / 1113, 125.0 / 192, -2187.0 / 6784, 11.0 / 84, 0},
};
static const double cvec11[S11] = {0, 1.0 / 5, 3.0 / 10, 4.0 / 5, 8.0 / 9, 1, 1};
static const double evec11[S11] = {71.0 / 57600, 0, -71.0 / 16695, 71.0 / 1920, -17253.0 / 339200, 22.0 / 525, -1.0 / 40};
void
Dormand_Prince54_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P11, s = S11;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec11[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A11[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec11[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A11[i][0] * work(0, j) + A11[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec11[i]);

	i = 3;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A11[i][0] * work(0, j) + A11[i][1] * work(1, j) + A11[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec11[i]);

	i = 4;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A11[i][0] * work(0, j) + A11[i][1] * work(1, j) + A11[i][2] * work(2, j) + A11[i][3] * work(3, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec11[i]);

	i = 5;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A11[i][0] * work(0, j) + A11[i][1] * work(1, j) + A11[i][2] * work(2, j) + A11[i][3] * work(3, j) + A11[i][4] * work(4, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec11[i]);

	i = 6;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A11[i][0] * work(0, j) + A11[i][1] * work(1, j) + A11[i][2] * work(2, j) + A11[i][3] * work(3, j) + A11[i][4] * work(4, j) + A11[i][5] * work(5, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec11[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec11[0] * work(0, j) + evec11[1] * work(1, j) + evec11[2] * work(2, j) + evec11[3] * work(3, j) + evec11[4] * work(4, j) + evec11[5] * work(5, j) + evec11[6] * work(6, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Dormand_Prince54_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S11 + 2) * n * sizeof(double));
	return work;
}
void
Dormand_Prince54_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Dormand_Prince54_n_dim_Initial_Setting(n);
	Dormand_Prince54_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P10               (3)
#define         S10               (4)

static const double A10[S10][S10] = {{0, 0, 0, 0}, {1.0 / 2, 0, 0, 0}, {0, 3.0 / 4, 0, 0}, {2.0 / 9, 1.0 / 3, 4.0 / 9, 0}};
static const double cvec10[S10] = {0, 1.0 / 2, 3.0 / 4, 1.0};
static const double evec10[S10] = {-5.0 / 72, 1.0 / 12, 1.0 / 9, -1.0 / 8};
void
Bogaki_Shampine32_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P10, s = S10;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec10[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A10[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec10[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A10[i][0] * work(0, j) + A10[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec10[i]);

	i = 3;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A10[i][0] * work(0, j) + A10[i][1] * work(1, j) + A10[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec10[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec10[0] * work(0, j) + evec10[1] * work(1, j) + evec10[2] * work(2, j) + evec10[3] * work(3, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Bogaki_Shampine32_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S10 + 2) * n * sizeof(double));
	return work;
}
void
Bogaki_Shampine32_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Bogaki_Shampine32_n_dim_Initial_Setting(n);
	Bogaki_Shampine32_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P9               (9)
#define         S9               (16)

static const double A9[S9][S9] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.04000000000000000000000000000000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{-0.01988527319182290976502415114660891, 0.1163726333296965222173745449432725, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.03618276005170260466963139767374884, 0, 0.1085482801551078140088941930212465, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{2.272114264290177409193144938921415, 0, -8.526886447976398578316416192982602, 6.830772183686221169123271254061187, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.05094385535389374394512668566783434, 0, 0, 0.1755865049809071110203693328749562, 0.0007022961270757467498778006760324450, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.1424783668683284782770955365543879, 0, 0, -0.3541799434668684104094753917518524, 0.07595315450295100889001534202778550, 0.6765157656337123215269906939508561, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.07111111111111111111111111111111111, 0, 0, 0, 0, 0.3279909287605898328568406057725492, 0.2408979601282990560320482831163397, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.07125000000000000000000000000000000, 0, 0, 0, 0, 0.3268842451575245554847578757216916, 0.1156157548424754445152421242783084, -0.03375000000000000000000000000000000, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.04822677322465810178387112087673611, 0, 0, 0, 0, 0.03948559980495400110769549704186108, 0.1058851161934658144373823566907778, -0.02152006320474309346664428710937500, -0.1045374260183348238623046875000000, 0, 0, 0, 0, 0, 0, 0},
	{-0.02609113435754923412210928689962011, 0, 0, 0, 0, 0.03333333333333333333333333333333333, -0.1652504006638105086724681598195267, 0.03434664118368616658319419895678839, 0.1595758283215209043195814910843068, 0.2140857321828193385584684233447183, 0, 0, 0, 0, 0, 0},
	{-0.03628423396255658590765099790912671, 0, 0, 0, 0, -1.096167597427208807028761474420298, 0.1826035504321331052308236240517254, 0.07082254444170683256130286854556251, -0.02313647018482431269999297384826304, 0.2711204726320932916455631550463655, 1.308133749422980744437146904349994, 0, 0, 0, 0, 0},
	{-0.5074635056416974879347823927726392, 0, 0, 0, 0, -6.631342198657237090355284142048734, -0.2527480100908801052700209730148603, -0.4952612380036095562991116175550168, 0.2932525545253886902857397203600036, 1.440108693768280908474851998204424, 6.237934498647055877243623886838802, 0.7270192054526987638549835199880203, 0, 0, 0, 0},
	{0.6130118256955931701496387847232542, 0, 0, 0, 0, 9.088803891640463313341034206647776, -0.4073788156293448681033153811383252, 1.790733389490374687043894756399015, 0.7149271667617550737248752506296027, -1.438580857841722850237810322456327, -8.263329312064740580595954649844133, -1.537570570808865115231450725068827, 0.3453832827564871699090880801079644, 0, 0, 0},
	{-1.211697910343873872490625222495537, 0, 0, 0, 0, -19.05581871559595277753334676575234, 1.263060675389875101359431018519053, -6.913916969178458046793476128409111, -0.6764622665094980653001156413836212, 3.367860445026607887090352785684064, 18.00675164312590810020103216906572, 6.838828926794279896350389904990814, -1.031516451921950498420447675652291, 0.4129106232130622755368055554332539, 0, 0},
	{2.157389007494053627033175177985667, 0, 0, 0, 0, 23.80712219809580523172312179815280, 0.8862779249216555490303680141526631, 13.13913039759876381480201677314223, -2.604415709287714883747369630937415, -5.193859949783872300189266203049579, -20.41234071154150778768154893536134, -12.30085625250572261314889445241581, 1.521553095008539362178397458330792, 0, 0, 0}
};
static const double cvec9[S9] = {
	0, 0.04000000000000000000000000000000000,
	0.09648736013787361245235039379666357,
	0.1447310402068104186785255906949954,
	0.5760000000000000000000000000000000,
	0.2272326564618766017153738192188230,
	0.5407673435381233982846261807811770,
	0.6400000000000000000000000000000000,
	0.4800000000000000000000000000000000,
	0.06754000000000000000000000000000000,
	0.2500000000000000000000000000000000,
	0.6770920153543242682384311058159604,
	0.8115000000000000000000000000000000,
	0.9060000000000000000000000000000000,
	1.000000000000000000000000000000000,
	1.000000000000000000000000000000000
};
static const double evec9[S9] = {
	-0.005757813768188948806063455165771541, 0, 0, 0, 0, 0, 0,
	-1.067593453094810776890755316730143,
	0.1409963613439397918928943109654191,
	0.01441171539691492553699712175355675,
	-0.03079696125188303336339142424284142,
	1.161315257817906713129169874591697,
	-0.3222111348611858500471171795791358,
	0.1294845879197561516869001434704642,
	0.02947744761261941714007911131590717,
	-0.04932600711506839027871318637915325
};
void
Verner98_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P9, s = S9;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 3;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 4;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 5;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 6;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 7;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 8;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 9;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j) + A9[i][8] * work(8, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 10;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j) + A9[i][8] * work(8, j) + A9[i][9] * work(9, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 11;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j) + A9[i][8] * work(8, j) + A9[i][9] * work(9, j) + A9[i][10] * work(10, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 12;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j) + A9[i][8] * work(8, j) + A9[i][9] * work(9, j) + A9[i][10] * work(10, j) + A9[i][11] * work(11, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 13;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j) + A9[i][8] * work(8, j) + A9[i][9] * work(9, j) + A9[i][10] * work(10, j) + A9[i][11] * work(11, j) + A9[i][12] * work(12, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 14;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j) + A9[i][8] * work(8, j) + A9[i][9] * work(9, j) + A9[i][10] * work(10, j) + A9[i][11] * work(11, j) + A9[i][12] * work(12, j) + A9[i][13] * work(13, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	i = 15;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A9[i][0] * work(0, j) + A9[i][1] * work(1, j) + A9[i][2] * work(2, j) + A9[i][3] * work(3, j) + A9[i][4] * work(4, j) + A9[i][5] * work(5, j) + A9[i][6] * work(6, j) + A9[i][7] * work(7, j) + A9[i][8] * work(8, j) + A9[i][9] * work(9, j) + A9[i][10] * work(10, j) + A9[i][11] * work(11, j) + A9[i][12] * work(12, j) + A9[i][13] * work(13, j) + A9[i][14] * work(14, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec9[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec9[0] * work(0, j) + evec9[1] * work(1, j) + evec9[2] * work(2, j) + evec9[3] * work(3, j) + evec9[4] * work(4, j) + evec9[5] * work(5, j) + evec9[6] * work(6, j) + evec9[7] * work(7, j) + evec9[8] * work(8, j) + evec9[9] * work(9, j) + evec9[10] * work(10, j) + evec9[11] * work(11, j) + evec9[12] * work(12, j) + evec9[13] * work(13, j) + evec9[14] * work(14, j) + evec9[15] * work(15, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Verner98_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S9 + 2) * n * sizeof(double));
	return work;
}
void
Verner98_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Verner98_n_dim_Initial_Setting(n);
	Verner98_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P8               (8)
#define         S8               (13)
static const double A8[S8][S8] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.2500000000000000000000000000000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.08740084650491523205268632759487741, 0.02548760493865432175308795062034569, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.04233316929133858267716535433070866, 0, 0.1269995078740157480314960629921260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.4260950588874226149488144523757227, 0, -1.598795284659152326542773323065718, 1.596700225771729711593958870689995, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.05071933729671392951509061813851364, 0, 0, 0.2543337726460040758275471440887778, 0.2039468900572819946573622377727086, 0, 0, 0, 0, 0, 0, 0, 0},
	{-0.2900037471752311097038837928542590, 0, 0, 1.344187391026078988943868110941434, -2.864777943361442730961110382703656, 2.677594299510594851721126064616482, 0, 0, 0, 0, 0, 0, 0},
	{0.09853501133799354646974040298072701, 0, 0, 0, 0.2219268063075138484202403649819739, -0.1814062291180699431269033828807395, 0.01094441147256254823692261491803863, 0, 0, 0, 0, 0, 0},
	{0.3871105254573114467944461816516637, 0, 0, -1.442445497485527757125674555307793, 2.905398189069950931769134644923385, -1.853771069630105929084333267581198, 0.1400364809872815426949732510977124, 0.5727394081149581657574677462444771, 0, 0, 0, 0, 0},
	{-0.1612440344443930810063001619791348, 0, 0, -0.1733960295735898408357840447396257, -1.301289281406514740601681274517249, 1.137950375173861730855879213143100, -0.03174764966396688010692352113804302, 0.9335129382493366643981106448605688, -0.08378631833473385270330085562961643, 0, 0, 0, 0},
	{-0.01919944488158953328151080465148358, 0, 0, 0.2733085726526428490794232625401612, -0.6753497320694437291969161121094238, 0.3415184981384601607173848997472838, -0.06795006480337577247892051619852463, 0.09659175224762387888426558649121638, 0.1325308251118210118072103846654539, 0.3685495936038611344690632995153167, 0, 0, 0},
	{0.6091877403645289867688841211158882, 0, 0, -2.272569085898001676899980093141309, 4.757898342694029006815525588191479, -5.516106706692758482429468966784425, 0.2900596369680119270909581856594617, 0.5691423963359036822910985845480185, 0.7926795760332167027133991620589333, 0.1547372045328882289412619077184990, 1.614970895662181624708321510633454, 0, 0},
	{0.8873576220853471966321169405198102, 0, 0, -2.975459782108536755851363280470930, 5.600717009488163059799039254835010, -5.915607450536674468001493018994166, 0.2202968915613492701687914254080764, 0.1015509782446221666614327134090300, 1.151434564738605590978039775212585, 1.929710166527123939613436190080584, 0, 0, 0}
};
static const double cvec8[S8] = {
	0, 0.2500000000000000000000000000000000,
	0.1128884514435695538057742782152231,
	0.1693326771653543307086614173228346,
	0.4240000000000000000000000000000000,
	0.5090000000000000000000000000000000,
	0.8670000000000000000000000000000000,
	0.1500000000000000000000000000000000,
	0.7090680365138684008060140010282475,
	0.3200000000000000000000000000000000,
	0.4500000000000000000000000000000000,
	1.000000000000000000000000000000000,
	1.000000000000000000000000000000000
};
static const double evec8[S8] = {
	-0.001117546733800211675648889692960900, 0, 0, 0, 0,
	-0.1054085787644418762407465857411916,
	-0.007083989297009741637832867621621511,
	0.008072082741843727115010119774442734,
	0.02056426027136527890202010434792404,
	-0.03904976140869743731260679559009543,
	0.1227729023501861961082434631592144,
	0.04181195863899163158338484280087188,
	-0.04056132779843756684182339143658361
};

void
Verner87_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P8, s = S8;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 3;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 4;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 5;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 6;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j) + A8[i][5] * work(5, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 7;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j) + A8[i][5] * work(5, j) + A8[i][6] * work(6, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 8;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j) + A8[i][5] * work(5, j) + A8[i][6] * work(6, j) + A8[i][7] * work(7, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 9;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j) + A8[i][5] * work(5, j) + A8[i][6] * work(6, j) + A8[i][7] * work(7, j) + A8[i][8] * work(8, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 10;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j) + A8[i][5] * work(5, j) + A8[i][6] * work(6, j) + A8[i][7] * work(7, j) + A8[i][8] * work(8, j) + A8[i][9] * work(9, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 11;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j) + A8[i][5] * work(5, j) + A8[i][6] * work(6, j) + A8[i][7] * work(7, j) + A8[i][8] * work(8, j) + A8[i][9] * work(9, j) + A8[i][10] * work(10, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	i = 12;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A8[i][0] * work(0, j) + A8[i][1] * work(1, j) + A8[i][2] * work(2, j) + A8[i][3] * work(3, j) + A8[i][4] * work(4, j) + A8[i][5] * work(5, j) + A8[i][6] * work(6, j) + A8[i][7] * work(7, j) + A8[i][8] * work(8, j) + A8[i][9] * work(9, j) + A8[i][10] * work(10, j) + A8[i][11] * work(11, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec8[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec8[0] * work(0, j) + evec8[1] * work(1, j) + evec8[2] * work(2, j) + evec8[3] * work(3, j) + evec8[4] * work(4, j) + evec8[5] * work(5, j) + evec8[6] * work(6, j) + evec8[7] * work(7, j) + evec8[8] * work(8, j) + evec8[9] * work(9, j) + evec8[10] * work(10, j) + evec8[11] * work(11, j) + evec8[12] * work(12, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Verner87_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S8 + 2) * n * sizeof(double));
	return work;
}
void
Verner87_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Verner87_n_dim_Initial_Setting(n);
	Verner87_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P7               (7)
#define         S7               (10)
static const double A7[S7][S7] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.005000000000000000000000000000000000, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{-1.076790123456790123456790123456790, 1.185679012345679012345679012345679, 0, 0, 0, 0, 0, 0, 0, 0},
	{0.04083333333333333333333333333333333, 0, 0.1225000000000000000000000000000000, 0, 0, 0, 0, 0, 0, 0},
	{0.6360714285714285714285714285714286, 0, -2.444464285714285714285714285714286, 2.263392857142857142857142857142857, 0, 0, 0, 0, 0, 0},
	{-2.535121107934924522925638355466022, 0, 10.29937465444926792043851446075602, -7.951303288599057994949321745826688, 0.7930114892310059220122601427111526, 0, 0, 0, 0, 0},
	{1.001876581252463296196919658309500, 0, -4.166571282442379833131393800547097, 3.834343292912864241255266521825138, -0.5023333356071084754746433022861177, 0.6676847438841607711538509226985770, 0, 0, 0, 0},
	{27.25501835463076713033396381917501, 0, -42.00461727841063835531864544390930, -10.53571312661948991792108160054653, 80.49553671141193714798365215892683, -67.34388227179051346854907596321298, 13.04865761077793746347118702956696, 0, 0, 0},
	{-3.039737805711496514694365865875576, 0, 10.13816141032980111185794619070970, -6.429305674864721572146282562955530, -1.586437148340827658711531285379861, 1.892178184196842441086430890913135, 0.01969933540760886906129236016333644, 0.005441698982793323546510272424795257, 0, 0},
	{-1.444951891677773513735100317935571, 0, 8.031891385995591922411703322301956, -7.583174166340134682079888302367159, 3.581616935319007421124768544245288, -2.436972263219952941118380906569375, 0.8515899999232617933968976603248614, 0, 0, 0}
};
static const double cvec7[S7] = {
	0, 0.005000000000000000000000000000000000,
	0.1088888888888888888888888888888889,
	0.1633333333333333333333333333333333,
	0.4550000000000000000000000000000000,
	0.6059617471462913245758145021744683,
	0.8350000000000000000000000000000000,
	0.9150000000000000000000000000000000,
	1.000000000000000000000000000000000,
	1.000000000000000000000000000000000
};
static const double evec7[S7] = {
	-0.00005940986559287495396210108815342685, 0, 0,
	0.0002294907067992936280680921236922958,
	-0.001071012479934821030570738411921828,
	0.001810037246667899323503160409922553,
	-0.003172427816837888851375943925499498,
	0.003074483740820063133530438847909918,
	0.04802380998949694330818906334714312,
	-0.04883497152141861455738197130309314
};

void
Verner76_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P7, s = S7;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 3;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j) + A7[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 4;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j) + A7[i][2] * work(2, j) + A7[i][3] * work(3, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 5;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j) + A7[i][2] * work(2, j) + A7[i][3] * work(3, j) + A7[i][4] * work(4, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 6;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j) + A7[i][2] * work(2, j) + A7[i][3] * work(3, j) + A7[i][4] * work(4, j) + A7[i][5] * work(5, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 7;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j) + A7[i][2] * work(2, j) + A7[i][3] * work(3, j) + A7[i][4] * work(4, j) + A7[i][5] * work(5, j) + A7[i][6] * work(6, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 8;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j) + A7[i][2] * work(2, j) + A7[i][3] * work(3, j) + A7[i][4] * work(4, j) + A7[i][5] * work(5, j) + A7[i][6] * work(6, j) + A7[i][7] * work(7, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	i = 9;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A7[i][0] * work(0, j) + A7[i][1] * work(1, j) + A7[i][2] * work(2, j) + A7[i][3] * work(3, j) + A7[i][4] * work(4, j) + A7[i][5] * work(5, j) + A7[i][6] * work(6, j) + A7[i][7] * work(7, j) + A7[i][8] * work(8, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec7[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec7[0] * work(0, j) + evec7[1] * work(1, j) + evec7[2] * work(2, j) + evec7[3] * work(3, j) + evec7[4] * work(4, j) + evec7[5] * work(5, j) + evec7[6] * work(6, j) + evec7[7] * work(7, j) + evec7[8] * work(8, j) + evec7[9] * work(9, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Verner76_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S7 + 2) * n * sizeof(double));
	return work;
}
void
Verner76_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Verner76_n_dim_Initial_Setting(n);
	Verner76_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P6               (6)
#define         S6               (9)
static const double A6[S6][S6] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{9.0 / 50, 0, 0, 0, 0, 0, 0, 0, 0},
	{29.0 / 324, 25.0 / 324, 0, 0, 0, 0, 0, 0, 0},
	{1.0 / 16, 0, 3.0 / 16, 0, 0, 0, 0, 0, 0},
	{79129.0 / 250000, 0, -(261237.0 / 250000), 19663.0 / 15625, 0, 0, 0, 0, 0},
	{1336883.0 / 4909125, 0, -(25476.0 / 30875), 194159.0 / 185250, 8225.0 / 78546, 0, 0, 0, 0},
	{-(2459386.0 / 14727375), 0, 19504.0 / 30875, 2377474.0 / 13615875, -(6157250.0 / 5773131), 902.0 / 735, 0, 0, 0},
	{2699.0 / 7410, 0, -(252.0 / 1235), -(1393253.0 / 3993990), 236875.0 / 72618, -(135.0 / 49), 15.0 / 22, 0, 0},
	{11.0 / 144, 0, 0, 256.0 / 693, 0, 125.0 / 504, 125.0 / 528, 5.0 / 72, 0}
};
static const double cvec6[S6] = {0, 9.0 / 50, 1.0 / 6, 1.0 / 4, 53.0 / 100, 3.0 / 5, 4.0 / 5, 1, 1};
static const double evec6[S6] = {15.0 / 848, 0, 0, -(60.0 / 539), 312500.0 / 366177, -(375.0 / 392), 125.0 / 528, 9145.0 / 71064, -(2995.0 / 17766)};

void
Verner65_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P6, s = S6;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j) + A6[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 3;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j) + A6[i][1] * work(1, j) + A6[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 4;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j) + A6[i][1] * work(1, j) + A6[i][2] * work(2, j) + A6[i][3] * work(3, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 5;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j) + A6[i][1] * work(1, j) + A6[i][2] * work(2, j) + A6[i][3] * work(3, j) + A6[i][4] * work(4, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 6;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j) + A6[i][1] * work(1, j) + A6[i][2] * work(2, j) + A6[i][3] * work(3, j) + A6[i][4] * work(4, j) + A6[i][5] * work(5, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 7;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j) + A6[i][1] * work(1, j) + A6[i][2] * work(2, j) + A6[i][3] * work(3, j) + A6[i][4] * work(4, j) + A6[i][5] * work(5, j) + A6[i][6] * work(6, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	i = 8;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A6[i][0] * work(0, j) + A6[i][1] * work(1, j) + A6[i][2] * work(2, j) + A6[i][3] * work(3, j) + A6[i][4] * work(4, j) + A6[i][5] * work(5, j) + A6[i][6] * work(6, j) + A6[i][7] * work(7, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec6[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec6[0] * work(0, j) + evec6[1] * work(1, j) + evec6[2] * work(2, j) + evec6[3] * work(3, j) + evec6[4] * work(4, j) + evec6[5] * work(5, j) + evec6[6] * work(6, j) + evec6[7] * work(7, j) + evec6[8] * work(8, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Verner65_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S6 + 2) * n * sizeof(double));
	return work;
}
void
Verner65_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Verner65_n_dim_Initial_Setting(n);
	Verner65_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P5               (5)
#define         S5               (8)
static const double A5[S5][S5] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1.0 / 6, 0, 0, 0, 0, 0, 0, 0},
	{2.0 / 27, 4.0 / 27, 0, 0, 0, 0, 0, 0},
	{183.0 / 1372, -(162.0 / 343), 1053.0 / 1372, 0, 0, 0, 0, 0},
	{68.0 / 297, -(4.0 / 11), 42.0 / 143, 1960.0 / 3861, 0, 0, 0, 0},
	{597.0 / 22528, 81.0 / 352, 63099.0 / 585728, 58653.0 / 366080, 4617.0 / 20480, 0, 0, 0},
	{174197.0 / 959244, -(30942.0 / 79937), 8152137.0 / 19744439, 666106.0 / 1039181, -(29421.0 / 29068), 482048.0 / 414219, 0, 0},
	{587.0 / 8064, 0, 4440339.0 / 15491840, 24353.0 / 124800, 387.0 / 44800, 2152.0 / 5985, 7267.0 / 94080, 0}
};
static const double cvec5[S5] = {0, 1.0 / 6, 2.0 / 9, 3.0 / 7, 2.0 / 3, 3.0 / 4, 1, 1};
static const double evec5[S5] = {3817.0 / 1959552, 0, -(140181.0 / 15491840), 4224731.0 / 272937600, -(8557.0 / 403200), 57928.0 / 4363065, 23930231.0 / 4366535040, -(3293.0 / 556956)};

void
Bogacki_Shampine54_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P5, s = S5;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A5[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A5[i][0] * work(0, j) + A5[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	i = 3;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A5[i][0] * work(0, j) + A5[i][1] * work(1, j) + A5[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	i = 4;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A5[i][0] * work(0, j) + A5[i][1] * work(1, j) + A5[i][2] * work(2, j) + A5[i][3] * work(3, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	i = 5;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A5[i][0] * work(0, j) + A5[i][1] * work(1, j) + A5[i][2] * work(2, j) + A5[i][3] * work(3, j) + A5[i][4] * work(4, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	i = 6;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A5[i][0] * work(0, j) + A5[i][1] * work(1, j) + A5[i][2] * work(2, j) + A5[i][3] * work(3, j) + A5[i][4] * work(4, j) + A5[i][5] * work(5, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	i = 7;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A5[i][0] * work(0, j) + A5[i][1] * work(1, j) + A5[i][2] * work(2, j) + A5[i][3] * work(3, j) + A5[i][4] * work(4, j) + A5[i][5] * work(5, j) + A5[i][6] * work(6, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec5[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec5[0] * work(0, j) + evec5[1] * work(1, j) + evec5[2] * work(2, j) + evec5[3] * work(3, j) + evec5[4] * work(4, j) + evec5[5] * work(5, j) + evec5[6] * work(6, j) + evec5[7] * work(7, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Bogacki_Shampine54_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S5 + 2) * n * sizeof(double));
	return work;
}
void
Bogacki_Shampine54_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Bogacki_Shampine54_n_dim_Initial_Setting(n);
	Bogacki_Shampine54_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P4               (4)
#define         S4               (5)
static const double A4[S4][S4] = {
	{0, 0, 0, 0, 0},
	{2.0 / 5, 0, 0, 0, 0},
	{-(3.0 / 20), 3.0 / 4, 0, 0, 0},
	{19.0 / 44, -(15.0 / 44), 10.0 / 11, 0, 0},
	{11.0 / 72, 25.0 / 72, 25.0 / 72, 11.0 / 72, 0}
};
static const double cvec4[S4] = {0, 2.0 / 5, 3.0 / 5, 1, 1};
static const double evec4[S4] = {119041.0 / 8970912, -(595205.0 / 8970912), 595205.0 / 8970912, 1309451.0 / 8970912, -(119041.0 / 747576)};

void
Wolfram43_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P4, s = S4;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec4[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A4[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec4[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A4[i][0] * work(0, j) + A4[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec4[i]);

	i = 3;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A4[i][0] * work(0, j) + A4[i][1] * work(1, j) + A4[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec4[i]);

	i = 4;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A4[i][0] * work(0, j) + A4[i][1] * work(1, j) + A4[i][2] * work(2, j) + A4[i][3] * work(3, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec4[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec4[0] * work(0, j) + evec4[1] * work(1, j) + evec4[2] * work(2, j) + evec4[3] * work(3, j) + evec4[4] * work(4, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Wolfram43_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S4 + 2) * n * sizeof(double));
	return work;
}
void
Wolfram43_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Wolfram43_n_dim_Initial_Setting(n);
	Wolfram43_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P3               (3)
#define         S3               (4)
#define         SQ82            (9.0553851381374166265738081669840664)
static const double A3[S3][S3] = {{0, 0, 0, 0}, {1.0 / 2, 0, 0, 0}, {-1, 2, 0, 0}, {1.0 / 6, 2.0 / 3, 1.0 / 6, 0}};
static const double cvec3[S3] = {0, 1.0 / 2, 1, 1};
static const double evec3[S3] = {1.0 / 72 * (-10 + SQ82), 1.0 / 36 * (10 - SQ82), 1.0 / 144 * (28 - SQ82), 1.0 / 48 * (-16 + SQ82)};

void
Wolfram32_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P3, s = S3;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec3[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A3[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec3[i]);

	i = 2;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A3[i][0] * work(0, j) + A3[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec3[i]);

	i = 3;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A3[i][0] * work(0, j) + A3[i][1] * work(1, j) + A3[i][2] * work(2, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec3[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec3[0] * work(0, j) + evec3[1] * work(1, j) + evec3[2] * work(2, j) + evec3[3] * work(3, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Wolfram32_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S3 + 2) * n * sizeof(double));
	return work;
}
void
Wolfram32_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Wolfram32_n_dim_Initial_Setting(n);
	Wolfram32_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define         P2               (2)
#define         S2               (3)
static const double A2[S2][S2] = {{0, 0, 0}, {1, 0, 0}, {1.0 / 2, 1.0 / 2, 0}};
static const double cvec2[S2] = {0, 1, 1};
static const double evec2[S2] = {-(1.0 / 2), 2.0 / 3, -(1.0 / 6)};

void
Wolfram21_n_dim_core(int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double          sc, err, tmp;
	double          facmax = FACMAX;
	double          facmin = FACMIN;
	double          facsave = FACSAVE;
	double          t = *time;
	double          dh = *h;
	int             i, j;
	int             p = P2, s = S2;

	i = 0;
	for (j = 0; j < n; j++)
		work(s, j) = x[j];
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec2[i]);

	i = 1;
	for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A2[i][0] * work(0, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec2[i]);

	i = 2;
    for (j = 0; j < n; j++)
		work(s, j) = x[j] + dh * (A2[i][0] * work(0, j) + A2[i][1] * work(1, j));
	(*my_function) (n, &(work(s, 0)), &(work(i, 0)), t + dh * cvec2[i]);

	for (j = 0; j < n; j++)
		work(s + 1, j) = dh * (evec2[0] * work(0, j) + evec2[1] * work(1, j) + evec2[2] * work(2, j));

	err = 0.0;
	for (j = 0; j < n; j++)
	{
		sc = Atol + MAX(fabs(x[j]), fabs(work(s, j))) * Rtol;
		tmp = work(s + 1, j) / sc;
		err += tmp * tmp;
	}
	err = sqrt(err / n);
	if (err < 1.0)
	{
		for (j = 0; j < n; j++)
			x[j] = work(s, j);
		*time += dh;
	}
	*h = dh * MIN(facmax, MAX(facmin, facsave * pow(err, -1.0 / p)));
}
double         *
Wolfram21_n_dim_Initial_Setting(int n)
{
	double         *work;
	work = (double *) malloc((S2 + 2) * n * sizeof(double));
	return work;
}
void
Wolfram21_n_dim(int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	work = Wolfram21_n_dim_Initial_Setting(n);
	Wolfram21_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	free(work);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Multiple_n_dim_core(int p, int n, double *x, double *time, double *h, double *work, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	if (p == 2)
	{
		Wolfram21_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 3)
	{
		Wolfram32_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 4)
	{
		Wolfram43_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 5)
	{
		Bogacki_Shampine54_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 6)
	{
		Verner65_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 7)
	{
		Verner76_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 8)
	{
		Verner87_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 9)
	{
		Verner98_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 10)
	{
		Bogaki_Shampine32_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else if (p == 11)
	{
		Dormand_Prince54_n_dim_core(n, x, time, h, work, Atol, Rtol, my_function);
	} else
	{
		printf("Err. Please Input method number (2 ~ 11)\n");
	}
}
double         *
Multiple_n_dim_Initial_Setting(int p, int n)
{
	double         *work;
	if (p == 2)
	{
		work = (double *) malloc((S2 + 2) * n * sizeof(double));
		return work;
	} else if (p == 3)
	{
		work = (double *) malloc((S3 + 2) * n * sizeof(double));
		return work;
	} else if (p == 4)
	{
		work = (double *) malloc((S4 + 2) * n * sizeof(double));
		return work;
	} else if (p == 5)
	{
		work = (double *) malloc((S5 + 2) * n * sizeof(double));
		return work;
	} else if (p == 6)
	{
		work = (double *) malloc((S6 + 2) * n * sizeof(double));
		return work;
	} else if (p == 7)
	{
		work = (double *) malloc((S7 + 2) * n * sizeof(double));
		return work;
	} else if (p == 8)
	{
		work = (double *) malloc((S8 + 2) * n * sizeof(double));
		return work;
	} else if (p == 9)
	{
		work = (double *) malloc((S9 + 2) * n * sizeof(double));
		return work;
	} else if (p == 10)
	{
		work = (double *) malloc((S10 + 2) * n * sizeof(double));
		return work;
	} else if (p == 11)
	{
		work = (double *) malloc((S11 + 2) * n * sizeof(double));
		return work;
	} else
	{
		printf("Err. Please Input method number (2 ~ 11)\n");
		work = (double *) malloc(0);
		return work;
	}
}
void
Multiple_n_dim(int p, int n, double *x, double *time, double *h, double Atol, double Rtol, void (*my_function) (int n, double *u, double *u_dot, double time))
{
	double         *work;

	if (1 < p && p < 12)
	{
		work = Multiple_n_dim_Initial_Setting(p, n);
		Multiple_n_dim_core(p, n, x, time, h, work, Atol, Rtol, my_function);
		free(work);
	} else
	{
		printf("Err. Please Input method number (2 ~ 11)\n");
		exit(0);
	}
}
