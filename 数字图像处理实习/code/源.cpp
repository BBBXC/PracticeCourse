#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include "subplot.h"
#include "read_write.h"
#include "linear_trans.h"
#include "filter.h"
#include "geometry.h"
#include "HPfusion.h"
#include "HISfuse.h"

using namespace cv;
using namespace std;

void test1()
{
	//��һ����ҵ  ��raw��ʽ�ļ�ת����Ϊbmp��ʽ
	const char source_path[80] = "C:\\Users\\yyc\\Desktop\\test-imgs\\test-imgs\\��׼ͼ��1024-1024.raw";
	const char target_path[80] = "C:\\Users\\yyc\\Desktop\\��׼ͼ��1024-1024.bmp";

	////��BIL�ļ�Ϊ��   ��Ҫ�޸����еĺ궨��
	//const char source_path[80] = "C:\\Users\\yyc\\Desktop\\test-imgs\\test-imgs\\BIL_856cols_743rows_6bands_byte.raw";
	//const char target_path[80] = "C:\\Users\\yyc\\Desktop\\BIL.bmp";
	
	RawRead(source_path, target_path);
}

void test2(const char *path)
{
	


	double k = 1; double b = 100;   //�Ҷ����Ա任Ĭ��ֵ

	//�û�����
	/*cout << "Linear Transforamtion"<< "F(x)=kx+b" << endl;
	cout << "Please Enter the value of 'k' and 'b':";
	cin >> k >> b;*/

	LinearTrans(path, k, b);
}

void test3(const char *path)
{
	HPF(path);
	LPF(path);
}

void test4(const char *path)
{
	Mat img = imread(path);
	Translation(img, 200, 'p', 'l');
	//������С����۲���ת
	Mat immg = Scaling(img, 0.5, 0.5);
	Rotation(immg, 3.14 *5/ 6);

	////�û�����
	//char d1 = 'p'; char d2 = 'l';    //Ĭ�ϲ���
	//int length=200;
	//cout << "For Geometric Transformation" << endl;
	//cout << "Translation" << endl;
	//cout << "Please Enter Direction1(Parallel,Vertical)in 'p' or 'v' :";
	//cin >> d1;
	//if (d1 == 'p')
	//{
	//	cout << "Please Enter Direction2(Left,Right) in 'l' or 'r':";
	//}
	//else if(d2 == 'l')
	//{
	//	cout<< "Please Enter Direction2(Up,Down) in 'u' or 'd':";	
	//}
	//cin >> d2;
	//cout << "Please Enter the Length of Translation:";
	//cin >> length;
	//
	//Translation(img, length, d1, d2);

	//
	//double r1 = 0.5; double r2 = 0.5;
	//cout << endl<<"Scaling" << endl;
	//cout << "Please Enter Ratios for x & y:";
	//cin >> r1 >> r2;

	//Scaling(img, r1, r2);

	//
	//double alpha = pi / 4;
	//cout << endl << "Rotation" << endl;
	//cout << "Please Enter the Angle for Rotation:";
	//cin >> alpha;

	//Rotation(img, alpha);
}

void test5_chose(const char *path1, const char *path2)
{
	//Fuse(path1, path2);
}

int main()
{
	const char path[80] = "C:\\Users\\yyc\\Desktop\\��׼ͼ��1024-1024.bmp";
	//test1();
	//test2(path);
	//test3(path);
	//test4(path);
	
	const char path_gray[80] = "C:\\Users\\yyc\\Desktop\\gray.bmp";
	const char path_color[80] = "C:\\Users\\yyc\\Desktop\\01931.bmp";

	//test5_chose(path_gray, path_color);



	HISfuse(path_gray, path_color);

	return 0;
}