/*************************************************************************
    > File Name: Sys_client.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Sun 07 Jul 2019 02:44:55 AM PDT
 ************************************************************************/

#include"tcp.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<atomic>
#include<json/json.h>
using namespace std; 

atomic_int id;

class Sys_Client
{
public:
  Sys_Client()
  {
    if(!client.Connect("0.0.0.0",9090))
    {
      cout<<"Sys_client Connect error!"<<endl;
      exit(-1);
    }
  }
  void put();
  void run();//服务器主函数
  //1.登录
  bool login();
  //2.注册
  bool registe();
private:
  //服务器对象
  Client client;
};

void Sys_Client:: put()
{
  cout<<"#################欢迎进入客户端##################"<<endl;
  cout<<"#               1.login(登录)                   #"<<endl;
  cout<<"#               2.register(注册)                #"<<endl;
  cout<<"#################欢迎进入客户端##################"<<endl;
}

bool Sys_Client:: login()
{
  int user_id = 0;
  int passwd = 0;
  cout<<"账号: "<<endl;
  cin>>user_id;
  cout<<"密码: "<<endl;
  cin>>passwd;
  Json::Value val;
  val["type"] = "Login"
  val["user_id"] = user_id;
  val["passwd"] = passwd;
  send(client.client_sock,val.toStyledString().c_str(),strlen(val.toStyledString().c_str());
  char buf[1024];
  recv(client.client_sock,buf,size)
}

bool Sys_Client::registe()
{
  //设置操作次数不能超过三次
  int second = 0;
  while(second != 3)
  {
    int passwd = 0;
    int re_passwd = 0;
    cout<<"你的账号为: "<<id<<endl;
    cout<<"请输入密码(六位)："<<endl;
    cin>>passwd;
    cout<<"请输入确认密码: "<<endl;
    cin>>re_passwd;
    if(passwd == re_passwd)
      break;
    second++;
  }
  if(second == 3)
  {
    cout<<"重新操作次数过多，退出，请重新选择！"<<endl;
    return false;
  }
  id++;
  return true;
}
