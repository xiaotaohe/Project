/*************************************************************************
    > File Name: show_error.hpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Thu 20 Jun 2019 05:23:17 AM PDT
 ************************************************************************/
#pragma once
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

void show_404(int sock)
{
  char html[1024] = {0};
  strcat(html,"HTTP/1.0 200 OK\r\n");
  strcat(html,"Content-Type:text/html;charset=utf-8\r\n");
  strcat(html,"\r\n");
  strcat(html,"<html><body><h1 style=\"text-align:center\">404! Not Found!</h1><center><img src=\"/error/404.jpg\"></center></body></html>");
  send(sock,html,strlen(html),0);
}
