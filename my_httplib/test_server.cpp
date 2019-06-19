/*************************************************************************
    > File Name: test.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Tue 18 Jun 2019 09:39:47 PM PDT
 ************************************************************************/
#include"tcp.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
using namespace std;

int main()
{
  Server server("0.0.0.0",8080);
  int client_sock = -1;
  if(server.Accept(&client_sock))
  {
    while(1)
    {
      sleep(2);
      Buf buf;
      int flag;
      if(buf.Read(client_sock,&flag))
        cout<<buf.buf<<endl;
      else if(flag == 0)
      {
        cout<<"client close"<<endl;
      }
      else if(flag<0)
        return false;
    }
  }
  return 0;
}
