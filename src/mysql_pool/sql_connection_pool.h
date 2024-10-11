#ifndef _SQL_CONNECTION_POOL_
#define _SQL_CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>

#include "../lock/locker.h"


using namespace std;


// MySQL连接池
class sql_connection_pool
{
	public:
		MYSQL *GetConnection();	 // 获取一个连接
		bool ReleaseConnection(MYSQL *conn);  // 释放一个连接
		int GetFreeConn();  // 查看池中空闲连接数
		void DestroyPool();  // 关闭、销毁连接池

		static sql_connection_pool *GetInstance();  // 单例模式
		void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn);  // 初始化连接池

	private:
		sql_connection_pool();  // 构造函数
		~sql_connection_pool();  // 析构函数

		int m_MaxConn;  // 最大连接数
		int m_CurConn;  // 已使用的连接数
		int m_FreeConn; // 空闲的连接数
		locker lock;  // 锁
		list<MYSQL *> connList; //连接池
		sem reserve;  // 信号量

	public:
		string m_url;  // IP地址
		string m_Port;  // 端口
		string m_User;  // 用户名
		string m_PassWord;  // 密码
		string m_DatabaseName;  // 数据库名
};


// RAII: 资源获取即初始化
class sql_connectionRAII{
	public:
		sql_connectionRAII(MYSQL **con, sql_connection_pool *connPool);  // 构造函数
		~sql_connectionRAII();  // 析构函数
	private:
		MYSQL *conRAII;  // MySQL连接
		sql_connection_pool *poolRAII;  // MySQL连接池
};


#endif