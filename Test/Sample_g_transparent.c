#include <glsc3d.h>
#include <stdio.h>

#define PI M_PI
#define G_REAL double

int main()
{
	double scale = 1;
	double t = 0, dt = 0.01;
	G_POSITION eye[4];
	double shu = 10;
	
	g_init_core("GRAPH", 1280, 960,50,50,0,0,0,G_TRUE,100*24,200000);

    FILE *fp;
	fp = fopen("data.txt","a");

	while(t < shu * 2 * PI)
	{
		t += dt;
		for(int id = 0;id < 4;id ++)
		{
            eye[id].x = cos(t);
            eye[id].y = sin(t);
            eye[id].z = cos(t + id / 4.0 * 2 * PI);
		}
		g_def_scale_3D_core(0, -scale, scale, -scale, scale, -scale, scale,   0,   0, 640, 480, eye[0].x, eye[0].y, eye[0].z, scale*20, 0, 0, 1);
		g_def_scale_3D_core(1, -scale, scale, -scale, scale, -scale, scale, 640,   0, 640, 480, eye[1].x, eye[1].y, eye[1].z, scale*20, 0, 0, 1);
		g_def_scale_3D_core(2, -scale, scale, -scale, scale, -scale, scale,   0, 480, 640, 480, eye[2].x, eye[2].y, eye[2].z, scale*20, 0, 0, 1);
		g_def_scale_3D_core(3, -scale, scale, -scale, scale, -scale, scale, 640, 480, 640, 480, eye[3].x, eye[3].y, eye[3].z, scale*20, 0, 0, 1);

		g_init_light_core(0,1,1,1,1);
		g_init_light_core(1,-1,-1,-1,1);

		g_cls();
		g_sleep(0.01);
		for(int id = 0;id < 4;id ++)
		{
			g_sel_scale_3D(id);

			g_area_color_3D(1, 0, 0, 0.4);
			//g_line_width(2);
			g_line_color(1,0, 0, 1);
			g_arrow_3D(-2*scale, 0, 0, 1, 0, 0, 4*scale, 0.1*scale);

			g_area_color_3D(0, 1, 0, 0.4);
			g_line_color(0, 1, 0, 1);
			g_arrow_3D(0, -2*scale, 0, 0, 1, 0, 4*scale, 0.1*scale);

			g_area_color_3D(0, 0, 1, 0.4);
			g_line_color(0, 0, 1, 1);
			g_arrow_3D(0, 0, -2*scale, 0, 0, 1, 4*scale, 0.1*scale);

			g_area_color_3D(1, 0, 1, 1);

			/********************test sphere*******************************/
			//g_sphere_3D_core(0, 0, 0, scale*1, (int)d_order, 0, G_WIRE);

			/********************test sphere*******************************/
//			g_cylinder_3D_core(0, 0, 0,
//					1, 1, 1,
//					1, 2, 0,
//					5, 0, G_FALSE, G_FILL);

			/********************test sphere*******************************/
			g_cone_3D_core(0.5, 1, 0.5,
			  1, 1, 1,
			  1, 2, 100, 0, G_FILL);

//			g_arrow_3D_core(0, 0, 0, 1, 0, 1, 2*scale, 0.5*scale, 0, G_WIRE);

			/********************test sphere*******************************/
//			g_ellipse_3D_core(1, 0.5, 0.5,
//					1, 1, 1,
//					1, 1, 1,
//					6, 0, G_FILL);

			/********************test sphere*******************************/
//			            g_prism_3D_core(0, 0, 0,
//			                            1, 1, 1,
//			                            1, 2, 0, 5,
//			                            0, G_WIRE, G_FILL);

			/********************test triangle*******************************/
			//            double length=2;
			//
			//            g_triangle_3D_core(length,0,0,
			//                               0,length,0,
			//                               0,0,length,
			//                               2,G_FILL);

			/********************test fan*******************************/
//			            g_fan_3D_core(1,1,1,
//			                          -1,1,-1,
//			                          1,0.5*M_PI,0,
//			                          5,0,G_FILL);

			/********************test circle*******************************/
			//            g_circle_3D_core(0,0,0,1,
			//                             0,0,10,
			//                             0,0);

			/********************test circle*******************************/
			/*g_box_3D_core(0,0,0,
			  1,1,1,
			  0, G_WIRE);
			 */
			/********************test circle*******************************/
//			g_pyramid_3D_core(0,0,0,
//			  0,0,1,
//			  1,1,0,
//			  5,5,G_FILL);

			/********************test circle*******************************/
			//            g_rectangle_3D_core(0,0,0,
			//                                0,1,0,
			//                                1,2,0,
			//                                1,0);

			/*******************test bird_view******************************/
			//            int i_time;
			//            int Imax = 30,Jmax = 20;
			//            int i,j;
			//            double xx,yy,dx=4.0/Imax,dy=4.0/Jmax;
			//            double array[Imax*Jmax];
			//            double array2[Imax][Jmax];
			//            
			//            for (i = 0; i < Imax; i++)
			//            {
			//                xx = dx * i;
			//                for (j = 0; j < Jmax; j++)
			//                {
			//                    yy = dy * j;
			//                    array[i*Jmax+j] = sin(2*xx*0.1*t - 1) * cos(3*yy*0.1*t - 0.5);
			//                    array2[i][j] = sin(2*xx*0.1*t - 1) * cos(3*yy*0.1*t - 0.5);
			//                }
			//            }
			//            //g_bird_view_3D(-1, 1, -1, 1, -1, 1, Imax, Jmax, array2, 1);
			//            g_bird_view_f_3D(-1, 1, -1, 1, -1, 1, Imax, Jmax, array, 1);

        }
		g_finish();
	}
}

