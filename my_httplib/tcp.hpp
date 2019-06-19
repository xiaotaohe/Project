/*************************************************************************
    > File Name: server.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Tue 18 Jun 2019 08:23:34 PM PDT
 ************************************************************************/

/*
 *本段代码目标：1.实现将tcp套接口的4个过程封装到一块
 */
#pragma once 
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
using namespace std;

//服务端
class Server
{
  public:
    //监听套接字
    Server(const string& ip,u_int16_t port)
    {
      //1.创建套接字
      listen_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(listen_sock<0)
      {
        cout<<"socket error!"<<endl;
        exit(-1);
      }
      //2.绑定地址信息
      sockaddr_in listen_addr;
      listen_addr.sin_family = AF_INET;
      listen_addr.sin_addr.s_addr = inet_addr(ip.c_str());
      listen_addr.sin_port = ntohs(port);
      socklen_t len = sizeof(listen_addr);
      int ret = bind(listen_sock,(struct sockaddr*)&listen_addr,len);
      if(ret < 0)
      {
        cout<<"bind error!"<<endl;
        exit(-1);
      }
      //3.将套接字置为监听状态
      ret = listen(listen_sock,5);
      if(ret<0)
      {
        cout<<"listen error!"<<endl;
        exit(-1);
      }
    }
    //封装accept
    bool Accept(int* client_sock)
    {
      sockaddr_in client_addr;
      socklen_t len = sizeof(client_addr);
      *client_sock = accept(listen_sock,(struct sockaddr*)&client_addr,&len);
      if(*client_sock<0)
      {
        cout<<"accept error!"<<endl;
        return false;
      }
      return true;
    }
    //关闭套接字
    bool Close()
    {
      close(listen_sock);
    }
  public:
    //listen为服务端套接字
    int listen_sock;
};


//客户端
class Client
{
  public:
    //1.创建套接字
    Client()
    {
      client_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(client_sock<0)
      {
        cout<<"socket error!"<<endl;
        exit(-1);
      }
    }
    //2.tcp连接
    bool Connect(const string& ip,uint16_t port)
    {
      sockaddr_in peer_addr;
      peer_addr.sin_family = AF_INET;
      peer_addr.sin_addr.s_addr = inet_addr(ip.c_str());
      peer_addr.sin_port = htons(port);
      socklen_t len = sizeof(peer_addr);
      int ret = connect(client_sock,(struct sockaddr*)&peer_addr,len);
      if(ret < 0)
      {
        cout<<"connect error!"<<endl;
        return false;
      }
      return true;
    }
  public:
    int client_sock;
};

class Buf
{
  public:
    bool Read(int fd,int* flag)
    {
      *flag = read(fd,buf,1024);
      if((*flag)<=0)
        return false;
      cout<<buf<<endl;
      return true;
    }
    bool Write(int fd,const string& content)
    {
      int ret = write(fd,content.c_str(),content.size());
      if(ret < 0)
        return false;
      return true;
    }
  public:
    char buf[1024];
};
