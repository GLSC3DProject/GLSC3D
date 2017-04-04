/*******************************************************************************
//
//  g_isosurface.c & g_isosurface.h
//  Type0
//
//  Created by masakazu on 2014/05/20.
//  Copyright © 2014.5.20 Masakazu Akiyama All Rights Reserved.
//
*******************************************************************************/

#include "glsc3d_private.h"

#define WIRE	G_FALSE
#define FILL	G_TRUE
#define DIVIDE_LEVEL	0

struct MarchingCube
{
	G_VECTOR r[8 + 1];
	double   value[8 + 1];
};

struct MarchingTetrahedron
{
	G_VECTOR r[4 + 1];
	double   value[4 + 1];
};

struct MarchingEdge
{
	G_VECTOR r;
	int flag;
};

static MarchingEdge Judgeheight(G_VECTOR r0, G_VECTOR r1, double v0, double v1, double c)
{
	MarchingEdge ans;

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

//G_POSITION g_position3Ds(G_VECTOR u)
//{
//    G_POSITION r = g_position(u.x,u.y,u.z);
//    return r;
//}
#define g_position3Ds(u) u

static void do_isosurface(const MarchingCube *Cube, MarchingTetrahedron *Tetrahedron, MarchingEdge *Edge,
						  double height, int Table1[6][5], int Table2[7][3])
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
				/*
                if(g_dot(n,n1) > 0) r1 = g_position3Ds(Edge[EdgeMember[1]].r), r2 = g_position3Ds(Edge[EdgeMember[2]].r);
                else r1 = g_position3Ds(Edge[EdgeMember[2]].r), r2 = g_position3Ds(Edge[EdgeMember[1]].r);
				 */
				r1 = g_position3Ds(Edge[EdgeMember[2]].r), r2 = g_position3Ds(Edge[EdgeMember[1]].r);
				g_triangle_3D_core_s(r0, r1, r2, DIVIDE_LEVEL, WIRE, FILL);
			}
			else
			{
				if(EdgeMember[0] == 1 && EdgeMember[1] == 2 && EdgeMember[2] == 5 && EdgeMember[3] == 6)
				{
					r0 = g_position3Ds(Edge[1].r);
					r2 = g_position3Ds(Edge[5].r);
					/*
                    if(g_dot(n,n1) > 0) r1 = g_position3Ds(Edge[2].r), r3 = g_position3Ds(Edge[6].r);
                    else r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
					 */
					r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
				}
				else if(EdgeMember[0] == 1 && EdgeMember[1] == 3 && EdgeMember[2] == 4 && EdgeMember[3] == 5)
				{
					r0 = g_position3Ds(Edge[1].r);
					r2 = g_position3Ds(Edge[5].r);
					/*
                    if(g_dot(n,n1) < 0) r1 = g_position3Ds(Edge[3].r), r3 = g_position3Ds(Edge[4].r);
                    else r1 = g_position3Ds(Edge[4].r),r3 = g_position3Ds(Edge[3].r);
					 */
					r1 = g_position3Ds(Edge[4].r),r3 = g_position3Ds(Edge[3].r);
				}
				else
				{
					r0 = g_position3Ds(Edge[3].r);
					r2 = g_position3Ds(Edge[4].r);

					/*
                    if(g_dot(n,n1) > 0) r1 = g_position3Ds(Edge[2].r),r3 = g_position3Ds(Edge[6].r);
                    else r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
					 */
					r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
				}
				g_triangle_3D_core_s(r0, r1, r2, DIVIDE_LEVEL, WIRE, FILL);
				g_triangle_3D_core_s(r0, r2, r3, DIVIDE_LEVEL, WIRE, FILL);
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
					do_isosurface(&Cube, &Tetrahedron, Edge, height,Table1r,Table2r);
				else
					do_isosurface(&Cube, &Tetrahedron, Edge, height,Table1l,Table2l);
			}
		}
	}
}
#undef u
//
///*******************************************************************************
// //
// //  g_isosurface.c & g_isosurface.h
// //  Type0
// //
// //  Created by masakazu on 2014/05/20.
// //  Copyright © 2014.5.20 Masakazu Akiyama All Rights Reserved.
// //
// *******************************************************************************/
//#include<stdio.h>
//#include<stdlib.h>
//
//#include "glsc3d_private.h"
//#define WIRE_OR_FILL  (G_FILL)
//#define DIVIDE_LEVEL    (0)
//struct MarchingCube
//{
//	G_VECTOR r[15 + 1];
//	G_VECTOR nabla[15 + 1];
//	double   value[15 + 1];
//};
//
//struct MarchingTetrahedron
//{
//	G_VECTOR r[28];
//	double   value[28];
//};
//
//struct MarchingEdge
//{
//	G_VECTOR r;
//	G_VECTOR nabla;
//	int flag;
//};
//
//static MarchingEdge Judgeheight(G_VECTOR r0,G_VECTOR r1,G_VECTOR nabla0, G_VECTOR nabla1, double v0, double v1,double c)
//{
//	MarchingEdge    ans;
//
//	ans.flag = 0;
//	if(v1 < c && c < v0)
//	{
//		ans.flag = -1;
//		ans.r = r1 + (c - v1) / (v0 - v1) * (r0 - r1);
//		ans.nabla = nabla1 + (c - v1) / (v0 - v1) * (nabla0 - nabla1);
//	}
//	if(v0 < c && c < v1)
//	{
//		ans.flag = 1;
//		ans.r = r1 + (v1 - c) / (v1 - v0) * (r0 - r1);
//		ans.nabla = nabla1 + (v1 - c) / (v1 - v0) * (nabla0 - nabla1);
//	}
//	return ans;
//}
//static const int CubleList[9][3] = {
//		{0, 0, 0},
//		{0, 0, 1},//1
//		{1, 0, 1},//2
//		{1, 1, 1},//3
//		{0, 1, 1},//4
//		{0, 0, 0},//5
//		{1, 0, 0},//6
//		{1, 1, 0},//7
//		{0, 1, 0} //8
//};
//
//static const int FaceList[28][4] = {
//		{9, 1, 11, 2},
//		{9, 2, 12, 3},
//		{9, 3, 13, 4},
//		{9, 4, 14, 1},
//		{10, 6, 11, 5},
//		{10, 5, 14, 8},
//		{10, 8, 13, 7},
//		{10, 7, 12, 6},
//		{9, 11, 12, 2},
//		{9, 12, 13, 3},
//		{9, 13, 14, 4},
//		{9, 14, 11, 1},
//		{10, 11, 14, 5},
//		{10, 14, 13, 8},
//		{10, 13, 12, 7},
//		{10, 12, 11, 6},
//		{11, 6, 12, 2},
//		{12, 7, 13, 3},
//		{13, 8, 14, 4},
//		{14, 5, 11, 1},
//		{15, 9, 11, 12},
//		{15, 9, 12, 13},
//		{15, 9, 13, 14},
//		{15, 9, 14, 11},
//		{15, 10, 11, 14},
//		{15, 10, 14, 13},
//		{15, 10, 13, 12},
//		{15, 10, 12, 11}
//};
//
//static const int MakingCubleList[7][10] = {
//		{ 9, 4, 1, 2, 3, 4, 0, 0, 0, 0},
//		{10, 4, 5, 6, 7, 8, 0, 0, 0, 0},
//		{11, 4, 1, 5, 6, 2, 0, 0, 0, 0},
//		{12, 4, 2, 6, 7, 3, 0, 0, 0, 0},
//		{13, 4, 3, 7, 8, 4, 0, 0, 0, 0},
//		{14, 4, 4, 8, 5, 1, 0, 0, 0, 0},
//		{15, 8, 1, 2, 3, 4, 5, 6, 7, 8}
//};
//
//G_VECTOR calc_nabla(int ii,int jj, int kk,int me, MarchingCube Cube,G_VECTOR d)
//{
//	return G_VECTOR(
//			(CubleList[ii][0] - CubleList[me][0]) * (Cube.r[ii].x - Cube.r[me].x) / d.x,
//			(CubleList[jj][1] - CubleList[me][1]) * (Cube.r[jj].y - Cube.r[me].y) / d.y,
//			(CubleList[kk][2] - CubleList[me][2]) * (Cube.r[kk].z - Cube.r[me].z) / d.z
//	);
//}
//#define     u(i, j, k)              u[(k) * (number_y * number_x) + (j) * (number_x) + (i)]
//void g_isosurface_f_3D(double x0,double x1,
//					   double y0,double y1,
//					   double z0,double z1,
//					   int number_x,int number_y,int number_z,
//					   double *u,
//					   double height)
//{
//	MarchingCube Cube;
//	G_VECTOR L = G_VECTOR(x1 - x0,y1 - y0,z1 - z0);
//	G_VECTOR d = G_VECTOR(L.x / (number_x - 1),L.y / (number_y - 1),L.z / (number_z - 1));
//	G_VECTOR r;
//	int i,j,k,l,m;
//	for(k = 0;k < number_z - 1;k ++)
//	{
//		r.z = (k + 0.5) * d.z - L.z / 2;
//		for (j = 0; j < number_y - 1; j ++)
//		{
//			r.y = (j + 0.5) * d.y - L.y / 2;
//			for(i = 0; i < number_x - 1; i ++)
//			{
//				r.x = (i + 0.5) * d.x - L.x / 2;
//				//Making Cube
//				for(l = 0;l < 8;l ++)
//				{
//					Cube.value[l + 1] =
//							u(i + CubleList[l + 1][0],
//							  j + CubleList[l + 1][1],
//							  k + CubleList[l + 1][2]);
//					Cube.r[l + 1] = G_VECTOR(r.x + (2 * CubleList[l + 1][0] - 1) * d.x / 2,
//											 r.y + (2 * CubleList[l + 1][1] - 1) * d.y / 2,
//											 r.z + (2 * CubleList[l + 1][2] - 1) * d.z / 2);
//				}
//				int ii,jj,kk,me;
//				ii = 2;jj = 4;kk = 5;me = 1;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//				ii = 1;jj = 3;kk = 6;me = 2;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//				ii = 4;jj = 2;kk = 7;me = 3;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//				ii = 3;jj = 1;kk = 8;me = 4;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//				ii = 6;jj = 8;kk = 1;me = 5;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//				ii = 5;jj = 7;kk = 2;me = 6;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//				ii = 8;jj = 6;kk = 3;me = 7;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//				ii = 7;jj = 5;kk = 4;me = 8;
//				Cube.nabla[me] = calc_nabla(ii, jj, kk, me, Cube, d);
//
//				for(l = 0;l < 7;l ++)
//				{
//					Cube.value[MakingCubleList[l][0]] = 0;
//					Cube.r[MakingCubleList[l][0]] = G_VECTOR(0,0,0);
//					Cube.nabla[MakingCubleList[l][0]] = G_VECTOR(0,0,0);
//
//					for(m = 0;m < MakingCubleList[l][1];m ++)
//					{
//						Cube.value[MakingCubleList[l][0]] += Cube.value[MakingCubleList[l][m + 2]] / MakingCubleList[l][1];
//						Cube.r[MakingCubleList[l][0]] += Cube.r[MakingCubleList[l][m + 2]] / MakingCubleList[l][1];
//						Cube.nabla[MakingCubleList[l][0]] += Cube.nabla[MakingCubleList[l][m + 2]] / MakingCubleList[l][1];
//					}
//				}
//
//				for(m = 0;m < 28;m ++)
//				{
//					/*
//					G_VECTOR r[4];
//					double v[4];
//					G_VECTOR nabla[4];
//					MarchingEdge Edge[4];
//					for(int n = 0; n < 4; n ++)
//					{
//						r[n] = Cube.r[FaceList[m][n]];
//						v[n] = Cube.value[FaceList[m][n]];
//						nabla[n] = Cube.nabla[FaceList[m][n]];
//					}
//					G_VECTOR norm[4];
//					G_VECTOR p[4];
//					int EdgePair[4];
//					int SankakuShikaku = 0;
//					Edge[0] = Judgeheight(
//										  r[0],
//										  r[1],
//										  nabla[0],
//										  nabla[1],
//										  v[0],
//										  v[1], height);
//					Edge[1] = Judgeheight(
//										  r[2],
//										  r[3],
//										  nabla[2],
//										  nabla[3],
//										  v[2],
//										  v[3], height);
//
//					if((Edge[0].flag != 0) && (Edge[1].flag != 0))//4つあるはず
//					{
//						Edge[2] = Judgeheight(
//											  r[1],
//											  r[2],
//											  nabla[1],
//											  nabla[2],
//											  v[1],
//											  v[2], height);
//						if(Edge[2].flag != 0)
//						{
//							Edge[3] = Judgeheight(
//												  r[0],
//												  r[3],
//												  nabla[0],
//												  nabla[3],
//												  v[0],
//												  v[3], height);
//							g_triangle_3D_smooth(
//													  Edge[0].r,
//													  Edge[2].r,
//													  Edge[1].r,
//													  Edge[0].nabla,
//													  Edge[2].nabla,
//													  Edge[1].nabla,
//													  WIRE_OR_FILL);
//							g_triangle_3D_smooth(
//													  Edge[0].r,
//													  Edge[1].r,
//													  Edge[3].r,
//													  Edge[0].nabla,
//													  Edge[1].nabla,
//													  Edge[3].nabla,
//													  WIRE_OR_FILL);
//						}
//						Edge[2] = Judgeheight(
//											  r[0],
//											  r[2],
//											  nabla[0],
//											  nabla[2],
//											  v[0],
//											  v[2], height);
//						if(Edge[2].flag != 0)
//						{
//							Edge[3] = Judgeheight(
//												  r[1],
//												  r[3],
//												  nabla[1],
//												  nabla[3],
//												  v[1],
//												  v[3], height);
//							g_triangle_3D_smooth(
//													  Edge[0].r,
//													  Edge[2].r,
//													  Edge[1].r,
//													  Edge[0].nabla,
//													  Edge[2].nabla,
//													  Edge[1].nabla,
//													  WIRE_OR_FILL);
//							g_triangle_3D_smooth(
//													  Edge[0].r,
//													  Edge[1].r,
//													  Edge[3].r,
//													  Edge[0].nabla,
//													  Edge[1].nabla,
//													  Edge[3].nabla,
//													  WIRE_OR_FILL);
//						}
//					}
//					 */
//
//					G_VECTOR r[4],nabla[4];
//					double v[4];
//					MarchingEdge Edge[6];
//
//					for(int n = 0; n < 4; n ++)
//					{
//						r[n] = Cube.r[FaceList[m][n]];
//						v[n] = Cube.value[FaceList[m][n]];
//						nabla[n] = Cube.nabla[FaceList[m][n]];
//					}
//					Edge[0] = Judgeheight(r[0], r[1], nabla[0], nabla[1], v[0], v[1], height);
//					Edge[1] = Judgeheight(r[0], r[2], nabla[0], nabla[2], v[0], v[2], height);
//					Edge[2] = Judgeheight(r[0], r[3], nabla[0], nabla[3], v[0], v[3], height);
//					Edge[3] = Judgeheight(r[1], r[2], nabla[1], nabla[2], v[1], v[2], height);
//					Edge[4] = Judgeheight(r[2], r[3], nabla[2], nabla[3], v[2], v[3], height);
//					Edge[5] = Judgeheight(r[3], r[1], nabla[3], nabla[1], v[3], v[1], height);
//					int SankakuShikaku = 0;
//					int EdgeMember[4];
//					for(int n = 0; n < 6; n ++)
//					{
//						if(Edge[n].flag != 0)
//						{
//							EdgeMember[SankakuShikaku] = n;
//							SankakuShikaku ++;
//						}
//					}
//					if(SankakuShikaku != 0)
//					{
//						G_VECTOR r0,r1,r2,r3;
//						if(SankakuShikaku == 3)
//						{
//							r0 = Edge[EdgeMember[0]].r;
//							r1 = Edge[EdgeMember[1]].r;
//							r2 = Edge[EdgeMember[2]].r;
//							g_triangle_3D_smooth(r0,r1,r2,
//													  g_normalize(Edge[EdgeMember[0]].nabla),
//													  g_normalize(Edge[EdgeMember[1]].nabla),
//													  g_normalize(Edge[EdgeMember[2]].nabla),
//													  WIRE_OR_FILL);
//
//						}
//						if(SankakuShikaku == 4)
//						{
//							/*
//							if(EdgeMember[0] == 1 && EdgeMember[1] == 2 && EdgeMember[2] == 5 && EdgeMember[3] == 6)
//							{
//								r0 = g_position3Ds(Edge[1].r);
//								r2 = g_position3Ds(Edge[5].r);
//								r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
//							}
//							else if(EdgeMember[0] == 1 && EdgeMember[1] == 3 && EdgeMember[2] == 4 && EdgeMember[3] == 5)
//							{
//								r0 = g_position3Ds(Edge[1].r);
//								r2 = g_position3Ds(Edge[5].r);
//								r1 = g_position3Ds(Edge[4].r),r3 = g_position3Ds(Edge[3].r);
//							}
//							else
//							{
//								r0 = g_position3Ds(Edge[3].r);
//								r2 = g_position3Ds(Edge[4].r);
//								r1 = g_position3Ds(Edge[6].r),r3 = g_position3Ds(Edge[2].r);
//							}
//							g_triangle_3D_cores(r0,r1,r2,DIVIDE_LEVEL,WIRE_OR_FILL);
//							g_triangle_3D_cores(r0,r2,r3,DIVIDE_LEVEL,WIRE_OR_FILL);
//							*/
//							if(EdgeMember[0] == 0 && EdgeMember[1] == 1 && EdgeMember[2] == 4 && EdgeMember[3] == 5)
//							{
//								r0 = Edge[0].r;
//								r2 = Edge[4].r;
//								r1 = Edge[5].r;
//								r3 = Edge[1].r;
//							}
//							else if(EdgeMember[0] == 0 && EdgeMember[1] == 2 && EdgeMember[2] == 3 && EdgeMember[3] == 4)
//							{
//								r0 = Edge[0].r;
//								r2 = Edge[4].r;
//								r1 = Edge[3].r;
//								r3 = Edge[2].r;
//							}
//							else
//							{
//								r0 = Edge[2].r;
//								r2 = Edge[3].r;
//								r1 = Edge[5].r;
//								r3 = Edge[1].r;
//							}
//
//							g_triangle_3D_smooth(r0,r1,r2,
//													  g_normalize(Edge[EdgeMember[0]].nabla),
//													  g_normalize(Edge[EdgeMember[1]].nabla),
//													  g_normalize(Edge[EdgeMember[2]].nabla),
//													  WIRE_OR_FILL);
//
//							g_triangle_3D_smooth(r0,r2,r3,
//													  g_normalize(Edge[EdgeMember[0]].nabla),
//													  g_normalize(Edge[EdgeMember[2]].nabla),
//													  g_normalize(Edge[EdgeMember[3]].nabla),
//													  WIRE_OR_FILL);
//
//						}
//					}
//				}
//			}
//		}
//	}
//}
//#undef u
