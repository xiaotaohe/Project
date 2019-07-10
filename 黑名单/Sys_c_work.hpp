/*************************************************************************
  > File Name: Sys_c_work.hpp
  > Author: 陶超
  > Mail: taochao1997@qq.com 
  > Created Time: Tue 09 Jul 2019 11:14:29 PM PDT
 ************************************************************************/
#pragma once 
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<json/json.h>

#include<string>
using namespace std;

/*
 *客户端主程序+客户端服务器主程序
 */


void put();
bool add_send(int sock,string add_string);
bool add(int sock,bool is_select = false,string selecter = "");//默认不是信息采集器的消息
//1.客户端主程序
void client_work(int sock)
{
  while(1)
  {
    int option = 0;
    put();
    cout<<"请选择管理员操作: "<<endl;
    cin>>option;
    switch(option)
    {
      case 1:
        add(sock);
        break;
      case 2:
        //del(sock);
        break;
      case 3:
       // change(sock);
        break;
      case 4:
        //check(sock);
        break;
      case 5:
        cout<<"客户端系统关闭"<<endl;
        exit(1);
        break;
      default:
        cout<<"选择有误！"<<endl;
        break;
    }
  }
}
//2.子服务器工作函数
void selecter_work(int sock)
{
  //从客户端读取数据，根据读取到的数据，来做处理
  char buf[1024] = {0};
  if(recv(sock,buf,1023,0)<0)
  {
    cout<<"Selecter: Select_work recv error!"<<endl;
    return;
  }
  else 
  {
    //1.解析Json: name + id + year + month + day
    Json::Value select_info;
    Json::Reader read;
    if(read.parse(buf,select_info) == -1)
    {
      cout<<"Selecter: Select_work parse error!"<<endl;
      return;
    }
    //2.查看在没在表中：1.在的话，处理 2.不在直接加入，并进入公园
  }

}


//客户端操作
void put()
{
  cout<<"#################管理员所有选项##################"<<endl;
  cout<<"#               1.增加                          #"<<endl;
  cout<<"#               2.删除                          #"<<endl;
  cout<<"#               3.修改                          #"<<endl;
  cout<<"#               4.查找                          #"<<endl;
  cout<<"#               5.关闭客户端系统                #"<<endl;
  cout<<"#################################################"<<endl;
}

bool add_send(int sock,string add_string)
{
      if(send(sock,add_string.c_str(),strlen(add_string.c_str()),0)<0)
        return false;
      else 
      {
        char buf[1024] = {0};
        if(recv(sock,buf,strlen(buf),0)<0)
          return false;
        else 
        {
          Json::Value response;
          Json::Reader read;
          //解析失败说明，没有加入成功
          if(read.parse(buf,response) == -1)
            return false;
          else if(response["code"] != 1)
            return false;
          else if(response["code"] == 1)
            return true;
        }
      }
}

bool add(int sock,bool is_select,string selecter)//默认不是信息采集器的消息
{
    //1.信息采集器的加入，直接加
    if(is_select)
    {
      if(add_send(sock,selecter))
        return true;
    }
    //主程序
    else
    {
      //1.录入要加入的信息，组织为json
      Json::Value add_info;
      add_info["type"] = "Add";
      string str = "";
      //录入形式：姓名(name)+身份证号(id)+年(year)+月(month)+日(day)
      cout<<"请输入姓名: "<<endl;
      cin>>str;
      add_info["name"] = str;

      cout<<"请输入身份证号: "<<endl;
      cin>>str;
      add_info["id"] = str;

      cout<<"请输入年: "<<endl;
      cin>>str;
      add_info["year"] = str;

      cout<<"请输入月: "<<endl;
      cin>>str;
      add_info["month"] = str;

      cout<<"请输入日: "<<endl;
      cin>>str;
      add_info["day"] = str;
      //2.发送
      if(add_send(sock,add_info.toStyledString().c_str()))
        return true;
    }
    return false;
}

//客户端子服务器操作
