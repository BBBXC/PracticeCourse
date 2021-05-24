#pragma once
#include<Eigen/Dense>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<iostream>
#include<fstream>
#include <iomanip>
#include"struct.h"

class DLT
{
public:
	DLT(vector<PhotoPt> pht,unordered_map<int, GroundPt> gcp,int check_num,string out_path); //�������
	DLT(vector<PhotoPt> pht, unordered_map<int, GroundPt> gcp, unordered_set<int> seed,string out_path); //�����ų�ʼ��

	Eigen::MatrixXd LeastSquare(Eigen::MatrixXd A, Eigen::MatrixXd L);
	unordered_set<int> Random(int min, int max, int n);    //���ȡ����

	double Judge(vector<double> L);   //����ֵ����
	void Init(int num);       //L������ֵ  ѡ��ǰn�������
	void Calculate(int times);  //ֱ�����Ա任����

	pair<InterElement,ExterElement> GetElement();  //�������ⷽλԪ��
	void Display();               //���ӻ�
	vector<double> GetDistort();                   //���ػ������
	unordered_set<int> GetCheckNum();              //���ؼ�����
	vector<PhotoPt> GetCheckPhoPts();              //���ؼ���������
	vector<GroundPt> GetCheckGroundPts();          //���ؼ����﷽����
	vector<PhotoPt> GetUnknownPts();               //����δ֪��������
	vector<double> GetL();                         //����Lϵ��

	

	//�﷽�����!!!!
	vector<GroundPt> ForGround(vector<PhotoPt> pht_left, vector<PhotoPt> pht_right
		            , vector<double> left_L, vector<double> right_L,
				vector<double> left_distort, vector<double> right_distort,
			pair<InterElement, ExterElement> left_element, pair<InterElement, ExterElement> right_element);
	
	void ExtCheck(DLT left_photo, DLT right_photo); //�⾫�ȼ��
	vector<GroundPt> CalUnknown(DLT left_photo, DLT right_photo); //δ֪�����

	void Check();      //��ͶӰ��� ���ȼ��
	void PrecisionDisplay(Eigen::MatrixXd A, Eigen::MatrixXd X, Eigen::MatrixXd L, int num);   //�������
	~DLT();


private:
	vector<GroundPt> ground_pts_ctrl;
	vector<GroundPt> ground_pts_check;
	vector<PhotoPt> pho_pts_ctrl;
	vector<PhotoPt> pho_pts_check;
	vector<PhotoPt> unknownpts;
	string out_dir;
	vector<double> L;   //ֱ�����Ա任����
	vector<double> distort; // �������
};