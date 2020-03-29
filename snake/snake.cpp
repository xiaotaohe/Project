//#pragma once
//#include<iostream>
//#include<windows.h>
//#include<list>
//#include<vector>
//#include<time.h>
//#define WID 42
//#define HIGHT 22
//using namespace std;
//
////图形
//class snakemap
//{
//public:
//	vector<vector<char>> _snakemap;
//	snakemap()
//	{
//		_snakemap.resize(HIGHT);
//		for (size_t i = 0; i < _snakemap.size(); i++)
//			_snakemap[i].resize(WID);
//	}
//};
////蛇的每一节
//class node
//{
//	public:
//		int x;
//		int y;
//		node(int x, int y)
//			:x(x),
//			 y(y)
//		{}
//};
////蛇的定义
//class snake
//{
//public: 
//	list<node> _snake;
//	snake()
//	{
//		_snake.push_front(node(HIGHT / 2 + 1, WID / 2 + 1));
//		_snake.push_front(node(HIGHT / 2 + 1, WID / 2 + 2));
//		_snake.push_front(node(HIGHT / 2 + 1, WID / 2 + 3));
//	}
//};
//
//snakemap _snakemap;
//snake _snake;
//void display()
//{
//	system("cls");
//	//初始化画面的边界
//	cout << _snakemap._snakemap.size() << endl;
//	cout << _snakemap._snakemap[0].size() << endl;
//	for (size_t i = 0; i < _snakemap._snakemap[0].size(); i++)
//	{
//		_snakemap._snakemap[0][i] = '-';
//		_snakemap._snakemap[HIGHT-1][i] = '-';
//	}
//	for (size_t i = 0; i < _snakemap._snakemap.size(); i++)
//	{
//		_snakemap._snakemap[i][0] = '|';
//		_snakemap._snakemap[i][_snakemap._snakemap[0].size() - 1] = '|';
//	}
//
//	for (list<node>::iterator it = _snake._snake.begin(); it != _snake._snake.end();it++)
//	{
//		if (it == _snake._snake.begin())
//			_snakemap._snakemap[it->x][it->y] = '#';
//		else
//			_snakemap._snakemap[it->x][it->y] = '*';
//	}
//	for (size_t i = 0;i< _snakemap._snakemap.size(); i++)
//	{
//		for (size_t j = 0; j < _snakemap._snakemap[0].size(); j++)
//			cout << _snakemap._snakemap[i][j];
//		cout << endl;
//	}
//}


