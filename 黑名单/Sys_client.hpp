/*************************************************************************
  > File Name: Sys_client.cpp
  > Author: 陶超
  > Mail: taochao1997@qq.com 
  > Created Time: Sun 07 Jul 2019 02:44:55 AM PDT
 ************************************************************************/

#include"tcp.hpp"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<atomic>
#include<json/json.h>
#include<string.h>
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



//约定：用户信息：type;user_id;passwd两个字段
//      服务器返回：code(返回类型：成功 1，失败 0);reason原因
bool Sys_Client:: login()
{
  bool flag = false;
  //服务端返回登录信息：
  //1.登录表示：登陆成功 1 登录失败 0
  //2.原因：
  do{
    int user_id = 0;
    int passwd = 0;
    cout<<"账号: "<<endl;
    cin>>user_id;
    cout<<"密码: "<<endl;
    cin>>passwd;
    Json::Value user;
    user["type"] = "Login";
    user["user_id"] = user_id;
    user["passwd"] = passwd;
    if((send(client.client_sock,user.toStyledString().c_str(),strlen(user.toStyledString().c_str()),0))<0)
    {
      cout<<"Login::send error!"<<endl;
      continue;
    }
    char buf[1024];
    if((recv(client.client_sock,buf,1023,0))<0)
    {
      cout<<"Login::recv error!"<<endl;
      continue;
    }
    //获取服务端对登录成功的反馈
    Json::Value response;
    Json::Reader read;
    if(-1 == read.parse(buf,response))
    {
      cerr<<"read fail;error:"<<errno<<endl;
      continue;
    }
    if(response["code"] != 1)
    {
      cout<<"login error "<<"reason: "<<response["reason"].asString()<<endl;
    }
    else 
      flag = true;
  }while(flag == false);
}

bool Sys_Client::registe()
{
  //设置操作次数不能超过三次
  int second = 0;
  int passwd = 0;
  int re_passwd = 0;
  while(second != 3)
  {
    cout<<"你的账号为: "<<id<<endl;
    cout<<"请输入密码(六位)："<<endl;
    cin>>passwd;
    cout<<"请输入确认密码: "<<endl;
    cin>>re_passwd;
    if(passwd == re_passwd)
      break;
    second++;
  }
  //1.三次设置密码错误，此时退出
  if(second == 3)
  {
    cout<<"重新操作次数过多，退出，请重新选择！"<<endl;
    return false;
  }
  //2.设置成功交给服务器
  else 
  {
    //1.组织并发送给服务器
    Json::Value user_val;
    int user_id = id;
    user_val["type"] = "Register";
    user_val["user_id"] = user_id;
    user_val["passwd"] = passwd;
    //当用户信息发送失败时，一直发送10次
    int second = 0;
    while((send(client.client_sock,user_val.toStyledString().c_str(),strlen(user_val.toStyledString().c_str()),0))<0)
    {
      second++;
      if(second == 10)
        break;
    }
    //2.判断是否将注册信息完整发送成功
    if(second>=10)//发送失败
    {
      cout<<"register error！"<<"信息发送失败，请稍后重试！"<<endl;
      return false;
    }
    else//发送成功
    {
        //获取服务器返回结果
      char buf[1024] = {'0'};
      if(recv(client.client_sock,buf,1023,0)<0)
      {
        cout<<"Registe: recv error!"<<endl;
        return false;
      }
      Json::Value response;
      Json::Reader read;
      if(-1 == read.parse(buf,response))
      {
        cout<<"Registe: pase error!"<<endl;
        return false;
      }
      if(response["code"] == 1)
        cout<<"Registe: registe success!"<<endl;
      else 
      {
        cout<<"Registe: registe error! reason: "<<response["reason"].asString()<<endl;
        return false;
      }
    }
  }
  id++;
  return true;
}

int main()
{}
