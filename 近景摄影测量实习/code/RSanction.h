#pragma once

#include"struct.h"
#include<Eigen/Dense>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<iostream>
#include<fstream>
#include <iomanip>
  
#define PI 3.1415926
#define f0   29     //���� ���� ��Ϊ�����ֵ
#define Xs0 3719.423130         //����
#define Ys0 - 119.302023		//����
#define Zs0 - 1391.599527       //����
#define Phi0 6.5/180.0*PI
#define Omega0 -1.3/180.0*PI
#define Kappa0 3/180.0*PI

class RSanction
{
public:
	RSanction(vector<PhotoPt> pht, unordered_map<int, GroundPt> gcp, int check_num,string outpath);  
	~RSanction();

	void locTransform();    //���������ת�� û���� ���ڶ��ļ���ת��

	unordered_set<int> Random(int min, int max, int n);    //���ȡ����

	void init();    //�󷽽����ʼ��
	void init_from_DLT(InterElement int_element, ExterElement ext_element, vector<double>distort);    //����DLT��ʼ��
	Eigen::MatrixXd calRotate(ExterElement temp);
	Eigen::MatrixXd LeastSquare(Eigen::MatrixXd A, Eigen::MatrixXd L);

	Eigen::MatrixXd diff(PhotoPt pho_pt,GroundPt ground_pt);
	Eigen::MatrixXd resi(PhotoPt pho_pt, GroundPt ground_pt);

	void calRSanction(int times);  //�󷽽������
	void check(Eigen::MatrixXd A, Eigen::MatrixXd X, Eigen::MatrixXd L, int num);  //����
	void Display();

private:
	vector<GroundPt> ground_pts_ctrl;
	vector<GroundPt> ground_pts_check;
	vector<PhotoPt> pho_pts_ctrl;
	vector<PhotoPt> pho_pts_check;
	vector<PhotoPt> unknownpts;

	string out_dir;
	InterElement int_element;   //�ڷ�λԪ��
	ExterElement ext_element;   //�ⷽλԪ��
	vector<double> distort;     // �������
};