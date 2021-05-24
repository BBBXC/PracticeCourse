#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <string>

#define  L 0.5                //����ֲ���ܶȱ仯�ĳ�����

//���ڿ��ӻ���ʾ
#define VI_NDVI 0             //��һ��ֲ��ָ��
#define VI_SAVI 1             //��������ֲ��ָ��
#define WI_MNDWI 2            //�޸Ĺ�һ��ˮ��ָ��
#define WI_AWEI 3             //�Զ�ˮ����ȡָ��
#define LI_NDBI 4             //��һ�����ָ��
#define LI_IBI  5             //�����õ�ָ��


using namespace cv;
using namespace std;

Mat Normalize(Mat r1, Mat r2);      //��һ��ָ��
Mat SAVI(Mat r, Mat nr);      //��������ֲ��ָ��
Mat AWEI(Mat g, Mat nr, Mat s1, Mat s2); //�Զ�ˮ����ȡָ��
Mat IBI(Mat ndbi, Mat savi, Mat mndwi); //�����õ�ָ��

void Col_Display(Mat b, Mat g, Mat r);       //ȫ��ɫ
void Display(Mat VI, int type);        //α��ɫ���ӻ� ���ļ����
void Binary(Mat img,int type);                 //��ֵ���ӻ�  ���ļ����  

int main()
{
	Mat img_b, img_g, img_r, img_nr, img_mr, img_dr;     //����
	Mat img_NV, img_SV,img_MW,img_AW,img_NL,img_IL;      //ָ��

	//���·��
	string path0 = "../../../ʵϰ����/�ڶ����� ר��ָ��/";
	string path[7] = { "tm1.tif","tm2.tif","tm3.tif","tm4.tif","tm5.tif","tm6.tif","tm7.tif" };

	//������
	img_b = imread(path0+path[0], 0);
	//�̲���
	img_g = imread(path0 + path[1], 0);
	//�첨��
	img_r = imread(path0 + path[2], 0);
	//�����Ⲩ��
	img_nr = imread(path0 + path[3], 0);
    //�к��Ⲩ�� SWIR1
	img_mr = imread(path0 + path[4], 0);
	//Զ���Ⲩ�� SWIR2
	img_dr = imread(path0 + path[6], 0);



	//��һ��ֲ��ָ��
	img_NV = Normalize(img_r, img_nr);
	//Display(img_NV, VI_NDVI);
	Binary(img_NV, VI_NDVI);

	//��������ֲ��ָ��
	img_SV = SAVI(img_r, img_nr);
	//Display(img_SV, VI_SAVI);
	Binary(img_SV, VI_SAVI);

	//�޸Ĺ�һ��ˮ��ָ��
	img_MW = Normalize(img_dr, img_g);
	//Display(img_MW, WI_MNDWI);
	Binary(img_MW, WI_MNDWI);

	//�Զ�ˮ����ȡָ��
	img_AW = AWEI(img_g, img_nr, img_mr, img_dr);
	//Display(img_AW, WI_AWEI);
	Binary(img_AW, WI_AWEI);

	//��һ�����ָ��
	img_NL = Normalize(img_nr, img_mr);
	//Display(img_NL, LI_NDBI);
	Binary(img_NL, LI_NDBI);

	//�����õ�ָ��
	img_IL = IBI(img_NL, img_SV, img_MW);
	//Display(img_IL, LI_IBI);
	Binary(img_IL, LI_IBI);

	//���ɫ�ϳ�
	Col_Display(img_b, img_g, img_r);

	return 0;
}

Mat Normalize(Mat r1, Mat r2)     //��һ��  (r2-r1)/(r2+r1)
{
	Mat res1(r1.size(), CV_64FC1);   //����
	Mat res2(r1.size(), CV_64FC1);   //��ĸ

	subtract(r2, r1, res1);
	add(r2, r1, res2);

	Mat img(r1.size(), CV_64FC1);    //����õ���ָ�� ��double�洢

	res1.convertTo(res1, CV_64FC1);   //add��subtract�ᵼ��res�����Ͳ�Ϊdoule
	res2.convertTo(res2, CV_64FC1);   // �˴�����ǿ��ת��

	divide(res1, res2, img);

	return img;
}

Mat SAVI(Mat r, Mat nr)
{
	Mat res1(r.size(), CV_64FC1);   //����
	Mat res2(r.size(), CV_64FC1);   //��ĸ

	subtract(nr, r, res1);
	add(nr, r, res2);
	add(res2, L, res2);

	res1.convertTo(res1, CV_64FC1);   //add��subtract�ᵼ��res�����Ͳ�Ϊdoule
	res2.convertTo(res2, CV_64FC1);   // �˴�����ǿ��ת��

	Mat img(r.size(), CV_64FC1);    //����õ���ָ��

	divide(res1, res2, img);
	multiply(L + 1, img, img);

	return img;
}

Mat AWEI(Mat g, Mat nr, Mat s1, Mat s2)
{
	Mat res1(g.size(), CV_64FC1);   //����
	Mat res2(g.size(), CV_64FC1);   //��ĸ
	Mat res3(g.size(), CV_64FC1);

	nr.convertTo(nr, CV_64FC1);
	s2.convertTo(s2, CV_64FC1);

	subtract(g, s1, res1);
	multiply(res1, 4, res1);
	res1.convertTo(res1, CV_64FC1);

	multiply(nr, 0.25, res2);
	multiply(s2, 2.75, res3);
	add(res2, res3, res2);

	Mat img(g.size(), CV_64FC1);

	divide(res1, res2, img);

	return img;
}

Mat IBI(Mat ndbi, Mat savi, Mat mndwi)
{
	Mat res1(ndbi.size(), CV_64FC1);

	add(savi, mndwi, res1);
	divide(res1, 2.0, res1);

	Mat img = Normalize(res1, ndbi);

	return img;
}

void Display(Mat Index, int type)
{

	Mat out(Index.size(), CV_8UC3);

	if (type == 0 || type == 1)          //ֲ��
	{
		for (int i = 0; i < Index.rows; i++)
			for (int j = 0; j < Index.cols; j++)
			{
				if (Index.at<double>(i, j) > 0)
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);   //�����ɫ
					out.at<Vec3b>(i, j)[2] = 0;
				}
				else
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}
			}
	}
	else if (type == 2 || type == 3)      //ˮ��
	{
		for (int i = 0; i < Index.rows; i++)
			for (int j = 0; j < Index.cols; j++)
			{
				if (Index.at<double>(i, j) > 0)
				{
					out.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);  //�����ɫ
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}
				else
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}
			}
	}
	else if (type == 4 || type == 5)      //����
	{
		for (int i = 0; i < Index.rows; i++)
			for (int j = 0; j < Index.cols; j++)
			{
				if (Index.at<double>(i, j) > 0)
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);  //��ӻ�ɫ
					out.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);
				}
				else
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}
			}
	}
	
	if (type == 0)    //��һ��ֲ��ָ��
	{
		imshow("��һ��ֲ��ָ��", out);
		waitKey(600);
		//imwrite("��һ��ֲ��ָ��.jpg", out);
	}
	else if (type == 1)   //��������ֲ��ָ��
	{
		imshow("��������ֲ��ָ��", out);
		waitKey(600);
		//imwrite("��������ֲ��ָ��.jpg", out);
	}      
	else if (type == 2)  //�޸Ĺ�һ��ˮ��ָ��
	{
		imshow("�޸Ĺ�һ��ˮ��ָ��", out);
		waitKey(600);
		//imwrite("�޸Ĺ�һ��ˮ��ָ��.jpg", out);
	}
	else if (type == 3)  //�Զ�ˮ����ȡָ��
	{
		imshow("�Զ�ˮ����ȡָ��", out);
		waitKey(600);
		//imwrite("�Զ�ˮ����ȡָ��.jpg", out);
	}
	else if (type == 4)  //��һ�����ָ��
	{
		imshow("��һ�����ָ��", out);
		waitKey(600);
		//imwrite("��һ�����ָ��.jpg", out);
	}
	else if (type == 5)  //�����õ�ָ��
	{
		imshow("�����õ�ָ��", out);
		waitKey(0);
		//imwrite("�����õ�ָ��.jpg", out);
	}


}

void Col_Display(Mat b, Mat g, Mat r)
{
	Mat img(b.size(), CV_8UC3);

	/*for(int i=0;i<b.rows;i++)
		for (int j = 0; j < b.cols; j++)
		{
			img.at<Vec3b>(i, j)[0] = r.at<uchar>(i, j);
			img.at<Vec3b>(i, j)[1] = b.at<uchar>(i, j);
			img.at<Vec3b>(i, j)[2] = g.at<uchar>(i, j);
		}*/
	Mat channel[3] = { b,g,r };

	merge(channel, 3, img);

	imshow("���ɫ�ϳ�", img);
	waitKey(0);
}

void Binary(Mat img,int type)
{
	Mat out(img.size(),CV_8UC1);
	
	double temp = 0;
	double max = 0;
	double min = 255;

	//��������
	
	//�Ҷ����� ��0-255
	for(int i=0;i<img.rows;i++)
		for (int j = 0; j < img.cols; j++)
		{
			temp = img.at<double>(i, j);
			if (temp > max)
			{
				max = temp;
			}
			if (temp < min)
			{
				min = temp;
			}
		}
	double scale = 255 / (max - min);

	for(int i=0;i<img.rows;i++)
		for (int j = 0; j < img.cols; j++)
		{
			out.at<uchar>(i, j) = (unsigned char)((int)((img.at<double>(i, j) - min)*scale));
		}

	//��̬������ֵ
	int thre = 0;
	double f1 = 0; double f2 = 0;         //ǰ�󾰻Ҷ�Ƶ��
	double s_temp = 0; double s = 0;     //����
	double avg1 = 0; double avg2 = 0;    //ǰ�󾰷ֱ��ֵ

	for (int t = 0; t < 256; t++)          //��ÿһ����ֵ
	{
		f1 = 0;
		f2 = 0;
		avg1 = 0;
		avg2 = 0;

		for(int i=0;i<out.rows;i++)
			for (int j = 0; j < out.cols; j++)
			{
				int val = (int)(out.at<uchar>(i, j));
				if(val<t)
				{
					f1++;
					avg1 += val;
				}
				else
				{
					f2++;
					avg2 += val;
				}
			}
		avg1 /= f1;
		avg2 /= f2;
		s_temp = f1 * f2*pow((avg1 - avg2), 2)/pow((out.rows-out.cols),2);

		if (s_temp > s)    //�ҵ���̬�滮�����ֵ
		{
			thre = t;
			s = s_temp;
		}
	}

	Mat res(out.size(), out.type());

	//��ֵ��
	for(int i=0;i<res.rows;i++)
		for (int j = 0; j < res.cols; j++)
		{
			int val = (int)(out.at<uchar>(i, j));
			if (val > thre)
			{
				res.at<uchar>(i, j) = 255;
			}
			else
			{
				res.at<uchar>(i, j) = 0;
			}
		}


	if (type == 0)    //��һ��ֲ��ָ��
	{
		imshow("��һ��ֲ��ָ��", res);
		waitKey(600);
		//imwrite("��һ��ֲ��ָ��_binary.jpg", out);
	}
	else if (type == 1)   //��������ֲ��ָ��
	{
		imshow("��������ֲ��ָ��", res);
		waitKey(600);
		//imwrite("��������ֲ��ָ��_binary.jpg", out);
	}
	else if (type == 2)  //�޸Ĺ�һ��ˮ��ָ��
	{
		imshow("�޸Ĺ�һ��ˮ��ָ��", res);
		waitKey(600);
		//imwrite("�޸Ĺ�һ��ˮ��ָ��_binary.jpg", res);
	}
	else if (type == 3)  //�Զ�ˮ����ȡָ��
	{
		imshow("�Զ�ˮ����ȡָ��", res);
		waitKey(600);
		//imwrite("�Զ�ˮ����ȡָ��_binary.jpg", res);
	}
	else if (type == 4)  //��һ�����ָ��
	{
		imshow("��һ�����ָ��", res);
		waitKey(600);
		//imwrite("��һ�����ָ��_binary.jpg", res);
	}
	else if (type == 5)  //�����õ�ָ��
	{
		imshow("�����õ�ָ��", res);
		waitKey(0);
		//imwrite("�����õ�ָ��_binary.jpg", res);
	}
}