#include<stdlib.h>
#include<stdio.h>

#include "glsc3d.h"
#include <time.h>

//#define g_bird_view_3D(x_left,x_right,y_bottom,y_top,N_x,N_z,data, wire, fill)\
//(g_bird_view_f_3D((x_left),(x_right),(y_bottom),(y_top),(N_x),(N_z),(double(*)[N_z])(data), wire, fill))

#define INTV              (1)
#define WINDOW_SIZE_X     (1024)
#define WINDOW_SIZE_Y     (512)
#define WINDOW_POS_X      (50)
#define WINDOW_POS_Y      (50)

#define LEN         (2.0)

#define SIZE        (30)
#define Imax        (2 * SIZE)
#define Jmax        (SIZE)

//uint64_t GLSC3D_Data_Buffer_Size = 1<<20ULL;
//void *GLSC3D_Data_Buffer;
//
//void * GLSC3D_Array_Extention(int Array_Size)
//{
//    int i;
//    int ADD_GLSC3D_Data_Buffer_Size = GLSC3D_Data_Buffer_Size;
//    for (i=0;;i++) {
//        if (Array_Size > ADD_GLSC3D_Data_Buffer_Size){
//            GLSC3D_Data_Buffer = (void*)realloc(GLSC3D_Data_Buffer, GLSC3D_Data_Buffer_Size * 2);
//            if (GLSC3D_Data_Buffer == NULL) break;
//            ADD_GLSC3D_Data_Buffer_Size *= 2;
//        }
//        else {
//            GLSC3D_Data_Buffer_Size = ADD_GLSC3D_Data_Buffer_Size;
//            break;
//        }
//    }
//    return (void *)malloc(GLSC3D_Data_Buffer_Size);
//}

int main(void)
{
	g_init("GRAPH", WINDOW_SIZE_X, WINDOW_SIZE_Y);

	g_def_scale_3D_core(0, -LEN/2.0, LEN/2.0, -LEN/2.0, LEN/2.0, -LEN/2.0, LEN/2.0, 0, 0, WINDOW_SIZE_X/2, WINDOW_SIZE_Y, 1, 1, 1, 0, 0, 0, 1);
	
	g_def_scale_3D_core(1, -LEN/2.0, LEN/2.0, -LEN/2.0, LEN/2.0, -LEN/2.0, LEN/2.0, WINDOW_SIZE_X/2, 0, WINDOW_SIZE_X/2, WINDOW_SIZE_Y,1, 1, 1, 0, 0, 0, 1);
	
	g_init_light_core(0, 1, 2, 3, 1);
	//g_capture_set("");
	
	int i_time;
	double dt = 0.03;
	int i,j;
	double xx,yy,dx=LEN/Imax,dy=LEN/Jmax;
    
//	double **array2;
//    array2=(double **)malloc(sizeof(double *) * Imax);
//    for (i=0; i<=Imax; i++){
//        array2[i]=(double *)malloc(sizeof(double ) * Jmax);
//    }
    
	double *array;
    array=(double *)malloc(sizeof(double ) * Imax*Jmax);
    
//    int Data_Size = Imax*Jmax;
//    GLSC3D_Data_Buffer = GLSC3D_Array_Buffer(Data_Size*sizeof(double));
//    double *array;
//    array = (double *)GLSC3D_Data_Buffer;
    
    double array2[Imax][Jmax];
//    double array[Imax*Jmax];
    
    /* time.h */
    clock_t start, end;
    start = clock();
    
	for(i_time = 0; i_time < 1000; i_time = (i_time + 1) % (int)(2*M_PI/dt))
	{
		double t = dt*i_time;

		for (j = 0; j < Jmax; j++)
		{
			yy = dy * (j + 0.5) - LEN/2.0;
			for (i = 0; i < Imax; i++)
			{
				xx = dx * (i + 0.5) - LEN/2.0;
				array[j*Imax + i] = sin(2*xx - t) * cos(3*yy - t) ;
			}
		}
		for (i = 0; i < Imax; i++)
		{
			xx = dx * (i + 0.5) - LEN/2.0;
			for (j = 0; j < Jmax; j++)
			{
				yy = dy * (j + 0.5) - LEN/2.0;
				array2[i][j] = sin(2*xx - t) * cos(3*yy - t) ;
			}
		}
		g_cls();
		
		g_sel_scale_3D(0);
		g_area_color_3D(1, 0, 0, 1);
		g_line_color(1, 0, 0, 1);
		g_bird_view_3D(-LEN/2.0, LEN/2.0, -LEN/2.0, LEN/2.0, Imax, Jmax, array2, G_TRUE, G_FALSE);
		
		
		g_sel_scale_3D(1);
		g_area_color_3D(1,0,0,1);
		g_bird_view_f_3D(-LEN/2.0, LEN/2.0, -LEN/2.0, LEN/2.0, Imax, Jmax, array, G_FALSE, G_TRUE);
		
		g_finish();
		//g_capture();
    }
    
    end = clock();
    double result = (double)(end-start)/CLOCKS_PER_SEC;
    printf("time = %.15f\n",result);
    
    free(array);
    
//    for (i=0; i<=Jmax; i++){
//        free(array2[i]);
//    }
//    free(array2);
//    
	return 0;
}
