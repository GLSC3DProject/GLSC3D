/*******************************************************************************
//
//  g_isosurface.c & g_isosurface.h
//  Type0
//
//  Created by masakazu on 2014/05/20.
//  Copyright © 2014.5.20 Masakazu Akiyama All Rights Reserved.
//
*******************************************************************************/
#include<stdio.h>
#include<stdlib.h>

#include "glsc3d_private.h"
#define WIRE_OR_FILL  (G_FILL)
#define DIVIDE_LEVEL    (0)
typedef struct
{
    G_VECTOR r[15 + 1];
    double   value[15 + 1];
} MarchingCube;

typedef struct
{
    G_VECTOR r[28];
    double   value[28];
} MarchingTetrahedron;

typedef struct
{
    G_VECTOR r;
    int flag;
} MarchingEdge;

static MarchingEdge Judgeheight(G_VECTOR r0,G_VECTOR r1,double v0, double v1,double c)
{
    MarchingEdge    ans;

    ans.flag = 0;
    if(v1 < c && c < v0)
    {
        ans.flag = -1;
		ans.r = r1 + (c - v1) / (v0 - v1) * (r0 - r1);
    }
    if(v0 < c && c < v1)
    {
		ans.flag = 1;
		ans.r = r1 + (v1 - c) / (v1 - v0) * (r0 - r1);
    }
    return ans;
}
static const int CubleList[9][3] = {
	{0, 0, 0},
	{0, 0, 1},
 {1, 0, 1}, {1, 1, 1}, {0, 1, 1}, {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}
	/*
	{0.5, 0.5, 1.},
	{0.5, 0.5, 0.},
	{0.5, 0., 0.5},
	{1., 0.5, 0.5},
	{0.5, 1., 0.5},
	{0., 0.5, 0.5},
	{0.5, 0.5, 0.5}
	 */
};

static const int FaceList[28][4] = {
	{9, 1, 11, 2},
	{9, 2, 12, 3},
	{9, 3, 13, 4},
	{9, 4, 14, 1},
	{10, 6, 11, 5},
	{10, 5, 14, 8},
	{10, 8, 13, 7},
	{10, 7, 12, 6},
	{9, 11, 12, 2},
	{9, 12, 13, 3},
	{9, 13, 14, 4},
	{9, 14, 11, 1},
	{10, 11, 14, 5},
	{10, 14, 13, 8},
	{10, 13, 12, 7},
	{10, 12, 11, 6},
	{11, 6, 12, 2},
	{12, 7, 13, 3},
	{13, 8, 14, 4},
	{14, 5, 11, 1},
	{15, 9, 11, 12},
	{15, 9, 12, 13},
	{15, 9, 13, 14},
	{15, 9, 14, 11},
	{15, 10, 11, 14},
	{15, 10, 14, 13},
	{15, 10, 13, 12},
	{15, 10, 12, 11}
};

//G_POSITION g_position3Ds(G_VECTOR u)
//{
//    G_POSITION r = g_position(u.x,u.y,u.z);
//    return r;
//}
#define g_position3Ds(u) u

/*
void g_triangle_3D_cores(G_POSITION r0,G_POSITION r1,G_POSITION r2,G_WIREFILL WireFill,int level)
{
    g_triangle_3D_core(r0.x,r0.y,r0.z,
                       r1.x,r1.y,r1.z,
                       r2.x,r2.y,r2.z,
                       WireFill,level);
}
 */

static void do_isosurface(const MarchingCube *Cube,MarchingTetrahedron *Tetrahedron, MarchingEdge *Edge,
						  double height,int Table1[6][5],int Table2[7][3])
{

}
/*
static void do_isosurface(const MarchingCube *Cube,MarchingTetrahedron *Tetrahedron, MarchingEdge *Edge,
                           double height,int Table1[6][5],int Table2[7][3])
{
    int UraOmoteFlag;
    int SankakuShikaku;
    int EdgeMember[4];
    G_VECTOR    n,n1;
    G_POSITION r0, r1, r2, r3;

    int l,m;
    for(l = 1;l <= 5; l++)//Tetrahedron
    {
        for(m = 1;m <= 4;m ++)//Vertex
        {
            Tetrahedron->r[m]     = Cube->r[Table1[l][m]];
            Tetrahedron->value[m] = Cube->value[Table1[l][m]];
        }

        for(m = 1;m <= 6;m ++)//Edge
        {
            Edge[m] = Judgeheight(Tetrahedron->r[Table2[m][1]], Tetrahedron->r[Table2[m][2]],
                                  Tetrahedron->value[Table2[m][1]], Tetrahedron->value[Table2[m][2]],
                                  height);
        }

        UraOmoteFlag = 0;
        SankakuShikaku = 0;
        for(m = 1;m <= 6;m ++)//Edge
        {
            if(Edge[m].flag != 0)
            {
                EdgeMember[SankakuShikaku] = m;
                UraOmoteFlag += Edge[m].flag;
                SankakuShikaku ++;
            }
        }
        if(SankakuShikaku != 0)
        {
            if(SankakuShikaku == 3)
            {
                r0 = g_position3Ds(Edge[EdgeMember[0]].r);
				r1 = g_position3Ds(Edge[EdgeMember[2]].r), r2 = g_position3Ds(Edge[EdgeMember[1]].r);
                g_triangle_3D_cores(r0,r1,r2,DIVIDE_LEVEL,WIRE_OR_FILL);
            }
            else
            {
                if(EdgeMember[0] == 1 && EdgeMember[1] == 2 && EdgeMember[2] == 5 && EdgeMember[3] == 6)
                {
                    r0 = g_position3Ds(Edge[1].r);
                    r2 = g_position3Ds(Edge[5].r);
					r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
                }
                else if(EdgeMember[0] == 1 && EdgeMember[1] == 3 && EdgeMember[2] == 4 && EdgeMember[3] == 5)
                {
                    r0 = g_position3Ds(Edge[1].r);
                    r2 = g_position3Ds(Edge[5].r);
					r1 = g_position3Ds(Edge[4].r),r3 = g_position3Ds(Edge[3].r);
                }
                else
                {
                    r0 = g_position3Ds(Edge[3].r);
                    r2 = g_position3Ds(Edge[4].r);
					r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
                }
                g_triangle_3D_cores(r0,r1,r2,DIVIDE_LEVEL,WIRE_OR_FILL);
                g_triangle_3D_cores(r0,r2,r3,DIVIDE_LEVEL,WIRE_OR_FILL);
            }
        }
    }
}
*/

#define     u(i, j, k)              u[(k) * (number_y * number_x) + (j) * (number_x) + (i)]
void g_isosurface_f_3D(double x0,double x1,
                       double y0,double y1,
                       double z0,double z1,
                       int number_x,int number_y,int number_z,
                       double *u,
                       double height)
{
	MarchingCube Cube;
	G_VECTOR L = G_VECTOR(x1 - x0,y1 - y0,z1 - z0);
	G_VECTOR d = G_VECTOR(L.x / (number_x - 1),L.y / (number_y - 1),L.z / (number_z - 1));
	G_VECTOR r;
	int i,j,k,l,m;
	for(k = 0;k < number_z - 1;k ++)
	{
		r.z = (k + 0.5) * d.z - L.z / 2;
		for (j = 0; j < number_y - 1; j ++)
		{
			r.y = (j + 0.5) * d.y - L.y / 2;
			for(i = 0; i < number_x - 1; i ++)
			{
				r.x = (i + 0.5) * d.x - L.x / 2;
				for(l = 0;l < 8;l ++)
				{
					Cube.value[l + 1] =
					u(i + CubleList[l + 1][0],
					  j + CubleList[l + 1][1],
					  k + CubleList[l + 1][2]);
					Cube.r[l + 1] = G_VECTOR(r.x + (2 * CubleList[l + 1][0] - 1) * d.x / 2,
											 r.y + (2 * CubleList[l + 1][1] - 1) * d.y / 2,
											 r.z + (2 * CubleList[l + 1][2] - 1) * d.z / 2);
				}
				l = 9;
				Cube.value[l] = (Cube.value[1] + Cube.value[2] + Cube.value[3] + Cube.value[4]) / 4;
				Cube.r[l] = (Cube.r[1] + Cube.r[2] + Cube.r[3] + Cube.r[4]) / 4;
				l = 10;
				Cube.value[l] = (Cube.value[5] + Cube.value[6] + Cube.value[7] + Cube.value[8]) / 4;
				Cube.r[l] = (Cube.r[5] + Cube.r[6] + Cube.r[7] + Cube.r[8]) / 4;
				l = 11;
				Cube.value[l] = (Cube.value[1] + Cube.value[5] + Cube.value[6] + Cube.value[2]) / 4;
				Cube.r[l] = (Cube.r[1] + Cube.r[5] + Cube.r[6] + Cube.r[2]) / 4;
				l = 12;
				Cube.value[l] = (Cube.value[2] + Cube.value[6] + Cube.value[7] + Cube.value[3]) / 4;
				Cube.r[l] = (Cube.r[2] + Cube.r[6] + Cube.r[7] + Cube.r[3]) / 4;
				l = 13;
				Cube.value[l] = (Cube.value[3] + Cube.value[7] + Cube.value[8] + Cube.value[4]) / 4;
				Cube.r[l] = (Cube.r[3] + Cube.r[7] + Cube.r[8] + Cube.r[4]) / 4;
				l = 14;
				Cube.value[l] = (Cube.value[4] + Cube.value[8] + Cube.value[5] + Cube.value[1]) / 4;
				Cube.r[l] = (Cube.r[4] + Cube.r[8] + Cube.r[5] + Cube.r[1]) / 4;
				l = 15;
				Cube.value[l] = (Cube.value[1] + Cube.value[2] + Cube.value[3] + Cube.value[4]
								 + Cube.value[5] + Cube.value[6] + Cube.value[7] + Cube.value[8]) / 8;
				Cube.r[l] = (Cube.r[1] + Cube.r[2] + Cube.r[3] + Cube.r[4]
							 + Cube.r[5] + Cube.r[6] + Cube.r[7] + Cube.r[8]) / 8;

				for(m = 0;m < 28;m ++)
				{
					G_VECTOR r[4];
					double v[4];
					MarchingEdge Edge[6];
					for(int n = 0; n < 4; n ++)
					{
						r[n] = Cube.r[FaceList[m][n]];
						v[n] = Cube.value[FaceList[m][n]];
					}
					Edge[0] = Judgeheight(r[0], r[1], v[0], v[1], height);
					Edge[1] = Judgeheight(r[0], r[2], v[0], v[2], height);
					Edge[2] = Judgeheight(r[0], r[3], v[0], v[3], height);
					Edge[3] = Judgeheight(r[1], r[2], v[1], v[2], height);
					Edge[4] = Judgeheight(r[2], r[3], v[2], v[3], height);
					Edge[5] = Judgeheight(r[3], r[1], v[3], v[1], height);

				}
			}
		}
	}
	/*
    MarchingCube Cube;
    MarchingTetrahedron Tetrahedron;
    MarchingEdge Edge[6 + 1];
	G_VECTOR    d,r;
	int i,j,k,l,ii,jj,kk;
    double xL = (x1 - x0),yL = (y1 - y0),zL = (z1 - z0);
    d = g_vector3(xL / (number_x - 1), yL / (number_y - 1), zL / (number_z - 1));
    for(k = 0;k < number_z - 1;k ++)
    {
        r.z = (k + 0.5) * d.z - zL / 2;kk = k % 2;
        for (j = 0; j < number_y - 1; j ++)
        {
            r.y = (j + 0.5) * d.y - yL / 2;jj = j % 2;
            for(i = 0; i < number_x - 1; i ++)
            {
                r.x = (i + 0.5) * d.x - xL / 2;ii = i % 2;
                for(l = 0;l < 8;l ++)
                {
                    Cube.value[l + 1] = u(i + Table[l][0], j + Table[l][1], k + Table[l][2]);
                    Cube.r[l + 1] = g_vector3(r.x + (2 * Table[l][0] - 1) * d.x / 2,
                                         r.y + (2 * Table[l][1] - 1) * d.y / 2,
                                         r.z + (2 * Table[l][2] - 1) * d.z / 2);
                }

                if(
                   (kk == 1 && jj == 1 && ii == 1)||(kk == 1 && jj == 0 && ii == 0)||
                   (kk == 0 && jj == 1 && ii == 0)||(kk == 0 && jj == 0 && ii == 1)
                   )
                    do_isosurface(&Cube, &Tetrahedron, Edge, height,Table1r,Table2r);
                else
                    do_isosurface(&Cube, &Tetrahedron, Edge, height,Table1l,Table2l);
            }
        }
    }
	 */
}
#undef u
