#include "glsc3d_private.h"

/*****************************行列************************************************/
G_VECTOR Rn(G_VECTOR u,G_VECTOR n,G_REAL theta)//uをn周りにtheta回転させる
{
    G_VECTOR ans = g_vector(0, 0, 0);
    
    G_REAL R[3][3] = {
        {
            n.x * n.x * (1 - cos(theta)) + cos(theta),
            n.x * n.y * (1 - cos(theta)) - n.z *  sin(theta),
            n.z * n.x * (1 - cos(theta)) + n.y * sin(theta)
        },
        {
            n.x * n.y * (1 - cos(theta)) + n.z *  sin(theta),
            n.y * n.y * (1 - cos(theta)) + cos(theta),
            n.y * n.z * (1 - cos(theta)) - n.x * sin(theta)
        },
        {
            n.z * n.x * (1 - cos(theta)) - n.y * sin(theta),
            n.y * n.z * (1 - cos(theta)) + n.x * sin(theta),
            n.z * n.z * (1 - cos(theta)) + cos(theta)
        }
    };
    
    ans.x = R[0][0] * u.x + R[0][1] * u.y + R[0][2] * u.z;
    ans.y = R[1][0] * u.x + R[1][1] * u.y + R[1][2] * u.z;
    ans.z = R[2][0] * u.x + R[2][1] * u.y + R[2][2] * u.z;
    return ans;
}
G_VECTOR Rx(G_VECTOR u,G_REAL theta)//uをx軸周りにtheta回転させる
{
    G_VECTOR ans = g_vector(0, 0, 0);
    
    G_REAL R[3][3] = {
        {1,     0,              0},
        {0,     cos(theta),     -sin(theta)},
        {0,     sin(theta),     cos(theta)}
    };
    ans.x = R[0][0] * u.x + R[0][1] * u.y + R[0][2] * u.z;
    ans.y = R[1][0] * u.x + R[1][1] * u.y + R[1][2] * u.z;
    ans.z = R[2][0] * u.x + R[2][1] * u.y + R[2][2] * u.z;
    return ans;
}
G_VECTOR Ry(G_VECTOR u,G_REAL theta)//uをy軸周りにtheta回転させる
{
    G_VECTOR ans = g_vector(0, 0, 0);
    
    G_REAL R[3][3] = {
        {cos(theta),    0,      -sin(theta)},
        {0,             1,      0},
        {sin(theta),   0,      cos(theta)}
    };
    ans.x = R[0][0] * u.x + R[0][1] * u.y + R[0][2] * u.z;
    ans.y = R[1][0] * u.x + R[1][1] * u.y + R[1][2] * u.z;
    ans.z = R[2][0] * u.x + R[2][1] * u.y + R[2][2] * u.z;
    return ans;
}
G_VECTOR Rz(G_VECTOR u,G_REAL theta)//uをz軸周りにtheta回転させる
{
    G_VECTOR ans = g_vector(0, 0, 0);
    
    G_REAL R[3][3] = {
        {cos(theta),    -sin(theta),    0},
        {sin(theta),    cos(theta),     0},
        {0,             0,              1}
    };
    ans.x = R[0][0] * u.x + R[0][1] * u.y + R[0][2] * u.z;
    ans.y = R[1][0] * u.x + R[1][1] * u.y + R[1][2] * u.z;
    ans.z = R[2][0] * u.x + R[2][1] * u.y + R[2][2] * u.z;
    return ans;
}
G_VECTOR Scaling3Ds(G_VECTOR u, G_VECTOR s)//uをx,y,z方向にそれぞれs.x,s.y,s.z倍する
{
    return g_vector(u.x * s.x, u.y * s.y, u.z * s.z);
}

G_VECTOR Rx2D(G_VECTOR u,G_REAL theta)//二次元平面上でuをtheta回転させる
{
    G_REAL R[2][2] = {
        { cos(theta),   -sin(theta)},
        { sin(theta),   cos(theta) }
    };
    return g_vector2(
			R[0][0] * u.x + R[0][1] * u.y, 
			R[1][0] * u.x + R[1][1] * u.y);
}
