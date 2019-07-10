/*************************************************************************
    > File Name: Sys_selecter.hpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Tue 09 Jul 2019 09:51:56 PM PDT
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
using namespace std;


class Selecter 
{
  public:
    //1.创建服务端
    Selecter()
      :server(new Server("0.0.0.0",9191))
    {}
    //2.释放服务器
    ~Selecter()
    {
      delete server;
    }
  public:
    Server* server;
};


//1.客户端selecter服务器入口函数
void* selecter(void* arg)
{
  //1.启动客户端子服务器
  Selecter Sel; 
  //2.对端服务器套接字
  int client_sock = *((int*)arg);
  //2.采用libenvent对套接字进行监听
  int select_client;
  Sel.server->Accept(&select_client);
  char buf[1024] = {0};
  while(1)
  {
    memset(buf,'\0',1024);
    if(recv(select_client,buf,1023,0)<0)
    {
      perror("Selecter: recv error!\n");
    }
    //1.查询 2.根据查询结果，判断是否需要转发
    else 
    {
      send(client_sock,buf,strlen(buf),0);
    }
  }
}
