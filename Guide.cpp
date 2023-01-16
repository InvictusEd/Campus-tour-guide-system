#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#define N 15				//顶点数目值
#define M 22				//边数目值
#define TN 5				//类型数目值
#define VexType string	//顶点数据类型
#define EdgeType int		//边数据类型
#define INF 0x3f3f3f3f	//作为最大值
using namespace std;

//景点数据结构
typedef struct Spot
{
	int number;
	char name[20];
	char SpotInfo[50];
}Spot;

//图的数据结构
typedef struct Graph
{
	VexType V[N];		//顶点表
	EdgeType E[N][N];	//边表
	int vnum, ednum;		//顶点数、边数
}Graph;

//景点类型数据结构
typedef struct SpotType
{
	string typeName;
	Spot S[TN];
	int number;
}SpotType;

//Dijkstra算法全局变量
bool S[N];	//顶点集
int D[N];	//到各个顶点的最短路径
int Pr[N];	//记录前驱

//绘制地图
void drawMap()
{
	printf("1.东北门               |--------------------------------------------------------------------------------------------|\n");
	printf("2.大学生活动中心       |-------------------------------☆玉溪师范学院校园地点一览☆---------------------------------|\n");
	printf("3.实训大楼A            |                                                                                            |\n");
	printf("4.滋味苑               |                                                                           ●1              |\n");
	printf("5.龙马公寓             |                                                                      ‖     ‖             |\n");
	printf("6.后山                 |                                               ●5=====●4==========●2=======●3====       |\n");
	printf("7.学生宿舍F,G幢        |                                               ‖                   ‖              ‖      |\n");
	printf("8.学生宿舍K,L,M,N幢    |                                               ‖                   ●7=============‖      |\n");
	printf("9.主教学区             |                                               ●6                  ‖              ●8     |\n");
	printf("10.品味苑              |                                               ‖                   ‖              ‖      |\n");
	printf("11.艺术综合楼          |                        ●10===‖              ‖                   ‖           ‖         |\n");
	printf("12.图书馆              |      ●15               ‖      ‖============●9============================‖            |\n");
	printf("13.运动场              |      ‖======‖           ‖                  ‖=                          ‖              |\n");
	printf("14.东南门              |               ====‖        ‖               ‖ ‖===========●11==========                |\n");
	printf("15.传习馆              |                    ====●13===‖===========‖                                              |\n");
	printf("                       |                                              ‖                                            |\n");
	printf("                       |                                               ●12====                                     |\n");
	printf("                       |                                                      ‖====‖                              |\n");
	printf("                       |                                                            ●14                            |\n");
	printf("                       |============================================================================================|\n");
}

//初始化图的顶点表，邻接矩阵等
void InitGraph(Graph& G)
{
	//初始化边表
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			G.E[i][j] = INF;
		}
	}
	G.ednum = G.vnum = 0;	//初始化顶点数、边数
}

//插入点函数，改变顶点表
void InsertNode(Graph& G, VexType v)
{
	if (G.vnum < N)
	{
		G.V[G.vnum++] = v;
	}
}

//插入边函数，改变邻接矩阵
void InsertEdge(Graph& G, VexType v, VexType w, int weight)
{
	int p1, p2;
	p1 = p2 = -1;
	for (int i = 0; i < G.vnum; i++)
	{
		if (G.V[i] == v)p1 = i;
		if (G.V[i] == w)p2 = i;
	}
	if (p1 != -1 && p2 != -1)
	{
		G.E[p1][p2] = G.E[p2][p1] = weight;	//无向图邻接矩阵对称
		G.ednum++;
	}
}

void Dijkstra(Graph G, int v)
{
	//初始化
	int n = G.vnum;	//n为图的顶点个数
	for (int i = 0; i < n; i++)
	{
		S[i] = false;
		D[i] = G.E[v][i];
		if (D[i] < INF)
		{
			Pr[i] = v;	//v与i连接，v为前驱
		}
		else
		{
			Pr[i] = -1;
		}
	}
	S[v] = true;
	D[v] = 0;
	//初始化结束，求最短路径，并加入S集
	for (int i = 1; i < n; i++)
	{
		int min = INF;
		int temp;
		for (int w = 0; w < n; w++)
		{
			if (!S[w] && D[w] < min)		//某点temp未加入S集，且为当前最短路径
			{
				temp = w;
				min = D[w];
			}
		}
		S[temp] = true;
		//更新从源点出发至其余点的最短路径 通过temp
		for (int w = 0; w < n; w++)
		{
			if (!S[w] && D[temp] + G.E[temp][w] < D[w])
			{
				D[w] = D[temp] + G.E[temp][w];
				Pr[w] = temp;
			}
		}
	}
}
//输出最短路径
void Path(Graph G, int v)
{
	if (Pr[v] == -1)
	{
		return;
	}
	Path(G, Pr[v]);
	cout << G.V[Pr[v]] << "->";
}

//创建图功能实现函数
void CreatGraph(Graph& G)
{
	int vn, an;	//顶点数，边数
	vn = N;
	an = M;
	int count = 0;
	char str1[20], str2[20];
	string s1, s2;
	int temp = 0;
	FILE* fp1 = fopen("Number.txt", "r");
	FILE* fp2 = fopen("Distance.txt", "r");
	if (fp1 == NULL && fp2 == NULL)
	{
		printf("打开文件失败！\n");
		exit(0);
	}
	//从文件中读取景点编号
	for (int i = 0; i < N; i++)
	{
		count = fscanf(fp1, "%s", str1);
		if (count == -1)exit(0);
		s1 = str1;
		InsertNode(G, s1);
	}
	//从文件中读取所有边的权值
	for (int i = 0; i < M; i++)
	{
		count = fscanf(fp2, "%s %s %d", str1, str2, &temp);
		if (count == -1)exit(0);
		s1 = str1;
		s2 = str2;
		InsertEdge(G, s1, s2, temp);
	}
	fclose(fp1);
	fclose(fp2);
}

//调用最短路径-Dijkstra算法
void Shortest_Dijkstra(Graph& G)
{
	system("cls");
	drawMap();

	string vname;
	string vnamed;
	int v1, v2;
	char ch = '1';

	while (true)
	{
		v1 = -1;
		v2 = -1;
		cout << "请输入起点编号：";
		cin >> vname;
		for (int i = 0; i < G.vnum; i++) {
			if (G.V[i] == vname)
			{
				v1 = i;
			}
		}
		if (v1 == -1)
		{
			cout << "没有找到输入点！" << endl;
			system("pause");
			system("cls");
			drawMap();
			continue;
		}
		cout << "请输入终点编号：";
		cin >> vnamed;
		for (int i = 0; i < G.vnum; i++) {
			if (G.V[i] == vnamed)
			{
				v2 = i;
			}
		}
		if (v2 == -1)
		{
			cout << "没有找到终点！" << endl;
			system("pause");
			system("cls");
			drawMap();
			continue;
		}
		Dijkstra(G, v1);
		cout << "\n目标点" << "\t" << "最短路径值" << "\t" << "最短路径" << "\t" << endl;
		for (int i = 0; i < G.vnum; i++)
		{
			if (i != v1 && i == v2)
			{
				cout << " " << G.V[i] << "\t" << D[i] << "米" << "\t" << "\t";
				Path(G, i);
				cout << G.V[i] << endl;
			}
		}
		cout << "\n** 输入1继续查询" << " / " << "输入0返回菜单 **" << endl;
		cin >> ch;
		if (ch == '0')break;
		system("cls");
		drawMap();
	}
}

//初始化景点类型数据结构
void BuildingType(Spot spt[], SpotType stype[])
{
	stype[0].typeName = "教学楼";
	stype[1].typeName = "学生宿舍";
	stype[2].typeName = "食堂";
	stype[3].typeName = "课外活动点";
	stype[0].S[0] = spt[2];
	stype[0].S[1] = spt[8];
	stype[0].S[2] = spt[10];
	stype[0].number = 3;
	stype[1].S[0] = spt[4];
	stype[1].S[1] = spt[6];
	stype[1].S[2] = spt[7];
	stype[1].number = 3;
	stype[2].S[0] = spt[3];
	stype[2].S[1] = spt[9];
	stype[2].number = 2;
	stype[3].S[0] = spt[1];
	stype[3].S[1] = spt[5];
	stype[3].S[2] = spt[12];
	stype[3].S[3] = spt[14];
	stype[3].number = 4;
}

void Print_Type(SpotType stype)
{
	for (int i = 0; i < stype.number; i++)
	{
		cout << stype.S[i].number << "." << stype.S[i].name << endl;
	}
}

void printInfo(Spot spt[], int i)
{
	printf("\n%d.%s\n简介：%s\n", spt[i].number, spt[i].name, spt[i].SpotInfo);
}

//查询景点类型
void ShowType(Spot spt[], SpotType stype[])
{
	int select = 0;
	system("cls");
	drawMap();

	while (true)
	{
		cout << "                                                    |==============================|" << endl;
		cout << "                                                    |          1." << stype[0].typeName << "            |" << endl;
		cout << "                                                    |          2." << stype[1].typeName << "          |" << endl;
		cout << "                                                    |          3." << stype[2].typeName << "              |" << endl;
		cout << "                                                    |          4." << stype[3].typeName << "        |" << endl;
		cout << "                                                    |==============================|" << endl;
		cout << "请选择你需要您要了解的类型：";
		cin >> select;
		getchar();
		switch (select)
		{
		case 1:
			Print_Type(stype[select - 1]);
			break;
		case 2:
			Print_Type(stype[select - 1]);
			break;
		case 3:
			Print_Type(stype[select - 1]);
			break;
		case 4:
			Print_Type(stype[select - 1]);
			break;
		default:
			cout << "请输入有效选项！\n回车键继续..." << endl;
			getchar();
			system("cls");
			drawMap();
			continue;
		}
		cout << "\n您需要了解以上建筑信息吗？（输入1了解，输入0取消）：";
		int select2;
		int select3;
		while (true)
		{
			cin >> select2;
			if (select2 == 1)
			{
				while (true)
				{
					cout << "\n请输入你想要了解建筑的编号（输入0取消）：";
					cin >> select3;
					if (select == 1)
					{
						if (select3 == 3 || select3 == 9 || select3 == 11)
						{
							printInfo(spt, select3 - 1);
						}
						else if (select3 == 0)
						{
							break;
						}
						else
						{
							cout << "请输入正确的此类型建筑编号！" << endl;
						}
					}
					else if (select == 2)
					{
						if (select3 == 5 || select3 == 7 || select3 == 8)
						{
							printInfo(spt, select3 - 1);
						}
						else if (select3 == 0)
						{
							break;
						}
						else
						{
							cout << "请输入正确的此类型建筑编号！" << endl;
						}
					}
					if (select == 3)
					{
						if (select3 == 4 || select3 == 10)
						{
							printInfo(spt, select3 - 1);
						}
						else if (select3 == 0)
						{
							break;
						}
						else
						{
							cout << "请输入正确的此类型建筑编号！" << endl;
						}
					}
					if (select == 4)
					{
						if (select3 == 2 || select3 == 6 || select3 == 13 || select3 == 15)
						{
							printInfo(spt, select3 - 1);
						}
						else if (select3 == 0)
						{
							break;
						}
						else
						{
							cout << "请输入正确的此类型建筑编号！" << endl;
						}
					}
				}
			}
			else if (select2 == 0)
			{
				break;
			}
			else
			{
				cout << "请输入正确选项！" << endl;
				break;
			}
			if (select3 == 0) break;
		}
		cout << "\n☆输入1继续查询景点类型（输入0返回菜单）☆：";
		cin >> select;
		if (select == 0) break;
		getchar();
		system("cls");
		drawMap();
	}
}

void menu()
{
	printf("                                                     |==============================|\n");
	printf("                                                     |            功能菜单          |\n");
	printf("                                                     |                              |\n");
	printf("                                                     |        1.景点信息查询        |\n");
	printf("                                                     |        2.问路查询            |\n");
	printf("                                                     |        3.景点类型查询        |\n");
	printf("                                                     |        0.退出                |\n");
	printf("                                                     |==============================|\n");
	printf("请输入选择：");
}

void NameFile(Spot spt[])
{
	int count = 0;
	int i;
	FILE* fp = fopen("Name.txt", "r");

	if (fp == NULL) {
		printf("打开文件失败！\n");
		exit(0);
	}
	for (i = 0; i < N; i++) {
		spt[i].number = i + 1;
		count = fscanf(fp, "%s", spt[i].name);
		if (count == -1) exit(0);
		//printf("%s\n", spt[i].name);		测试代码
	}
	fclose(fp);
}

void InfoFile(Spot spt[])
{
	int count = 0;
	int i;
	FILE* fp = fopen("Info.txt", "r");

	if (fp == NULL) {
		printf("打开文件失败！\n");
		exit(0);
	}
	for (i = 0; i < N; i++) {
		count = fscanf(fp, "%s", spt[i].SpotInfo);
		if (count == -1)exit(0);
		//printf("%s\n", spt[i].SpotInfo);		测试代码
	}
	fclose(fp);
}

void showInfo(Spot spt[])
{
	int select = 0;

	system("cls");
	drawMap();

	while (true)
	{
		printf("请输入景点编号（输入0返回菜单）：");
		scanf("%d", &select);
		getchar();
		switch (select)
		{
		case 1:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 2:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 3:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 4:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 5:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 6:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 7:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 8:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 9:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 10:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 11:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 12:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 13:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 14:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 15:
			printInfo(spt, select - 1);
			printf("\n任意键继续");
			getchar();
			break;
		case 0:
			system("cls");
			break;
		default:
			break;
		}
		system("cls");
		drawMap();
		if (select == 0)break;
	}
}

int main()
{
	int select = 0;
	Spot spt[N];
	SpotType stype[N];
	Graph G;

	drawMap();
	InitGraph(G);
	NameFile(spt);
	InfoFile(spt);
	CreatGraph(G);

	while (true)
	{
		menu();
		scanf("%d", &select);

		switch (select)
		{
		case 1:
			showInfo(spt);
			break;
		case 2:
			Shortest_Dijkstra(G);
			system("cls");
			drawMap();
			break;
		case 3:
			BuildingType(spt, stype);
			ShowType(spt, stype);
			system("cls");
			drawMap();
			break;
		case 0:
			printf("欢迎下次使用\n");
			exit(0);
			break;
		default:
			system("cls");
			drawMap();
			break;
		}
	}
	menu();

	system("pause");

	return 0;
}