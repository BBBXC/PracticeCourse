#pragma once
#include<iostream>
#include<fstream>
#include <iomanip>
#include<Eigen/Dense>
#include<Eigen/Sparse>
#include<vector>

#define CTRL 0;
#define UNKNOWN 1;
#define PI 3.1415926;

using namespace std;

struct mypair   //ƥ���
{
	int left, right;
};

struct resPoint   //�����
{
	int type;
	int num;
	double X, Y, Z;
};

struct phoPoint   //���
{
	int num;
	double x, y;
};

struct Point
{
	int type;  //����
	int num;   //���
	double g_x, g_y, g_z; //��������
	double p_x, p_y;  //��Ƭ����
};

struct UnknowPT
{
	int type;
	int photo_num;    //��Ƭ��
	int num;
	double pxl_x, pxl_y;   //��������
	double pho_x, pho_y;   //��Ƭ����
};

struct ExterElement
{
	int photo_num=0;    //��Ƭ��
	double phi, omega, kappa, Xs, Ys, Zs;
};

struct InterElement
{
	double x, y, f;
	int m;   
};

struct RelativeElement
{
	double phi, omega, kappa, u, v;
};

struct AbsoluteElement
{
	double lambda, phi, omega, kappa, dx, dy, dz;
};

class PhotoGram
{
public:
	PhotoGram(char *path_int, char *path_gcp);
	void setInt(char *path);   //��ȡ�ڷ�λԪ��
	void setGCP(char *path);    //��ȡ������Ƶ�

	Eigen::MatrixXd adj(Eigen::MatrixXd A, Eigen::MatrixXd L);
	Eigen::MatrixXd calRotate(ExterElement temp);

	vector<resPoint> getGCP();
	InterElement getInt();
	

protected:
	InterElement int_element;
	vector<resPoint> gcp;	
};

class RSanction: public PhotoGram
{

public:
	RSanction(char *path_int, char *path_realgcp,char *path_photogcp);
	void init();      //������ֵ

	Eigen::MatrixXd diff(Point pt,ExterElement temp);    // ϵ������
	Eigen::MatrixXd resi(Point pt, ExterElement temp);    // �������
	
	void calRSanction(int times,int photo_num); 
	void show();

	ExterElement getExt();
	void setphoGCP(char *path);

private:
	ExterElement initial;
	ExterElement res;
	Eigen::MatrixXd rotate;
	vector<phoPoint> p_gcp;   //����Ƭ����
};

class FSanction :public RSanction
{
public:
	FSanction(char *path_int, char *path_rgcp,char*path_pgcp, char *path_ext ,int count,int type);
	void setUP(char *path,int photo_num,int time);    //��ȡ���ӵ� ��Ƭ�� ����
	
	void setExt(char *path);   //��ȡ�ⷽλԪ��
	double angleTrans(double a);
	

	void point_prj(int point_num);   //��ͶӰϵ����
	Eigen::MatrixXd diff(Point pt, ExterElement temp);
	Eigen::MatrixXd resi(Point pt, ExterElement temp);

	void calFSanction(int times,int point_num);
private:
	vector<vector<UnknowPT>> unknown_pt;
	vector<ExterElement> ext_element;
	int photo_count;
	int type;
	Eigen::MatrixXd rotate;

	resPoint init;
	resPoint res;
};

class IOrientation
{
public:
	IOrientation(char *path);
	Eigen::MatrixXd calculate();
private:
	int num;
	Eigen::MatrixXd params;
	Eigen::MatrixXd origin;
	Eigen::MatrixXd change;
};

class ROrientation:public PhotoGram
{
public:
	ROrientation(char *path_int, char *path_gcp,char *path_left,char*path_right);
	void setPoints(char *path, int type);
	
	void calculate(int times);
	vector<resPoint> translate();
	void show();

	Eigen::MatrixXd diff(UnknowPT l, UnknowPT r);
	Eigen::MatrixXd resi(UnknowPT l, UnknowPT r);

	resPoint point_prj(double x1,double y1,double z1,double x2,double y2,double z2);
	vector<mypair> match();
	
private:

	double Bx = 0.08;   //Bx����
	Eigen::MatrixXd loc;    //��Ƭ�ռ�ģ������
	vector<resPoint> respt;  //��Ƭͬ����ռ丨������
	vector<UnknowPT> left, right; 
	Eigen::MatrixXd rotate;   //��Ƭ��ת����
	RelativeElement res;      //��������Զ������
	vector<mypair> match_res;  // ����Ƭ��ƥ����
};

class AOrientation :public ROrientation
{
public:
	AOrientation(char *path_int, char *path_gcp,char *path_model, char *path_left, char*path_right);
	void getModel(char *path);
	
	void calculate(int times);
	vector<resPoint> translate();

	vector<resPoint> gravitilize(vector<resPoint> pts);
	Eigen::MatrixXd diff(resPoint pt);
	Eigen::MatrixXd resi(resPoint pt_model, resPoint pt_gravi);
	
private:
	resPoint gravity;   //����XYZ��Ա�����Ļ����
	vector<resPoint> model;
	AbsoluteElement res;
};

class Bundle: public PhotoGram
{
public:
	Bundle(char* path_int, char* path_gcp,char*path_init_ext, char* path_init_pt, int count);
	void setPt(char* path, int photo_num, int time);  //����������Ƭ�еĵ�
	void setExt(char* path);      //����������Ƭ���ⷽλԪ��
	void setPoints(char* path);    //���õ��������Ƭ��Ķ�Ӧ��ϵ
	
	void calculate(int times);    //������ƽ�����
	void show(char* path1,char *path2);    

	Eigen::MatrixXd adj(Eigen::MatrixXd A, Eigen::MatrixXd L);   //ϡ�������ⷽ��

	Eigen::MatrixXd diff(Point pt, ExterElement temp);
	Eigen::MatrixXd resi(Point pt, ExterElement temp);
	void init();
	vector<mypair> search(int num);    //Ѱ����Ƭ���������ƥ��

	double angleTrans(double a);   //WuCAPs��ʽת�� �����任
	double invTrans(double len);

private:
	vector<vector<UnknowPT>> pts;  
	int pt_num=0;    //������Ƭ����

	vector<ExterElement> element;   //���
	vector<resPoint> res;
	
	vector<resPoint> init_pt;
	vector<int> point;   //��˳�򡪡�-���
};