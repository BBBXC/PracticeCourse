#pragma once
#include<Eigen/Dense>

#define ROW 2848   //ԭ��Ƭ��
#define COL 4272   //ԭ��Ƭ��
#define pix (5.1966*1e-3) //��Ԫ��С
using namespace std;

struct GroundPt
{
	double x, y, z;
};

struct PhotoPt
{
	int num;
	double x, y;
};

struct ExterElement
{
	Eigen::MatrixXd R;
	double Xs, Ys, Zs;
	double phi, omega, kappa;
};

struct InterElement
{
	double x0, y0, f; 
};