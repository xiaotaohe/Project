/*************************************************************************
  > File Name: Sys_server.cpp
  > Author: 陶超
  > Mail: taochao1997@qq.com 
  > Created Time: Mon 08 Jul 2019 10:24:45 PM PDT
 ************************************************************************/

#include"tcp.hpp"
#include"mysql.hpp"
#include"Sys_server.hpp"
#include"Sys_s_work.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<json/json.h>
#include<string.h>
#include<atomic>
#include<map>
#include<event.h>
using namespace std;


int main()
{
  //1.启动服务器
  Server server("0.0.0.0",9090);
  //server_flag tcp.hpp里自己设置的标志位，标识绑定失败
  if(server_flag == -1)
    return -1;
  //2.连接数据库
  mysql data_base;
  if(!data_base.connect ("root","1230","black_list"))
  {
    cout<<"connect data_bas error!"<<endl;
    exit(0);
  }
  //3.libevent
  work::lib_event(&server,&data_base); 
}
