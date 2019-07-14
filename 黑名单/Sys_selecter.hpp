/*************************************************************************
    > File Name: Sys_selecter.hpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Tue 09 Jul 2019 09:51:56 PM PDT
 ************************************************************************/
#pragma once
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
#include<map>
#include<event.h>
using namespace std;


map<int,struct event*> event_map;
//将对端服务器 和 event_base绑一块传给on_listen 和 on_client 
typedef struct cli_arg
{
  int fd;
  struct event_base* base;
}Cli_arg;

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

//客户端套接字回调函数
void on_client(int fd,short event,void* arg)
{ 
  // cout<<"触发 on_client 回调"<<endl;
  //cout<<"on_client: cli_sock"<<fd<<endl;
  char buf[1024] = {'\0'};
  //解析出服务端套接字
  int Sys_sock = ((Cli_arg*)arg)->fd;
  if(recv(fd,buf,1023,0)>0)
  {
    //cout<<buf<<endl;
    //1.消息处理
    //2.转发
    //cout<<"Sys_sock: "<<Sys_sock<<endl;
    //if(send(Sys_sock,buf,strlen(buf),0)<0)
    //   cout<<"Sys_select: transpond error!"<<endl;
    //!!!!!!上述任务统一有 work中封装的selecter_work处理
    work::selecter_work(Sys_sock,buf);
  }
  else//关闭客户端
  {
    //cout<<"close :"<<fd<<endl;
    event_free(event_map[fd]);
    event_map.erase(fd);
    close(fd);
  }
}

//监听套接字回调函数
void on_listen(int fd,short event,void* arg)
{
  //解析 arg 
  Cli_arg cli_arg = *((Cli_arg*)arg);

  //1.客户端套接字
  //cout<<"触发 on_listen 回调"<<endl;
  int cli_sock = -1;
  //2.接受连接
  struct sockaddr_in cli_addr;
  socklen_t len = sizeof(cli_addr);
  cli_sock = accept(fd,(struct sockaddr*)&cli_addr,&len);
  if(cli_sock == -1)
  {
    cout<<"Sys_select: accept error!"<<endl;
    return;
  }
  //cout<<"cli_sock: "<<cli_sock<<endl;
  //4.创建事件
  struct event* cli_event = event_new(cli_arg.base,cli_sock,EV_READ|EV_PERSIST,on_client,arg);
  if(cli_event == NULL)
  {
    cout<<"Sys_select: cli_event error!"<<endl;
    return;
  }
  //5.加入事件到事件列表,触发后会自动删除
  event_map.insert(make_pair(cli_sock,cli_event));
  event_add(cli_event,NULL);
}


//1.客户端selecter服务器入口函数
void* selecter(void* arg)
{
  //1.启动客户端子服务器
  Selecter Sel; 
  cout<<"listen_sock: "<<Sel.server->listen_sock<<endl; 
  //2.对端服务器套接字
  //接收到的信息可能要发往该套接字
  int Sys_serv_sock = *((int*)arg);

  //libevent进行多路复用
  //1.初始化libevent
  struct event_base* lib_base = event_base_new();
  //2.创建事件
  Cli_arg cli_arg;
  cli_arg.fd = Sys_serv_sock;
  cli_arg.base = lib_base;
  struct event* listen_event = event_new(lib_base,Sel.server->listen_sock,EV_READ|EV_PERSIST,on_listen,&cli_arg); 
  //3.将事件加入到事件列表
  event_add(listen_event,NULL);
  //4.启动循环监听
  event_base_dispatch(lib_base);
}
