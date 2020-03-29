#include"snake.hpp"
#include"time.h"
#include<conio.h>

int main()
{
	//1.初始化地图
	init();
	//3.运动过程,默认运动方向为右
	char direction = 77;
	display();
	while (1)
	{
		bool timeflag = true;
		int x, y;
		long start = clock();
		//判断是否到了自行运动的时间或者键盘有变化
		while ((timeflag = (clock() - start) < 1000) && !_kbhit()){}
		if (timeflag)
		{
			 _getch();
			 direction = _getch();
		}
		switch (direction)
		{
			//向上
			case 72: x = snake.front().x - 1, y = snake.front().y;
			break;
			//向下
			case 80: x = snake.front().x + 1, y = snake.front().y;
			break;
			//向左
			case 75: x = snake.front().x, y = snake.front().y - 1;
			break;
			//向右
			case 77: x = snake.front().x, y = snake.front().y + 1;
			break;
			default: cout << "\tGame Over!\n";
			return 0;
		}
		//1.判断是否撞墙或者头尾相连
		if (x == 0 || x == HIGHT - 1 || y == 0 || y == WID - 1)
		{
			display();
			cout << "Game Over!" << endl;
			return 0;
		}
		//2.判断是否吃到蛇身
		if (snakemap[x][y] == '*')
		{
			display();
			cout << "Game Over!" << endl;
			return 0;
		}
		//判断当前位置是否有食物
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