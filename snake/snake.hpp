#include<iostream>
#include<Windows.h>
#include<list>
using namespace std;

#define HIGHT 22
#define WID 44

//����snakemap
char snakemap[HIGHT][WID] = { ' ' };
//����snake_node

typedef struct snake_node
{
	int x, y;
	snake_node(int x, int y)
		:x(x), y(y)
	{}
}Node;
void creat_food();
//����snake
list<Node> snake;

//��ʼ��snakemap��snake
void init()
{
	for (int i = 0; i < HIGHT; i++)
		snakemap[i][0] = snakemap[i][WID-1] = '|';
	for (int j = 0; j < WID; j++)
		snakemap[0][j] = snakemap[HIGHT - 1][j] = '-';
	//�涨����տ�ʼ����Ϊ3�������м俪ʼ
	snake.push_back(Node(HIGHT / 2, WID / 2 - 1));
	snake.push_back(Node(HIGHT / 2, WID / 2 - 2));
	snake.push_back(Node(HIGHT / 2, WID / 2 - 3));
	for (list<Node>::iterator it = snake.begin(); it != snake.end(); it++)
	{
		if (it == snake.begin())
			snakemap[it->x][it->y] = '#';
		else
			snakemap[it->x][it->y] = '*';
	}
	for (int i = 0; i < 3; i++)
		creat_food();
}

//��ӡ
void display()
{
	system("cls");
	for (int i = 0; i < HIGHT; i++)
	{
		for (int j = 0; j < WID; j++)
			cout << snakemap[i][j];
		cout << endl;
	}
}

//����ʳ��
void creat_food()
{
	int x, y;
	while (true)
	{
		x = rand() % HIGHT;
		y = rand() % WID;
		if (!snakemap[x][y])
		{
			snakemap[x][y] = '%';
			break;
		}
	}
}

/*
int main()
{
	init();
	display();
}
*/