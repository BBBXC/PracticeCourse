#pragma once

#include <opencv2/highgui.hpp>  
#include <opencv2/core.hpp>  

#include<vector>
#include<iostream>
#include<fstream>
#include<string>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/ShapeDrawable>
#include<osg/LineWidth>

struct PtLoc {
	double X;
	double Y;
	double Z;
	int in;//��������ıߺ�
	int col;
	int row;//���ڱߵ����
	int HK;//0Ϊ��ߣ�1Ϊ����
};


struct CLine {
	std::vector<PtLoc> pt;
	int shape = 0;//0Ϊ�����ߣ�1Ϊ������
};


class DEM
{
public:
	DEM(std::string path, std::string image_path);    //���캯�������ʼ��
	~DEM();

	std::vector<std::vector<double>> GetNearestHeight();//��Чֵ����
	std::vector<std::vector<double>> Interp();     //ƥ������Ӱ���DEM
	osg::Node* Create_DEM_lines(std::string img_path, std::string dem_path); //���ӻ��ȸ���
	osg::Node* Create_DEM(std::string img_path, std::string dem_path); //���ӻ�DEM

	std::vector<std::vector<double>> StateMatHk(int t);
	std::vector<std::vector<double>> StateMatVk(int t);

	void GetlinePt(std::vector<std::vector<double>>& Hk, std::vector<std::vector<double>>& Vk, double Z, int ni, int nj, CLine& line);
	void getline(std::vector<std::vector<double>> Hk, std::vector<std::vector<double>> Vk, double Z, std::vector<CLine>& Line);
	void savelines(std::string dir, std::vector<CLine> Line, int k);
	void nlines(std::string dir, double dz);



private:
	std::vector<std::vector<double>> height_mat;   //������
	std::vector<std::vector<double>> dense_height_mat; //��ֵ����ܼ�����
	std::vector<std::pair<int, int>> invalid_val; //DEM��Чֵ
	std::vector<std::vector<CLine>> AllLines;//�����е���

	int col;
	int row;
	double A;
	double X0, Y0;    //��ʼ��ʵ�ʵ�������      
	double dx, dy;    //��������ʵ�ʵ�����

	double max_z, min_z;

	double delz = 5;	//���õȸ��߼�� Ĭ��5��

	int image_col, image_row;
};
