/*************************************************************************
	> File Name: Sys_s_work.hpp
	> Author: 陶超
	> Mail: taochao1997@qq.com 
	> Created Time: Sun 14 Jul 2019 11:39:43 PM PDT
 ************************************************************************/
#pragma once
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
#include<assert.h>
using namespace std;

//账号
atomic_int id(0);
//加载账号
void load()
{

	FILE* fp = fopen("Sys.config","r+b");
	assert(fp);
	int* p = new int();
	fread(p,4,1,fp);
	id = *p;
	fclose(fp);
}
//更新账号到文件中
void updata(int* ptr = NULL)
{
	FILE* fp =	fopen("Sys.config","w");
  fwrite(&id,4,1,fp);
  if(ptr)
    fwrite(&ptr,4,1,fp);
	fclose(fp);
}


//构造错误信息
void send_error(int sock,Json::Value& resp,int code,string error)
{
	resp["code"] = code;
	resp["reason"] = error;
	send(sock,resp.toStyledString().c_str(),strlen(resp.toStyledString().c_str()),0);
}



//抽象类
class fun 
{
	public:
		fun(){}
		virtual void method(int sock,Json::Value& val,mysql* data_base)=0;
};

//1.注册功能的类
class Register:public fun 
{
	public:
		Register(){}
		virtual void method(int sock,Json::Value& val,mysql* data_base);
};
//2.登录功能的类
class Login:public fun 
{
  public:
    virtual void method(int sock,Json::Value& val,mysql* data_base);
};


//1.注册
void Register::method(int sock,Json::Value& val,mysql* data_base)
{
	cout<<"send to the ID"<<endl;
	Json::Value res_id;
	Json::Value resp;
	const int ID = id; 
	res_id["id"] = ID;
	if(send(sock,res_id.toStyledString().c_str(),strlen(res_id.toStyledString().c_str()),0)<0)
	{
		send_error(sock,resp,0,"parse User_ID & Passwd error ");
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
			send_error(sock,resp,0,"user_id & passwd parse error!");
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
	send_error(sock,resp,0,"unknown error!");
}

//2.登录
void Login::method(int sock,Json::Value& val,mysql* data_base)
{
  //1.先查询是否存在账户
  //组织查询语句
  string sql = "select id, passwd from root_info where id = ";
  sql += val["user_id"].toStyledString().c_str();
  //Json回复
  Json::Value resp;
  int rows = -1;
  //检查账号是否存在
  bool flag=data_base->select(sql,&rows);
  if(flag)
  {
    if(rows == 0)
    {
      send_error(sock,resp,0,"the account not exist!");
      return;
    }
    else 
    {
      //账号存在，检查密码
      sql += " and passwd = ";
      sql += val["passwd"].toStyledString().c_str();
      if(data_base->select(sql,&rows))
      {
        if(rows == 0)
        {
          send_error(sock,resp,0,"passwd error!");
          return;
        }
        else //账号密码正确
        {
          send_error(sock,resp,1,"");
          return;
        }

      }   
    }
  }
  send_error(sock,resp,0,"unknown error!");
}
