/*************************************************************************
    > File Name: mysql.hpp
    > Author: 陶超
    > Mail: taochao1997@qq.com 
    > Created Time: Sun 14 Jul 2019 01:59:29 AM PDT
 ************************************************************************/

#pragma once
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<mysql/mysql.h>
#include<string>
using namespace std;


class mysql 
{
  public:
    mysql()
    {}
    //1.mysql连接封装
    bool connect(string usr,string passwd,string database)
    {
      conn_ptr = mysql_init(NULL);
      if(!conn_ptr)
      {
        cout<<"mysql_init error!"<<endl;
        return false;
      }
      if(!(conn_ptr = mysql_real_connect(conn_ptr,"localhost",usr.c_str(),passwd.c_str(),database.c_str(),0,NULL,0)))
      {
        cout<<"mysql_connect error!"<<endl;
        return false;
      }
      return true;
    }

    //2.mysql插入的简单封装
    bool insert(string& sql)
    {
      int ret = mysql_query(conn_ptr,sql.c_str());
      if(!ret)
        return true;
      else 
        return false;
    }
    //3.mysql查询的简单封装
    bool select(string& sql,int* rows)
    {
      int ret = mysql_query(conn_ptr,sql.c_str());
      if(!ret)
        return false;
      else 
      {
        //2.获取查询结果
        MYSQL_RES* res = mysql_store_result(conn_ptr);
        if(!res)
          return false;
        else{
          *rows = mysql_num_rows(res);
        } 
      }
    }
  private:
    MYSQL* conn_ptr;
};
