#include <float.h>

double g_max(int N_x, int N_z, double data[N_x][N_z])//dataの最大値を返す
{
    int i,j;
    double max = -DBL_MAX;
    for (j = 0; j < N_z; j++)
    {
        for (i = 0; i < N_x; i++)
        {
            if (data[i][j]>=max)
            {
                max = data[i][j];
            }
        }
    }
    return max;
}

double g_min(int N_x, int N_z, double data[N_x][N_z])//dataの最小値を返す
{
    int i,j;
    double min = DBL_MAX;
    for (j = 0; j < N_z; j++)
    {
        for (i = 0; i < N_x; i++)
        {
            if (data[i][j]<=min)
            {
                min = data[i][j];
            }
        }
    }
    return min;
}
