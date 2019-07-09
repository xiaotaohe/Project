/*************************************************************************
    > File Name: Sys_client.cpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Sun 07 Jul 2019 11:49:16 PM PDT
 ************************************************************************/
#include"Sys_client.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
using namespace std;

int main()
{
  Sys_Client client;
  client.run();
	return 0;
}
