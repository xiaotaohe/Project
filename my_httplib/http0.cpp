/*************************************************************************
  > File Name: http0.cpp
  > Author: 陶超
  > Mail: taochao1997@qq.com 
  > Created Time: Thu 20 Jun 2019 01:14:13 AM PDT
 ************************************************************************/
#include"tcp.hpp"
#include"show_error.hpp"
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
using namespace std;

void handler_get(int sock,char* path);
void handler_post(int sock,char* path);

//1.解析http请求报文,按行获取
int get_line(int sock,char line[],int num)
{
  assert(line);
  assert(num>0);

  char c = 'A';
  int i = 0;
  while(i<num-1 && c!='\n')
  {
    int s = recv(sock,&c,1,0);
    if(s > 0)
    {
      if(c == '\r')
      {
        //窥探
        recv(sock,&c,1,MSG_PEEK);
        if(c == '\n')
          recv(sock,&c,1,0);
        else 
          c = '\n';
      }
      line[i++] = c;
    }
}
  line[i] = '\0';
  return i;
}

//清楚报头
void clear_header(int sock)
{
  char line[1024];
  do{
    get_line(sock,line,1023);
  }while(strcmp(line,"\n"));
}
//2.获取首行
int get_first(int sock,char line[],int num)
{
  return get_line(sock,line,num);
}

//3.分析http的请求方法
void parse_method(char first[],char** method,char** query_string,char** path = NULL, int num = 0)
{
  int i = 0;
  char* url = NULL;
  char* version = NULL;
  int index = 0;
  *method = first;
  //1.分离出method //GET http://   ?zhangsan=nan&sex=nan HTTP/1.0 ---> GET
  while(i<num-1 && first[i] != ' ')
    i++;
  first[i++] = '\0';

  //2.分离url 
  url = first+i;
  while(i<num-1 && first[i] != ' ' &&  first[i] != '?')
    i++;
  
  if(first[i] == '?')
  {
    first[i++] = '\0';
    *query_string = first+i;
    while(i<num-1 &&first[i] != ' ')
      i++;
  }
  
  first[i++] = '\0';
  //3.分离出协议版本
  version = first+i;  
  if(path != NULL){
    *path = url;
  }

  //用于测试分析
  
  cout<<"url: "<<url<<endl;
  
  /*cout<<"version: "<<version<<endl;
  */ 
}

//4.构造返回结果的函数
void* handler(void* arg)
{
  cout<<"handler"<<endl;
  char buf[1024];
  //1.先获取首行进行分析
  int sock = *((int*)arg);
  int num = get_first(sock,buf,1023);
  char* method = new char[10],*query_string = new char[1024],*path = new char[1024];
  memset(method,0,10);
  parse_method(buf,&method,&query_string,&path,num);
  cout<<path<<endl;
  if(strcmp(method,"GET") == 0)
  {
    cout<<"get_func"<<endl;
    handler_get(sock,path);
  }
  else if(strcmp(method,"POST") == 0)
  {
    handler_post(sock,path);
  }
}

//5.对每种情况的错误处理
void echo_error(int sock,int error)
{
  switch(error)
  {
    case 400:
      break;
    case 403:
      break;
    case 404:
      {
        cout<<"send over!"<<endl;
        show_404(sock);
        break;
      }
  }
}

//处理本地要回显给浏览器的文件
void show_file(int sock,char* path)
{
  //1.先处理html文件
  struct stat st;
  stat(path,&st);
  cout<<"size: "<<st.st_size;
  //2.构造html页面
  char buf[1024];
  sprintf(buf,"HTTP/1.0 200 OK\r\n");
  send(sock,buf,strlen(buf),0);
  sprintf(buf,"Content-Type:text/html;charset=utf-8\r\n");
  send(sock,buf,strlen(buf),0);
  sprintf(buf,"\r\n");
  send(sock,buf,strlen(buf),0);
  
  cout<<path<<endl;
  int fd = open(path,O_RDONLY);
  sendfile(sock,fd,NULL,st.st_size);
  close(fd);
}
void handler_get(int sock,char* path_ptr)
{
  //1.根据需要，此时的报头就不再处理了
  clear_header(sock);
  //2.拼接路径
  char path[1024] = {0};
  sprintf(path,"wwwroot%s",path_ptr);
  struct stat st;
  //3.判断文件/目录属性
  if(stat(path,&st)<0)//文件不存在
  {
    cout<<"file not existed!"<<endl;
    show_404(sock);
  }
  else 
  {
    //1.判断文件文件的可读性
    if(!(st.st_mode & S_IROTH))
      return;
    if(S_ISDIR(st.st_mode)){
      strcat(path,"index.html");
      stat(path,&st);
    }
  }
  show_file(sock,path);//发送文件
  cout<<"path:\t"<<path<<endl;
}

void handler_post(int sock,char* path)
{

}
int main()
{
  //1.构建服务器
  Server server("0.0.0.0",8080);
  cout<<"处于监听状态。。。。"<<endl;
  int client_sock = -1;
  server.Accept(&client_sock);
  cout<<"get a link!"<<endl;
  char buf[1024];
  
  /*
  //2.获取请求首行
  int ret = get_first(client_sock,buf,1023);
  cout<<buf;
  
  //3.分析首行
  char* method = new char [10];
  char* query_string = new char[1024];
  parse_method(buf,&method,&query_string,NULL,ret);
  cout<<"method: "<<method<<endl;
  cout<<"query_string: "<<query_string<<endl;
  */

  //4.根据不同的请求返回不同的响应
  while(1)
  {
    handler((void*)&client_sock);
  }
  //构造response
  /*
  const char* hello = "<h1>hello wrold</h1>";
  sprintf(buf,"HTTP/1.0 200 OK\r\nContent-Length:%lu\r\n\r\n%s",strlen(hello),hello);
  write(client_sock,buf,strlen(buf));
  */
}
