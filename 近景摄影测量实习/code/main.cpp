#include"DLT.h"
#include"RSanction.h"
#include<fstream>
#include<iostream>


#define CheckNumber 8 //�������
#define InitNumber 6   //���Ƶ����ֵ���� ��������
#define IterNumber 10  //DLT��������

using namespace std;


void GCP_Read(string path, unordered_map<int, GroundPt> &gcp);
void Photo_Read(string path, vector<PhotoPt> &pt);

int main()
{
	string pht_path = "../../../data/measure.txt";    //"C:/Users/yyc/Desktop/1.txt";  
	string pht_path2 = "../../../data/measure2.txt"; //"C:/Users/yyc/Desktop/2.txt";  //
	string gcp_path = "../../../data/gcp_left_hand2.txt";

	string left_outpath = "../../../results/residual_left.txt";   //������·�� measure1��ʵ����Ƭ 
	string right_outpath = "../../../results/residual_right.txt";
	string rsanc_outpath = "../../../results/residual_rsanction.txt";  //�󽻲в�

	unordered_map<int, GroundPt> gcp;
	vector<PhotoPt> pht;   //��Ƭ
	vector<PhotoPt> pht2;  //��Ƭ
	Photo_Read(pht_path, pht);
	Photo_Read(pht_path2, pht2);
	GCP_Read(gcp_path, gcp);

	DLT dlt(pht, gcp, CheckNumber, right_outpath);
	dlt.Init(InitNumber);
	dlt.Calculate(IterNumber);
	dlt.Check();
	dlt.Display();

	unordered_set<int> check_num = dlt.GetCheckNum();

	//DLT���������
	DLT dlt2(pht2, gcp, check_num, left_outpath);
	dlt2.Init(InitNumber);
	dlt2.Calculate(InitNumber);
	dlt.Check();
	dlt2.Display();
	//DLT�⾫��
	dlt.ExtCheck(dlt, dlt2);
	dlt.CalUnknown(dlt, dlt2);

	pair<InterElement, ExterElement> element_pair = dlt.GetElement();
	vector<double> distort = dlt.GetDistort();

	RSanction rs(pht, gcp, 0,rsanc_outpath);
	rs.init();
	//rs.init_from_DLT(element_pair.first, element_pair.second, distort);
	rs.calRSanction(IterNumber);
	rs.Display();


	return 0;
}

void Photo_Read(string path, vector<PhotoPt> &pt)
{
	FILE* fp;
	fopen_s(&fp, path.c_str(), "rt");

	PhotoPt temp_pt; //����ϵ����
	
	while (!feof(fp))
	{
		// x~ col  y~row
		fscanf_s(fp, "%d %lf %lf\n",&temp_pt.num,&temp_pt.x,&temp_pt.y);

		//ͼ������ת��ƽ������
		temp_pt.x -= COL / 2;
		temp_pt.y = ROW / 2 - temp_pt.y;

		//���ص�λת��Ϊ���׵�λ
		temp_pt.x *= pix;
		temp_pt.y *= pix;
		pt.push_back(temp_pt);
	}
	
	fclose(fp);
}

void GCP_Read(string path, unordered_map<int, GroundPt> &gcp)
{
	FILE* fp;
	fopen_s(&fp, path.c_str(), "rt");
	
	////�ӵ�BOM�����ַ�
	//for (int i = 0; i < 3; i++)
	//{
	//	fgetc(fp);
	//}

	int count;   //���Ƶ���
	int number;  //���Ƶ���
	int condition; //״̬
	GroundPt temp_pt; //����ϵ����

	fscanf_s(fp, "%d\n", &count);
	for (int i = 0; i < count; i++)
	{
		//��ֵ�����ϵ�Ͷ���
		fscanf_s(fp, "%d\t%lf\t%lf\t%lf\t%d\n", &number, &temp_pt.z, &temp_pt.x, &temp_pt.y, &condition);
		temp_pt.z = -temp_pt.z;
		if (condition)
		{
			////ת����Ϊ��λ
			//temp_pt.x /= 1000;
			//temp_pt.y /= 1000;
			//temp_pt.z /= 1000;

			gcp.insert(pair<int, GroundPt>(number, temp_pt));
		}
	}
	fclose(fp);
}