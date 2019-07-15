/*************************************************************************
    > File Name: Sys_s_work.hpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Sun 14 Jul 2019 11:39:43 PM PDT
 ************************************************************************/
#include"mysql.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<atomic>
#include<sys/socket.h>
#include<json/json.h>
#include<string.h>
using namespace std;

//账号
atomic_int id(0);
//构造错误信息
void send_error(int sock,Json::Value& resp,string error)
{
  resp["code"] = 0;
  resp["reason"] = error;
  send(sock,resp.toStyledString().c_str(),strlen(resp.toStyledString().c_str()),0);
}


//抽象类
class fun 
{
  public:
    fun(){}
    virtual void method(int sock,mysql* data_base)=0;
};

//1.注册功能的类
class Register:public fun 
{
  public:
    Register(){}
    virtual void method(int sock,mysql* data_base);
};


//1.注册
void Register::method(int sock,mysql* data_base)
{
  cout<<"send to the ID"<<endl;
  Json::Value res_id;
  Json::Value resp;
  const int ID = id; 
  res_id["id"] = ID;
  if(send(sock,res_id.toStyledString().c_str(),strlen(res_id.toStyledString().c_str()),0)<0)
  {
    send_error(sock,resp,"parse User_ID & Passwd error ");
    return;
  }
  char buf[1024] = {0};
  int ret = recv(sock,buf,1023,0);
  if(ret>0)
  {
    Json::Value val;
    Json::Reader read;
    if(-1 == read.parse(buf,val))
    {
      cout<<"parse error!"<<endl;
      send_error(sock,resp,"user_id & passwd parse error!");
      return;
    }
    id++;
    if(val["type"] == "Register")
    {
      cout<<"create sql"<<endl;
      //2.组装sql语句
      string sql = "insert into root_info values(";
      sql += val["user_id"].toStyledString().c_str();
      sql += ",";
      sql += val["passwd"].toStyledString().c_str();
      sql += ")";
      cout<<"creat sql ok"<<endl;;
      if(data_base->insert(sql))
      {
        //3.构造返回结果
        cout<<"create resp"<<endl;
        int code = 1;
        resp["code"] = code;
        resp["reason"] = "";
        cout<<resp.toStyledString()<<endl;
        send(sock,resp.toStyledString().c_str(),strlen(resp.toStyledString().c_str()),0);
        return;
      }
    }
  }
  send_error(sock,resp,"unknown error!");
}

