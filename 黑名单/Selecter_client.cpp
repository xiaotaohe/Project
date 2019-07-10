/*************************************************************************
    > File Name: selecter_client.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Wed 10 Jul 2019 01:36:31 AM PDT
 ************************************************************************/
#include"tcp.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<json/json.h>

using namespace std;

void get_select_info(Json::Value& selecter_info);

int main()
{
  //1.创建selecter_client
 Client client;
 client.Connect("0.0.0.0",9191);
 //2.死循环，录入数据
 while(1)
 {
   Json::Value selecter_info;
   get_select_info(selecter_info);
   send(client.client_sock,selecter_info.toStyledString().c_str(),\
       strlen(selecter_info.toStyledString().c_str()),0);
 }
}

void get_select_info(Json::Value& selecter_info)
{
	cout<<"开始录入："<<endl;
	selecter_info["type"] = "Add";
	string str = "";
	//录入形式：姓名(name)+身份证号(id)+年(year)+月(month)+日(day)
	cout<<"1.请输入姓名: "<<endl;
	cin>>str;
	selecter_info["name"] = str;

	cout<<"2.请输入身份证号: "<<endl;
	cin>>str;
	selecter_info["id"] = str;

	cout<<"3.请输入年: "<<endl;
	cin>>str;
	selecter_info["year"] = str;

	cout<<"4.请输入月: "<<endl;
	cin>>str;
	selecter_info["month"] = str;

	cout<<"5.请输入日: "<<endl;
	cin>>str;
	selecter_info["day"] = str;
}
