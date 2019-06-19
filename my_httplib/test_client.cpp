/*************************************************************************
    > File Name: test_client.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Tue 18 Jun 2019 10:11:10 PM PDT
 ************************************************************************/
/*
 * 本段代码目标：测试封装的tcp的client
 */ 
#include"tcp.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdlib.h>
#include<string.h>
using namespace std;

int main()
{
  Client client;
  if(!client.Connect("0.0.0.0",8080))
  {
    cout<<"Connect error!"<<endl;
  }
  char* buf = "hello my_tcp!";
  while(1)
  {
    write(client.client_sock,buf,strlen(buf));
    sleep(2);
  }
	return 0;
}
