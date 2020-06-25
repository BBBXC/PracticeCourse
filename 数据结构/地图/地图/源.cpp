#include<stdio.h>
#include<iostream>
#include<string.h>
#include<limits>
#include<vector>
#include<string>

#include"Timer.h"

using namespace std;

#define CityNum 199    //��������
#define RouteNum 1975   //·������

struct City  //�������ڶ��㣩 ����
{ 
	double longitude, latitude;    //���еľ��� γ��
	char city[30], country[30];    //������ �������ڹ���
};

struct Route  //�������ڱߣ�  ·�� 
{
	char origin_city[30], destin_city[30];  //�������к�Ŀ�ĵس���
	string transport;       //��ͨ����
	double time, cost;
	string info;          //���������Ϣ��վ
};

struct Key          //�ؼ��������ṹ
{
	int key_num;
	char city_name[30];
};

struct Arc         //�߽ڵ�Ľṹ������ʱ��������������ȣ�
{
	double cost;    //����
	double time;    //����ʱ��
	string transport;
	string info;
};

struct Graph 
{
	int arcnum;       //�ߵĸ�����·��������
	int vexnum;       //�ڵ���������ҵĸ�����
	struct Arc **arcs;       //�ڽӾ���
	bool pass = false;    //Dijkistra �����ж�����
};

struct Way           //����Dijkistra�м�¼�����ĵ�
{
	vector <int> city;
	vector <double> cost;
	vector <string> info;
	vector <string> transport;
};

double distance_D[CityNum];      //��ʾDijkistra����
bool visit[CityNum] = { false };                    //�жϵ��Ƿ񱻷���

Way way[CityNum];                           //�洢·��
Graph g;
//�Ա๦�ܺ���//
//ƥ�������ؼ���
int LocateCity(char *city, Key *key,int num)
{
	for (int i = 0; i < num; i++)    //����KMP����
	{
		if (strcmp(key[i].city_name, city) == 0)   //�ҵ������򷵻س��йؼ���
			return key[i].key_num;
	
	}
}

char *Match(int key_num, Key *key)
{
	for(int i=0;i<CityNum;i++)
		if(key[i].key_num==key_num)
		return key[i].city_name;
}


//��Сֵ
double Min(double x, double y)
{
	return x <= y ?  x : y;
}


//��ȡ������Ϣ
void ReadCityFile(const char *source,City *city,Key *key)     
{
	FILE *fp = fopen(source, "rt");
	if (!fp)
	{
		printf("Error.CityFile Cannot be open./n");
		return;
	}
	
	char ch;         //�����ȡ�ַ������뵽city������
	int i = 0 , j = 0;       //������

	while (!feof(fp) && i<CityNum)
	{
		ch = fgetc(fp);
		for (; ch != ','; ch = fgetc(fp))   //��ȡ����
		{
			city[i].country[j] = ch;
			j++;
		}
		city[i].country[j] = '\0';    //�����ֹ��

		ch = fgetc(fp);
		j = 0;      //����������

		for (; ch != ','; ch = fgetc(fp))   //��ȡ����
		{
			city[i].city[j] = ch;
			j++;
		}
		city[i].city[j] = '\0';    //�����ֹ��

		fscanf(fp, "%lf", &city[i].latitude);
		ch = fgetc(fp);
		fscanf(fp, "%lf", &city[i].longitude);

		strcpy(key[i].city_name, city[i].city);   //�����йؼ���һһ��Ӧ
		key[i].key_num = i;                       //��һ�����б��Ϊ0 


		ch = fgetc(fp);    //��ȡ���з�
		i++;     //��ȡ��һ������
		j = 0;
	}
	
    fclose(fp);
}

//��ȡ·����Ϣ
void ReadRouteFile(const char *source, Route *route)  
{
	FILE *fp = fopen(source, "rt");
	if (!fp)
	{
		printf("Error.RouteFile cannot be open.\n");
		return;
	}

	int i = 0;    //������
	char *p;      //ָ������ָ���ȡ·���г�Ա��Ϣ
	char ch;          //�����ȡ�ַ������뵽city������
	while (!feof(fp) && i < RouteNum)
	{
		ch = fgetc(fp);
		for (p = route[i].origin_city; ch != ','; ch = fgetc(fp))     //��ȡ��������
		{
			*(p++) = ch;
		}
		*p = '\0';          //�����ֹ��

		ch = fgetc(fp);
		for (p = route[i].destin_city; ch != ','; ch = fgetc(fp))     //��ȡĿ�ĵس���
		{
			*(p++) = ch;
		}
		*p = '\0';          //�����ֹ��

		ch = fgetc(fp);
		for (; ch != ','; ch = fgetc(fp))     //��ȡ��ͨ����
		{
			route[i].transport += ch;
		}

		
		fscanf(fp, "%lf", &route[i].time);   //��ȡʱ��
		ch = fgetc(fp);           //��ȡ����
		fscanf(fp, "%lf", &route[i].cost);   //��ȡ�۸�
		ch = fgetc(fp);           //��ȡ����

		ch = fgetc(fp);
		while (ch != '\n')
		{
			route[i].info +=ch;
			ch = getc(fp);
		}

		i++;     //��ȡ��һ��·��
	}
    
	fclose(fp);
}

void CreateGraph(City *city, Route *route,Key *key,char *priority){
	g.arcnum = RouteNum;
	g.vexnum = CityNum;
	
	g.arcs = new struct Arc*[CityNum];
	for (int j = 0; j < g.vexnum; j++)
	{
		g.arcs[j] = new struct Arc[CityNum];
	}

	for(int i=0;i<g.vexnum;i++)
		for (int j = 0; j < g.vexnum; j++)
		{
			if (i == j)             //��ʼ����
			{
				g.arcs[i][j].cost = g.arcs[i][j].time = 0;    //��ʼ����ֵ
				g.arcs[i][j].transport="No need";
				g.arcs[i][j].info="Nothing";
			}
			else
			{
				g.arcs[i][j].cost = g.arcs[i][j].time = DBL_MAX;    //��ʼ�����ֵ
			}
		}
	int origin_num, dest_num;       //�ҵ�ʼ���ճ��ж�Ӧ�ַ��Ĺؼ���
	for (int i = 0; i < RouteNum; i++)
	{
		

		origin_num = LocateCity(route[i].origin_city, key, g.vexnum);
		dest_num = LocateCity(route[i].destin_city, key, g.vexnum);

		if (strcmp(priority, "TIME") == 0)   //����۸�����
		{
			if (g.arcs[origin_num][dest_num].cost != DBL_MAX          //ֻ���ж�cost��ǰһ�α����뼴��
				&& route[i].time< g.arcs[origin_num][dest_num].time)   //����������֮���ж���ѡ��
			   {
				g.arcs[origin_num][dest_num].cost = route[i].cost;
				g.arcs[origin_num][dest_num].time = route[i].time;
				g.arcs[origin_num][dest_num].transport = route[i].transport;
				g.arcs[origin_num][dest_num].info = route[i].info;
			   }			
				
			else if(g.arcs[origin_num][dest_num].cost = DBL_MAX){
				g.arcs[origin_num][dest_num].cost = route[i].cost;
				g.arcs[origin_num][dest_num].time = route[i].time;
				g.arcs[origin_num][dest_num].transport = route[i].transport;
				g.arcs[origin_num][dest_num].info = route[i].info;
			}
		}
		else
		{
			if (g.arcs[origin_num][dest_num].cost != DBL_MAX  
				&& route[i].cost < g.arcs[origin_num][dest_num].cost)    
			{
				g.arcs[origin_num][dest_num].cost = route[i].cost;
				g.arcs[origin_num][dest_num].time = route[i].time;
				g.arcs[origin_num][dest_num].transport = route[i].transport;
				g.arcs[origin_num][dest_num].info = route[i].info;
			}
			else if (g.arcs[origin_num][dest_num].cost = DBL_MAX) {
				g.arcs[origin_num][dest_num].cost = route[i].cost;
				g.arcs[origin_num][dest_num].time = route[i].time;
				g.arcs[origin_num][dest_num].transport = route[i].transport;
				g.arcs[origin_num][dest_num].info = route[i].info;
			}
		}
	 }

}


void Dijkistra(Graph &graph, int depart, int destination, char *priority)
{
	int start = depart;    //���������ʼ��

	for (int i = 0; i < graph.vexnum; i++)
	{
		distance_D[i] = DBL_MAX;
	}

	for (int i = 0; i < graph.vexnum; i++)
	{
		way[i].city.push_back(depart);
		way[i].info.push_back(graph.arcs[depart][i].info);
		way[i].transport.push_back(graph.arcs[depart][i].transport);
		if (strcmp(priority, "TIME") == 0)
			way[i].cost.push_back(graph.arcs[depart][i].time);
		else if (strcmp(priority, "PRICE") == 0)
			way[i].cost.push_back(graph.arcs[depart][i].cost);
	}

	//ʱ������
	if (strcmp(priority, "TIME") == 0)     //�ж�ʱ�����Ȼ��Ǿ�������
	{
		for (int i = 0; i < graph.vexnum; i++)          //����ͼ�۸���Ϣ ���¾�������
		{
			distance_D[i] = Min(distance_D[i], graph.arcs[start][i].time);
		}

		//Dijkistra ����
		for (int i = 0; i < graph.vexnum; i++)
		{
			double min = DBL_MAX;          //��¼���·��·����
			for (int j = 0; j < graph.vexnum; j++)
				if (visit[j] == false && distance_D[j] < min)
				{
					min = distance_D[j];
					start = j;            //��¼��̾����ֵ ��������
				}
			visit[start] = true;              //�õ��Ѿ�����


			for (int i = 0; i < graph.vexnum; i++)
			{
				if (visit[i] == false && distance_D[i] > distance_D[start] + graph.arcs[start][i].time)
				{

					way[i].city = way[start].city;
					way[i].city.push_back(start);

					way[i].cost = way[start].cost;
					way[i].cost.push_back(graph.arcs[start][i].time);

					way[i].info = way[start].info;
					way[i].info.push_back(graph.arcs[start][i].info);

					way[i].transport = way[start].transport;
					way[i].transport.push_back(graph.arcs[start][i].transport);

				}
				//��¼����ͨ��·��

				if (visit[i] == false) {
					distance_D[i] = Min(distance_D[i],
						distance_D[start] + graph.arcs[start][i].time);
				}
			}

		}
		way[destination].city.push_back(destination);
	}


	//�۸�����
	else if (strcmp(priority, "PRICE") == 0)
	{
		for (int i = 0; i < graph.vexnum; i++)          //����ͼ�۸���Ϣ ���¾�������
		{
			distance_D[i] = Min(distance_D[i], graph.arcs[start][i].cost);
		}

		//Dijkistra ����
		for (int i = 0; i < graph.vexnum; i++)
		{
			double min = DBL_MAX;          //��¼���·��·����
			for (int j = 0; j < graph.vexnum; j++)
				if (visit[j] == false && distance_D[j] < min)
				{
					min = distance_D[j];
					start = j;            //��¼��̾����ֵ ��������
				}
			visit[start] = true;              //�õ��Ѿ�����


			for (int i = 0; i < graph.vexnum; i++)
			{
				if (visit[i] == false && distance_D[i] > distance_D[start] + graph.arcs[start][i].cost)
				{

					way[i].city = way[start].city;
					way[i].city.push_back(start);

					way[i].cost = way[start].cost;
					way[i].cost.push_back(graph.arcs[start][i].cost);

					way[i].info = way[start].info;
					way[i].info.push_back(graph.arcs[start][i].info);

					way[i].transport = way[start].transport;
					way[i].transport.push_back(graph.arcs[start][i].transport);

				}
				//��¼����ͨ��·��

				if (visit[i] == false) {
					distance_D[i] = Min(distance_D[i],
						distance_D[start] + graph.arcs[start][i].cost);
				}
			}

		}
		way[destination].city.push_back(destination);
	}
}

//��Ļ��ʾ
void Display(int destination_num,Key *key,char *priority)
{
	int i =1;
	printf(";������Ϊ��");
	for (int j=0;j<way[destination_num].city.size();j++)
	{
		
		char *city = Match(way[destination_num].city[j], key);
		printf("%s ", city);
		if ( i<way[destination_num].city.size())
		{
			printf("---> ");
			i++;
		}
	}

	i = 1;        //��ʼ��
	if(strcmp(priority,"TIME")==0)  printf("\nʱ��Ϊ:");
	else if (strcmp(priority, "PRICE") == 0) printf("\n����Ϊ:");
	for (int j = 0; j < way[destination_num].cost.size(); j++)
	{
		cout << way[destination_num].cost[j];
		if (i < way[destination_num].cost.size())
		{
			printf("---> ");
			i++;
		}
	}

	i = 1;        //��ʼ��
	printf("\n��ͨ����Ϊ:");
	for (int j = 0;j < way[destination_num].transport.size();j++)
	{
		cout << way[destination_num].transport[j];
		if (i < way[destination_num].transport.size())
		{
			printf("---> ");
			i++;
		}
	}
	i = 1;
	printf("\n·����ϢΪ:");
	for (int j = 0; j < way[destination_num].info.size(); j++)
	{
		cout << way[destination_num].info[j]<<endl;
		if (i < way[destination_num].info.size())
		{
			printf("---> ");
			i++;
		}
	}



 }


void DFS(Route *route, int v, Key *key)
{
	visit[v] = true;
	char *city = Match(v,key);
	printf("%s\n", city);        //������ȱ������
	int origin_num, dest_num;
	int i = 0;
	for (; i < RouteNum; i++)        //����ҵ�һ��·�ߵ���ʼ���йؼ�ֵ��Ҫ��������ͬ
		                            //�������·�ߵ�Ŀ�ĵس���
	{
		if (!visit[v] && v == LocateCity(route[i].origin_city, key, CityNum))
		{
			origin_num = LocateCity(route[i].origin_city, key, g.vexnum);
			dest_num = LocateCity(route[i].destin_city, key, g.vexnum);
			DFS(route, dest_num, key);
			break;
		}
	}
}

void DFSTraverse(Route *route,Key *key)   //���
{
	for (int i = 0; i < g.vexnum; i++)
	{
		visit[i] = false;
	}
	for (int i = 0; i < g.vexnum; i++)
	{
		if (!visit[i]) DFS(route,i,key);    //��δ���ʵĶ������DFS
	}

}

//��ͼ��ʾ
void VisualDisplay(City *city,Route *route,char *departure, char *destination, const char* address_file_save,Key *key)
{
	FILE *fp = fopen(address_file_save, "wt");
	if (!fp)
	{
		printf("Error.File for VisualDisplay cannot be open.");
	    return;
     }
	int destination_num = LocateCity(destination, key, CityNum);
	int departure_num = LocateCity(departure, key, CityNum);

	for (int i=0;i+1< way[destination_num].city.size();i++)    //�ж϶���N'Djamena ��������ĳ���
	{
		char change_city[30];
		strcpy(change_city, Match(way[destination_num].city[i], key));
		for (int j = 0; j < strlen(change_city); j++)
		{
			if (change_city[j] == '\'')     //���С� ���ת������ƶ�����
			{
				char ch,trans;
				ch = change_city[j];
				change_city[j] = '\\';
				for (int x = j+1; x <= strlen(change_city) - j+1; x++)
				{
					trans= change_city[x];
					change_city[x] = ch;
					ch = trans;
				}
				strcpy(city[way[destination_num].city[i]].city, change_city);
			}
			
		}
	}


	//��ʼ�����ִ洢
	fprintf(fp, "<!DOCTYPE html><html><head><style type='text/css'>");   //%%��������%
	fprintf(fp, "body, html{width: 100%%;height: 100%%;margin:0;font-family:'΢���ź�';}");
	fprintf(fp, "#allmap{height:100%%;width:100%%;}#r-result{width:100%%;}");
	fprintf(fp, "</style><script type = 'text/javascript' src ='");
	fprintf(fp, "http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'>");
	fprintf(fp, "</script><title>Shortest path from %s to %s",departure,destination);   //��ͷ�ĳ���
	fprintf(fp, "</title></head><body><div id='allmap'>");
	fprintf(fp, "</div></div></body></html><script type='text/javascript'>");
	fprintf(fp, "var map = new BMap.Map('allmap');");
	fprintf(fp, "var point = new BMap.Point(0,0);map.centerAndZoom(point,2);");
	fprintf(fp, "map.enableScrollWheelZoom(true);var marker0 = new BMap.Marker(new BMap.Point(%lf,%lf));"
	                     , city[departure_num].longitude,city[departure_num].latitude);
	fprintf(fp, "map.addOverlay(marker0);\n");

	//�洢·������Ϣ
	//�����㵥���棨û��·������Ϣ)
	fprintf(fp, "var infoWindow0 = new BMap.InfoWindow");
	fprintf(fp, "(\"<p style = 'font-size:14px;'>country: %s<br/>city : %s</p>\");"
		                  ,city[departure_num].country,city[departure_num].city);
	fprintf(fp, "marker0.addEventListener(\"click\", function(){this.openInfoWindow(infoWindow0);}); ");
	fprintf(fp, "var marker1 = new BMap.Marker(new BMap.Point(%lf,%lf));"
	     		 ,city[way[destination_num].city[1]].longitude, city[way[destination_num].city[1]].latitude);
	fprintf(fp, "map.addOverlay(marker1);\n");
	
	for (int i = 1; i < way[destination_num].city.size(); i++)
	{
		fprintf(fp, "var infoWindow%d = new BMap.InfoWindow",i);
		fprintf(fp, "(\"<p style = 'font-size:14px;'>country: %s<br/>city : %s</p>\");"
			,city[way[destination_num].city[i]].country, city[way[destination_num].city[i]].city);
		fprintf(fp, "marker%d.addEventListener(\"click\", function(){this.openInfoWindow(infoWindow%d);}); ",i,i);
		fprintf(fp, "var contentString0%d ='",i);
		fprintf(fp, "%s,%s-->%s,%s", city[way[destination_num].city[i-1]].country, city[way[destination_num].city[i-1]].city
		                            ,city[way[destination_num].city[i]].country,city[way[destination_num].city[i]].city);
		
		//stringת��Ϊchar
		fprintf(fp, "(%s-", way[destination_num].transport[i - 1].c_str());
		int route_num;  //��¼����һ��·�������ڴ�ǰ����ѡ���ʱ��δ��way�д洢����ʱ��/�۸����Ϣ��
		for (route_num = 0; route_num < CityNum; route_num++)
		{
			if (way[destination_num].city[i - 1] == LocateCity(route[route_num].origin_city, key, CityNum)
				&& way[destination_num].city[i] == LocateCity(route[route_num].destin_city, key, CityNum))
				break;
		}
		fprintf(fp, "%lfhours-$%lf-", route[route_num].time, route[route_num].cost);

		fprintf(fp, "%s)';", route[route_num].info.c_str());

		fprintf(fp, "var path%d = new BMap.Polyline([new BMap.Point",i);
		fprintf(fp, "(%lf,%lf),new BMap.Point",
			city[way[destination_num].city[i-1]].longitude, city[way[destination_num].city[i-1]].latitude);
		fprintf(fp, "(%lf,%lf)],",
			city[way[destination_num].city[i]].longitude, city[way[destination_num].city[i]].latitude); 
		fprintf(fp, "{strokeColor:'#18a45b', strokeWeight:8, strokeOpacity:0.8});");
		fprintf(fp, "map.addOverlay(path%d);path%d.addEventListener", i, i);
		fprintf(fp, "(\"click\", function(){alert(contentString0%d);});", i);
		
		if (i + 1 != way[destination_num].city.size())
		{
			fprintf(fp, "var marker%d = new BMap.Marker(new BMap.Point", i);
			fprintf(fp, "(%lf,%lf));map.addOverlay(marker%d);\n"
				, city[way[destination_num].city[i]].longitude, city[way[destination_num].city[i]].latitude, i);

			fprintf(fp, "var infoWindow%d = new BMap.InfoWindow", i);
			fprintf(fp, "(\"<p style = 'font-size:14px;'>country: %s<br/>city : %s</p>\");"
				, city[way[destination_num].city[i]].country, city[way[destination_num].city[i]].city);
			fprintf(fp, "marker%d.addEventListener(\"click\", function(){this.openInfoWindow(infoWindow%d);}); ", i, i);
			fprintf(fp, "var marker%d = new BMap.Marker(new BMap.Point", i + 1);
			fprintf(fp, "(%lf,%lf));"
				, city[way[destination_num].city[i + 1]].longitude, city[way[destination_num].city[i + 1]].latitude);
			fprintf(fp, "map.addOverlay(marker%d);\n", i + 1);
		}
	}
    //��ֹ��Ҳ��Ҫ����д
	fprintf(fp, "</script>");

	fclose(fp);
}
int main()
{
	//����ʱ��//
	Timer timer;
	timer.Start();
	
	const char file_city[12] = "cities.csv";     //�ļ���ַ
	const char file_route[12] = "routes.csv";
	const char file_save[18] = "MyVisualGraph.htm";

	City my_city[CityNum];          //��ȡ���С�·�ߡ������ƥ��Ĺؼ�������
	Route my_route[RouteNum];
	Key my_key[CityNum];

	char departure[30]; int departure_num;
	char destination[30]; int destination_num;
	char priority[6];             //�û�ѡ��۸�/ʱ������

	//printf("please enter your departure place:\n");   //getline���ڶ�ȡ���пո�ĳ���
	//cin.getline(departure,30);
	//printf("please enter your source place:\n");
	//cin.getline(destination, 30);
	//printf("please choose your priority:(TIME or PRICE):\n");
	//scanf("%s", priority);

	strcpy(departure, "Beijing");
	strcpy(destination, "Berlin");
	strcpy(priority,"TIME");           //TIME OR PRICE

	ReadCityFile(file_city, my_city, my_key);
	ReadRouteFile(file_route, my_route);
	CreateGraph(my_city, my_route, my_key, priority);

	departure_num = LocateCity(departure, my_key,CityNum);
	destination_num = LocateCity(destination, my_key, CityNum);

	Dijkistra(g, departure_num, destination_num,priority);

	Display(destination_num, my_key,priority);
	
	//DFSTraverse(my_route,my_key);    //�������

	VisualDisplay(my_city,my_route, departure, destination, file_save,my_key);//��ͼ���ӻ�
	
	timer.Stop();
	printf("\nElapsed time is: <%4.2lf> ms\n", timer.ElapsedTime());

	return 0;
}