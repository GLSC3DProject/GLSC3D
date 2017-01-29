/*******************************************************************************
<<<<<<< HEAD
//  g_isosurface.c & g_isosurface.h
//  Type0
//
<<<<<<< HEAD
//  Created by masakazu on 2014/05/20.
=======
//  Created by masakazu on 2014/11/06.
>>>>>>> 408ef24d6aad516d7dd4ef6de2be084c255466f5
=======
//  g_isosurface.c
//
//  Created by masakazu on 2014/11/06.
>>>>>>> e5a7cf6836f24c34aea38afd5e42ed3a105350c5
//  Copyright © 2014.5.20 Masakazu Akiyama All Rights Reserved.
//
*******************************************************************************/
#include<stdio.h>
#include<stdlib.h>

#include "glsc3d_private.h"
#define NURITUBUSI  (G_FILL)
#define DIVIDE_LEVEL    (0)
typedef struct
{
    G_VECTOR r[8 + 1];
    double   value[8 + 1];
} MarchingCube;

typedef struct
{
    G_VECTOR r[4 + 1];
    double   value[4 + 1];
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
        ans.r = g_plus(r1, g_multi((c - v1) / (v0 - v1), g_minus(r0, r1)));
    }
    if(v0 < c && c < v1)
    {
		ans.flag = 1;
		ans.r = g_plus(r1, g_multi((v1 - c) / (v1 - v0), g_minus(r0, r1)));
    }
    return ans;
}

G_POSITION g_position3Ds(G_VECTOR u)
{
    G_POSITION r = g_position(u.x,u.y,u.z);
    return r;
}
void g_triangle_3D_cores(G_POSITION r0,G_POSITION r1,G_POSITION r2,G_WIREFILL WireFill,int level)
{
    g_triangle_3D_core(r0.x,r0.y,r0.z,
                       r1.x,r1.y,r1.z,
                       r2.x,r2.y,r2.z,
                       WireFill,level);
}
static void do_isosurface(MarchingCube Cube,MarchingTetrahedron *Tetrahedron, MarchingEdge *Edge,
                           double height,int Table1[6][5],int Table2[7][3])
{
    int UraOmoteFlag;
    int SankakuShikaku;
    int EdgeMember[4];
    G_VECTOR    n = {},n1 = {};
    G_POSITION r0, r1, r2, r3;

    int l,m;
    for(l = 1;l <= 5; l++)//Tetrahedron
    {
        for(m = 1;m <= 4;m ++)//Vertex
        {
            Tetrahedron->r[m]     = Cube.r[Table1[l][m]];
            Tetrahedron->value[m] = Cube.value[Table1[l][m]];
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

                if(g_dot(n,n1) > 0) r1 = g_position3Ds(Edge[EdgeMember[1]].r), r2 = g_position3Ds(Edge[EdgeMember[2]].r);
                else r1 = g_position3Ds(Edge[EdgeMember[2]].r), r2 = g_position3Ds(Edge[EdgeMember[1]].r);

                g_triangle_3D_cores(r0,r1,r2,DIVIDE_LEVEL,NURITUBUSI);
            }
            else
            {
                if(EdgeMember[0] == 1 && EdgeMember[1] == 2 && EdgeMember[2] == 5 && EdgeMember[3] == 6)
                {
                    r0 = g_position3Ds(Edge[1].r);
                    r2 = g_position3Ds(Edge[5].r);

                    if(g_dot(n,n1) > 0) r1 = g_position3Ds(Edge[2].r), r3 = g_position3Ds(Edge[6].r);
                    else r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
                }
                else if(EdgeMember[0] == 1 && EdgeMember[1] == 3 && EdgeMember[2] == 4 && EdgeMember[3] == 5)
                {
                    r0 = g_position3Ds(Edge[1].r);
                    r2 = g_position3Ds(Edge[5].r);

                    if(g_dot(n,n1) > 0) r1 = g_position3Ds(Edge[3].r), r3 = g_position3Ds(Edge[4].r);
                    else r1 = g_position3Ds(Edge[4].r),r3 = g_position3Ds(Edge[3].r);
                }
                else
                {
                    r0 = g_position3Ds(Edge[3].r);
                    r2 = g_position3Ds(Edge[4].r);

                    if(g_dot(n,n1) < 0) r1 = g_position3Ds(Edge[2].r),r3 = g_position3Ds(Edge[6].r);
                    else r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
                }
                g_triangle_3D_cores(r0,r1,r2,DIVIDE_LEVEL,NURITUBUSI);
                g_triangle_3D_cores(r0,r2,r3,DIVIDE_LEVEL,NURITUBUSI);
            }
        }
    }
}

int Table[8][3] = {
    {0,0,1},
    {1,0,1},
    {1,1,1},
    {0,1,1},
    {0,0,0},
    {1,0,0},
    {1,1,0},
    {0,1,0}
};
int Table1l[6][5] = {
    {0,0,0,0,0},
    {0,1,4,5,2},
    {0,3,2,7,4},
    {0,8,7,5,4},
    {0,6,5,7,2},
    {0,4,7,5,2},
};

int Table1r[6][5] = {
	{0,0,0,0,0},
	{0,4,1,3,8},
	{0,2,3,1,6},
	{0,5,6,1,8},
	{0,7,6,8,3},
	{0,1,6,3,8},
};

int Table2l[7][3] = {
    {0,0,0},
    {0,1,2},
    {0,1,4},
    {0,1,3},
    {0,4,2},
    {0,3,4},
    {0,2,3},
};

int Table2r[7][3] = {
    {0,0,0},
    {0,1,2},
    {0,1,3},
    {0,1,4},
    {0,2,3},
    {0,3,4},
    {0,4,2},
};

#define     u(i, j, k)              u[(k) * (number_y * number_x) + (j) * (number_x) + (i)]
void g_isosurface_f_3D(double x0,double x1,
                       double y0,double y1,
                       double z0,double z1,
                       int number_x,int number_y,int number_z,
                       double *u,
                       double height)
{
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
                    do_isosurface(Cube, &Tetrahedron, Edge, height,Table1r,Table2r);
                else
                    do_isosurface(Cube, &Tetrahedron, Edge, height,Table1l,Table2l);
            }
        }
    }
}
#undef u
////////////////////////////////////////////////////////////////////////////////
//#define FAST_CODE
void g_isosurface_3D(double x0,double x1,
                     double y0,double y1,
                     double z0,double z1,
                     int number_x,int number_y,int number_z,
                     double u[number_x][number_y][number_z],
                     double height)
{
    //g_isosurface_f_3D(x0,x1,y0,y1,z0,z1,number_x,number_y,number_z,&u[0][0][0],height);
	g_isosurface_f_3D(x0,x1,y0,y1,z0,z1,number_z,number_y,number_x,&u[0][0][0],height);
}
//#undef FAST_CODE
