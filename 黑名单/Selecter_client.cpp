/*************************************************************************
    > File Name: selecter_client.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Wed 10 Jul 2019 01:36:31 AM PDT
 ************************************************************************/
#include"tcp.hpp"
#include"Sys_c_work.hpp"
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
 if(!client.Connect("0.0.0.0",9191))
   return -1;
 //2.死循环，录入数据
 while(1)
 {
   Json::Value selecter_info;
   get_select_info(selecter_info);
   cout<<"录入中...."<<endl;
   sleep(2);
   if(send(client.client_sock,selecter_info.toStyledString().c_str(),\
       strlen(selecter_info.toStyledString().c_str()),0)<0)
   {
     cout<<"录入失败！请重试！"<<endl;
     continue;
   }
   cout<<"录入完成！"<<endl;
 }
}

void get_select_info(Json::Value& selecter_info)
{
	cout<<"开始录入："<<endl;
  selecter_info["type"] = "Selecter";
  work::get_info(selecter_info);
}
