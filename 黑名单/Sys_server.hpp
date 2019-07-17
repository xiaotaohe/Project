/*************************************************************************
  > File Name: Sys_server.hpp
  > Author: 陶超
  > Mail: taochao1997@qq.com 
  > Created Time: Sun 14 Jul 2019 07:26:59 PM PDT
 ************************************************************************/
#pragma  once 
#include"tcp.hpp"
#include"mysql.hpp"
#include"Sys_s_work.hpp"
#include<event.h>
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<atomic>
#include<map>
#include<json/json.h>
#include<string.h>
using namespace std;


//事件
map<int,struct event*> event_map;

//打包event_base 和 server对象
typedef struct Ser_arg
{
  Server* ser;
  struct event_base* base;
  mysql* data_base;
}Ser_arg;

//主要工作函数的类
class work 
{
  public:
    static void on_client(int fd,short event, void* arg);
    static void on_listen(int fd,short event, void* arg);
    static void lib_event(Server* ser,mysql* data_base);
  private:
    static void fun_work(int sock,Json::Value& val,fun* ptr,mysql* data_base);
    static void shunt(Json::Value& val,int sock,mysql* data_base);

};

//分流函数
void work:: shunt(Json::Value& val,int sock,mysql* data_base)
{
  if(val["type"].asString() == "Register-ID"){
    Register reg;
    fun_work(sock,val,&reg,data_base);
  }
  else if(val["type"].asString() == "Login")
  {
    Login login;
    fun_work(sock,val,&login,data_base);
  }
}

//on_client回调函数
void work:: on_client(int fd,short event, void* arg)
{
  //解析arg参数
  cout<<"触发onclient"<<endl;
  Ser_arg ser_arg = *((Ser_arg*)arg);
  char buf[1024] = {0};
  if(recv(fd,buf,1023,0)<=0)
  {
    cout<<"on_clicent: recv error!"<<endl;
    event_free(event_map[fd]);
    event_map.erase(fd);
    close(fd);
    return;
  }
  //1.数据解析
  Json::Value val; 
  Json::Reader read;
  if(read.parse(buf,val) == -1)
  {
    cout<<"on_client: parse error!"<<endl;
    return;
  }
  //2.分流
  shunt(val,fd,ser_arg.data_base);
  updata();
}


//on_listen回调函数
void work:: on_listen(int fd,short event, void* arg)
{
  //解析参数arg
  cout<<"触发on_listen"<<endl;
  Ser_arg ser_arg = *((Ser_arg*)arg);
  //解析server对象
  //Server* ser = ser_arg.ser;
  //解析libevent_base
  //struct event_base* base = ser_arg.base;
  //1.创建客户端套接字
  int cli_sock = -1;
  //2.接受连接
  if(!(ser_arg.ser->Accept(&cli_sock)))
  {
    cout<<"Work: accept error!"<<endl;
    return;
  }
  //3.创建事件
  struct event* cli_event = event_new(ser_arg.base,cli_sock,EV_READ|EV_PERSIST,on_client,arg);
  if(cli_event == NULL)
  {
    cout<<"Work: cli_event error!"<<endl;
    return;
  }
  //5.加入事件到事件列表
  event_map.insert(make_pair(cli_sock,cli_event));
  event_add(cli_event,NULL);
}

//libevent
void work:: lib_event(Server* ser,mysql* data_base)
{
  //libevent
  load();
  //1.创建event_base,初始化
  struct event_base* lib_base = event_base_new();
  //2.创建事件
  Ser_arg ser_arg; 
  ser_arg.ser = ser;
  ser_arg.base = lib_base;
  ser_arg.data_base = data_base;
  struct event* listen_event = event_new(lib_base,ser->listen_sock,EV_READ|EV_PERSIST,on_listen,(void*)&ser_arg);
  event_add(listen_event,NULL);
  event_base_dispatch(lib_base);
}

//各个业务的总处理函数
void work:: fun_work(int sock,Json::Value& val,fun* ptr,mysql* data_base)
{
  ptr->method(sock,val,data_base);
}
