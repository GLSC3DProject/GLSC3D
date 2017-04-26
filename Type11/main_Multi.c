#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define     N                   (27)//パラメタの数
#define     TaiwaMode           (0)//これを1にすると対話的にパラメタを入力することができる．
//#define     NumberOfCPU         (24)//コメントアウトすると自動的にCPU数が設定される．自動を推奨．

////////////////////////////////////////////////////////////////////////////////
/*これより以降は基本的にはいじらなくよい．                                           */
////////////////////////////////////////////////////////////////////////////////
#define		PI                  (3.14159265358979)
#define     BitPattern(i,j)     (BitPattern[(i) * N + (j)])

unsigned long long NumExecutableFile;
unsigned long long *MaxTable;
unsigned long long *BitPattern;
unsigned long long *ShuffleArray;

typedef struct
{
    int         EqualIntervalness;
    double		Start,End,dx;
    int			i,Imax;
    double      *Data;
}               Parameter;

typedef struct
{
    unsigned long long shou;
    unsigned long long amari;
}               Godo;

Godo CalcShouAmari(unsigned long long alpha,unsigned long long M)
{
    Godo Ans;

    Ans.amari = alpha % M;
    Ans.shou = (alpha - Ans.amari) / M;

    return Ans;
}
void CreateData(Parameter *P)
{
    int print_flags = 1;
    int i,j;

    (*P).Imax = (int)(((*P).End - (*P).Start) / (*P).dx) + 1;
    (*P).Data = (double *)malloc(sizeof(double) * (*P).Imax);

    for(i = 0; i < (*P).Imax; i ++)
        (*P).Data[i] = (*P).Start + (*P).dx * i;

    if(print_flags)
    {
        j = (*(P)).Imax;
        printf("\n(*P).Imax = %d\n",j);
        for(i = 0; i < j; i ++)
        {
            printf("(*P).Data[%3d] = %f\n",i,(*P).Data[i]);
        }
    }
}
void            CallEqualIntervalness(int size,Parameter *P)
{
    int i,j,k;

    for(i = 0;i < size;i ++)
    {
        printf("The %d Parameter is equal interval? yes = 1 or no = 0\n",i);
        scanf("%d",&(P[i].EqualIntervalness));
    }

    k = 0;
    for(i = 0;i < size;i ++)
    {
        j = P[i].EqualIntervalness;
        if(j == 0 || j == 1) k ++;
    }

    if(k == size)
        for(i = 0;i < size;i ++)
            printf("The %d Parameter EqualIntervalness is %d\n",i,j);
    else
        CallEqualIntervalness(size,P);
}
void            CallParameter(int size,Parameter *P)
{
    int i,j;
    int ParamSize;
    int flag;
    double tmp;

    for(i = 0;i < size;i ++)
    {
        //等間隔でない
        if(P[i].EqualIntervalness == 0)
        {
            flag = 1;
            do
            {
                printf("Please Input Number of %d Parameter Size:= \n",i);
                scanf("%d",&ParamSize);
                if(ParamSize > 0) flag = 0;
            }while(flag == 1);

            P[i].Imax = ParamSize;
            P[i].Data = (double *)malloc(sizeof(double) * ParamSize);

            for(j = 0;j < ParamSize;j ++)
            {
                printf("Please Input Number of %d Parameter[%5d]:= \n",i,j);
                scanf("%lf",&tmp);
                P[i].Data[j] = tmp;
            }
        }
        //等間隔である
        if(P[i].EqualIntervalness == 1)
        {
            printf("Please Input Number of %d Parameter(Min):= \n",i);
            scanf("%lf",&tmp);
            P[i].Start = tmp;
            printf("Please Input Number of %d Parameter(Max):= \n",i);
            scanf("%lf",&tmp);
            P[i].End = tmp;
            printf("Please Input Number of %d Parameter dx:= \n",i);
            scanf("%lf",&tmp);
            P[i].dx = tmp;
            CreateData(&P[i]);
        }
    }
}

void shuffle(unsigned long long * array, int size)
{
	int i,j,k;
	for(i = 0; i < size; i++)
	{
		j = rand() % size;
		k = array[i];
		array[i] = array[j];
		array[j] = k;
	}
}

void SwapBitPattern(unsigned long long times)
{
    srand((unsigned) time(NULL));
    int i;
    unsigned long long j,i0,i1;
    unsigned long long tmp;

    if(RAND_MAX < NumExecutableFile)
    {
        printf("No Swaping Data!!\a\n");
        exit(0);
    }
    else
    {
        for(j = 0;j < times;j ++)
        {
            i0 = rand() % NumExecutableFile;
            i1 = (RAND_MAX - rand()) % NumExecutableFile;

            for(i = 0;i < N;i ++)
            {
                tmp = BitPattern(i0,i);
                BitPattern(i0,i) = BitPattern(i1,i);
                BitPattern(i1,i) = tmp;
            }
        }
    }
}
int main(void)
{
    int             i,j;
    char            ExecutableFile[] = "./test_program";
	char            Shell[] = "myshell";
    char            Alphabet[N + 1][10];
    char            text[1024];
	char            msg[1024];

    for(i = 0;i < N + 1;i ++)
        sprintf(Alphabet[i], "-p%d",i);
    Parameter   *Param;
    Param = (Parameter *)malloc(sizeof(Parameter) * N);

    if(TaiwaMode == 1)
    {
        CallEqualIntervalness(N,Param);
        CallParameter(N,Param);
    }
    else
    {
        i = 0;//UseSeed
        Param[i].Start = 1;
        Param[i].End = 100;
        Param[i].dx = 1;
        CreateData(&Param[i]);
		i = 1;//DrawFurrowArea
		Param[i].Start = 1;
		Param[i].End = 1;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 2;//Capture
		Param[i].Start = 2;
		Param[i].End = 2;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 3;//tolerance
		Param[i].Start = 1.0e-8;
		Param[i].End = 1.0e-8;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 4;//Paraml
		Param[i].Start = 0.5;
		Param[i].End = 0.5;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 5;//Paramlx
		Param[i].Start = 0.5;
		Param[i].End = 0.5;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 6;//Paramg
		Param[i].Start = 0.0;
		Param[i].End = 0.0;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 7;//mu
		Param[i].Start = 3 * PI / 2;
		Param[i].End = 3 * PI / 2;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 8;//kappa
		Param[i].Start = 0.0;
		Param[i].End = 0.0;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 9;//muDs
		Param[i].Start = 3 * PI / 2;
		Param[i].End = 3 * PI / 2;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 10;//kappaDs
		Param[i].Start = 0.25;
		Param[i].End = 0.25;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 11;//StopMarkerThreshold
		Param[i].Start = 1.0e-7;
		Param[i].End = 1.0e-7;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 12;//Furrow_A_haba_x
		Param[i].Start = 2 * 0.1875;
		Param[i].End = 2 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 13;//Furrow_A_haba_y
		Param[i].Start = 4 * 0.1875;
		Param[i].End = 4 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 14;//Furrow_A_deepness
		Param[i].Start = 6 * 0.1875;
		Param[i].End = 6 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 15;//Furrow_A_n
		Param[i].Start = 4;
		Param[i].End = 4;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 16;//Furrow_A_theta
		Param[i].Start = 0;
		Param[i].End = 0;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 17;//Furrow_M_haba_x
		Param[i].Start = 8 * 0.1875;
		Param[i].End = 8 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 18;//Furrow_M_haba_y
		Param[i].Start = 24 * 0.1875;
		Param[i].End = 24 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 19;//Furrow_M_deepness
		Param[i].Start = 10 * 0.1875;
		Param[i].End = 10 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 20;//Furrow_M_n
		Param[i].Start = 4;
		Param[i].End = 4;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 21;//Furrow_M_theta
		Param[i].Start = 0;
		Param[i].End = 0;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 22;//Furrow_P_haba_x
		Param[i].Start = 8 * 0.1875;
		Param[i].End = 8 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 23;//Furrow_P_haba_y
		Param[i].Start = 24 * 0.1875;
		Param[i].End = 24 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 24;//Furrow_P_deepness
		Param[i].Start = 10 * 0.1875;
		Param[i].End = 10 * 0.1875;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 25;//Furrow_P_n
		Param[i].Start = 4;
		Param[i].End = 4;
		Param[i].dx = 1;
		CreateData(&Param[i]);
		i = 26;//Furrow_P_theta
		Param[i].Start = PI / 6;
		Param[i].End = PI / 6;
		Param[i].dx = 1;
		CreateData(&Param[i]);
    }
    /* Get Now Time */
    time_t timer;
	struct tm *t_st;
    time(&timer);
	t_st = localtime(&timer);
    char		LocalTime[256];

    sprintf(LocalTime,"%d.%d.%d.%d.%d.%d",
			t_st -> tm_year + 1900,
			t_st -> tm_mon + 1,
			t_st -> tm_mday,
			t_st -> tm_hour,
			t_st -> tm_min,
			t_st -> tm_sec
			);
    /* Get CPU Core Num*/
    int         CPUCORENUM;
    char        Confinfo[256],ConfinfoFileName[256];

    sprintf(ConfinfoFileName,"/tmp/getconf_%s",LocalTime);
    sprintf(Confinfo,"getconf _NPROCESSORS_ONLN > %s",ConfinfoFileName);
    system(Confinfo);
    FILE    *Confinfofp;
    Confinfofp = fopen (ConfinfoFileName,"r");
    if (Confinfofp == NULL)
        CPUCORENUM = 1;
    else
        fscanf(Confinfofp,"%d",&CPUCORENUM);

#ifdef NumberOfCPU
    CPUCORENUM = NumberOfCPU;
#endif
    FILE		*fp[CPUCORENUM],*fp2;
    for(i = 0;i < CPUCORENUM;i ++)
	{
		sprintf(text,"%s%d.sh",Shell,i);
		fp[i] = fopen(text,"w");
		sprintf(msg,"chmod +x %s\n",text);
		system(msg);
	}

	sprintf(text,"%s.sh",Shell);
	fp2 = fopen(text,"w");
    i = 0;
    fprintf(fp2,"./%s%d.sh &\n",Shell,i);
    if(CPUCORENUM > 1)
    {
        for(i = 1;i < CPUCORENUM - 1;i ++)
        {
            fprintf(fp2,"sleep 1.0; ./%s%d.sh &\n",Shell,i);
        }
        fprintf(fp2,"sleep 1.0; ./%s%d.sh \n",Shell,i);
    }

	sprintf(msg,"chmod +x %s\n",text);
	system(msg);
	fclose(fp2);

    //多重for文
    MaxTable = (unsigned long long * ) malloc(sizeof(unsigned long long) * N);
    NumExecutableFile = 1;
    for(i = 0;i < N;i ++)
    {
        j = Param[i].Imax;
        NumExecutableFile *= j;
        MaxTable[i] = j;
    }
    BitPattern = (unsigned long long *) malloc(sizeof(unsigned long long) * N * NumExecutableFile);
	ShuffleArray = (unsigned long long *) malloc(sizeof(unsigned long long) * NumExecutableFile);
    unsigned long long k;

	for(k = 0;k < NumExecutableFile;k ++)
		ShuffleArray[k] = k;
    for(k = 0;k < NumExecutableFile;k ++)
        for(i = 0;i < N;i ++)
            BitPattern(k,i) = 0;
    k = 0;
    for(i = 0;i < N;i ++)
        BitPattern(k,i) = 0;

    Godo tmp;
    for(k = 1;k < NumExecutableFile;k ++)
    {
        i = 0;
        tmp = CalcShouAmari(k, MaxTable[i]);
        BitPattern(k,N - 1) = tmp.amari;
        for(i = 1;i < N;i ++)
        {
            tmp = CalcShouAmari(tmp.shou, MaxTable[i]);
            BitPattern(k,N - i - 1) = tmp.amari;
        }
    }

    //SwapBitPattern(0);
	shuffle(ShuffleArray,NumExecutableFile);
	/*
	for(k = 0;k < NumExecutableFile;k ++)
	{
		printf("\n|%5lld|",ShuffleArray[k]);
		for(i = 0;i < N;i ++)
		{
			printf("%5lld ",BitPattern(ShuffleArray[k],i));
		}
	}
	printf("\n");
	 */
	/*
    SwapBitPattern(NumExecutableFile);
	for(k = 0;k < NumExecutableFile;k ++)
	{
		printf("\n|%5lld|",k);
		for(i = 0;i < N;i ++)
		{
			printf("%5lld ",BitPattern(k,i));
		}
	}
	printf("\n");
	 */
    char        *mess;
    mess = (char *) malloc(sizeof(char) * (N + 1) * 50);

    for(k = 0;k < NumExecutableFile;k ++)
    {
        sprintf(mess,"%s ",ExecutableFile);
        for(i = 0; i < N;i ++)
            sprintf(mess,"%s %s %15.15lf",mess,Alphabet[i],Param[i].Data[BitPattern(ShuffleArray[k],N - 1 - i)]);
        sprintf(mess,"%s %s %15.15lf \n",mess,Alphabet[i],(double)k);
        fprintf(fp[k % CPUCORENUM],"%s",mess);
    }

    for(i = 0;i < CPUCORENUM;i ++)
		fclose(fp[i]);

    free(MaxTable);
    free(BitPattern);
    free(Param);
    free(mess);

	sprintf(msg,"mkdir -p ShellBox");
	system(msg);
	sprintf(msg,"mv %s*.sh ShellBox",Shell);
	system(msg);
    printf("NumExecutableFile is %lld\n",NumExecutableFile);
    NumExecutableFile /= CPUCORENUM;
    printf("The Simulation time is ......\n");
    printf("-----------------------------------------------------\n");
    printf("-----------------------------------------------------\n");
    printf("--------------------- 0.1 [sec] ---------------------\n");
    printf("%5.5f [sec] %5.5f [min] %5.5f[hour] %5.5f[days] \n",NumExecutableFile * 0.1,NumExecutableFile * 0.1 / 60 ,NumExecutableFile * 0.1 / 3600,NumExecutableFile * 0.1 / 3600 / 24);
    printf("-----------------------------------------------------\n");
    printf("--------------------- 1.0 [sec] ---------------------\n");
    printf("%5.5f [sec] %5.5f [min] %5.5f[hour] %5.5f[days] \n",NumExecutableFile * 1.0,NumExecutableFile * 1.0 / 60 ,NumExecutableFile * 1.0 / 3600,NumExecutableFile * 1.0 / 3600 / 24);
    printf("-----------------------------------------------------\n");
    printf("--------------------- 10.0 [sec] ---------------------\n");
    printf("%5.5f [sec] %5.5f [min] %5.5f[hour] %5.5f[days] \n",NumExecutableFile * 10.0,NumExecutableFile * 10.0 / 60 ,NumExecutableFile * 10.0 / 3600,NumExecutableFile * 10.0 / 3600 / 24);
    printf("-----------------------------------------------------\n");
    printf("-----------------------------------------------------\n");
    printf("--------------------- 100.0 [sec] ---------------------\n");
    printf("%5.5f [sec] %5.5f [min] %5.5f[hour] %5.5f[days] \n",NumExecutableFile * 100.0,NumExecutableFile * 100.0 / 60 ,NumExecutableFile * 100.0 / 3600,NumExecutableFile * 100.0 / 3600 / 24);
    printf("-----------------------------------------------------\n");
    return 0;
}

