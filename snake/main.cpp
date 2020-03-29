#include"snake.hpp"
#include"time.h"
#include<conio.h>

int main()
{
	//1.��ʼ����ͼ
	init();
	//3.�˶�����,Ĭ���˶�����Ϊ��
	char direction = 77;
	display();
	while (1)
	{
		bool timeflag = true;
		int x, y;
		long start = clock();
		//�ж��Ƿ��������˶���ʱ����߼����б仯
		while ((timeflag = (clock() - start) < 1000) && !_kbhit()){}
		if (timeflag)
		{
			 _getch();
			 direction = _getch();
		}
		switch (direction)
		{
			//����
			case 72: x = snake.front().x - 1, y = snake.front().y;
			break;
			//����
			case 80: x = snake.front().x + 1, y = snake.front().y;
			break;
			//����
			case 75: x = snake.front().x, y = snake.front().y - 1;
			break;
			//����
			case 77: x = snake.front().x, y = snake.front().y + 1;
			break;
			default: cout << "\tGame Over!\n";
			return 0;
		}
		//1.�ж��Ƿ�ײǽ����ͷβ����
		if (x == 0 || x == HIGHT - 1 || y == 0 || y == WID - 1)
		{
			display();
			cout << "Game Over!" << endl;
			return 0;
		}
		//2.�ж��Ƿ�Ե�����
		if (snakemap[x][y] == '*')
		{
			display();
			cout << "Game Over!" << endl;
			return 0;
		}
		//�жϵ�ǰλ���Ƿ���ʳ��
		if (snakemap[x][y] == '%')
		{
			snakemap[x][y] = '#';
			snakemap[snake.front().x][snake.front().y] = '*';
			snake.push_front(Node(x, y));
			creat_food();
		}
		else
		{
			snakemap[x][y] = '#';
			snakemap[snake.front().x][snake.front().y] = '*';
			snakemap[snake.back().x][snake.back().y] = ' ';
			snake.push_front(Node(x, y));
			snake.pop_back();
		}
		display();
	}
}