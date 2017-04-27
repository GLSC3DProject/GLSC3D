#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

void
GetParam2(int argc, char *argv[], int max_va_num,...)
{
    int ans;
	va_list         list;
	double         **d;
    d = (double **)malloc(sizeof(double *) * max_va_num);
	int             i = 0;
	va_start(list, max_va_num);
	while ((d[i] = va_arg(list, double *)) != NULL) i++;
	va_end(list);

    char    **x;
    x = (char **)malloc(sizeof(char *) * 2 * (max_va_num + 1));
    for(i = 0;i < 2 * (max_va_num + 1);i++) x[i] = (char *)malloc(sizeof(char) * (1024));

    int j,num,xlen,flag,flag1,flag2;

    if(argc > 1)
    {
        for(i = 1; i < argc; i++) sprintf(x[i],"%s",argv[i]);
        for(i = 1; i < argc; i++)
        {
            if(i % 2 == 1 && x[i][0] == '-' && x[i][1] == 'p')
            {
                xlen = strlen(x[i]);
                flag = 0;
                for(j = 2;j < xlen;j ++) if(isdigit(x[i][j]) != 0) flag ++;
                if(flag + 2 != xlen)
                {
                    printf("Error.\nAfter this character '-p' following the number[0-9] only.\n");
                    exit(0);
                }
                else
                {
                    num = atof(&x[i][2]);
                    if(num < max_va_num)
                    {
                        xlen = strlen(x[i + 1]);
                        flag = flag1 = flag2 = 0;
                        for(j = 0;j < xlen;j ++)
                        {
                            if(isdigit(x[i + 1][j]) != 0) flag ++;
                            if(x[i + 1][j] == '.') flag1 ++;
                            if(x[i + 1][j] == '-') flag2 ++;
                        }
                        if(xlen == flag2 + flag1 + flag && flag1 < 2 && flag2 < 2) *d[num] = atof(x[i + 1]);
                        else
                        {
                            printf("Error.\nAfter this character '-p%d' following real number.\n",num);
                            exit(0);
                        }
                    }
                    else
                    {
                        printf("Error.\n Your input parameter number beyonds the max paramter number\n");
                        exit(0);
                    }
                }
            }
        }
    }

    for(i = 0;i < 2 * (max_va_num + 1);i++) free(x[i]);
    free(x);
    free(d);
}
