/*************************************************************************
  > File Name: mysql_test.cpp
  > Author: 陶超
  > Mail: taochao1997@qq.com 
  > Created Time: Sat 13 Jul 2019 01:27:50 AM PDT
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<mysql/mysql.h>
using namespace std;

int main()
{
  //1.创建mysql对象
  MYSQL* conn_ptr;
  //2.初始化
  conn_ptr = mysql_init(NULL);
  if(!conn_ptr)
  {
    perror("mysql_init error!\n");
    return -1;
  }

  //3.连接数据库
  conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","1230","school",0,NULL,0);
  if(conn_ptr)
    cout<<"connection success!"<<endl;
  else 
  {
    cout<<"connection failed!"<<endl;
    return -1;
  }
  //4.插入数据
  int ret = mysql_query(conn_ptr,"insert into stu values(2,'zhangsan')");
  if(!ret)
    cout<<"insert success!"<<endl;
  //4.查
  ret = mysql_query(conn_ptr,"select id from stu where id = 10");
  if(!ret)
  {
    cout<<"select success!"<<endl;
    MYSQL_RES* res_ptr = mysql_store_result(conn_ptr);
    if(res_ptr)
    {
      printf("%lu Rows\n",(int)mysql_num_rows(res_ptr));
      int j = mysql_num_fields(res_ptr);
      MYSQL_ROW sqlrow;
      while((sqlrow=mysql_fetch_row(res_ptr))){
        for(int i = 0;i<j;i++)
          printf("%s\t",sqlrow[i]);
        printf("\n");
      }
    }
  else 
  {
    cout<<"select error!"<<endl;
  }

  //5.
  mysql_close(conn_ptr);
  return 0;
}
