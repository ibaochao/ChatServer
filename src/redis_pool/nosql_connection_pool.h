#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "../lock/locker.h"
#include <hiredis/hiredis.h>

using namespace std;


// Redis连接池
class nosql_connection_pool
{
	public:
		redisContext *GetConnection();  // 获取一个连接
		bool ReleaseConnection(redisContext *conn);  // 释放一个连接
		int GetFreeConn();  // 查看池中空闲连接数
		void DestroyPool();  // 关闭、销毁连接池
		
		static nosql_connection_pool *GetInstance();  // 单例模式
		void init(string ip,string port,int MaxConn);  // 初始化连接池

	private:
		nosql_connection_pool();  // 构造函数
		~nosql_connection_pool();  // 析构函数

		int m_MaxConn;  // 最大连接数
		int m_CurConn;  // 已使用的连接数
		int m_FreeConn; // 空闲的连接数
		locker lock;  // 锁
		list<redisContext *> connList; //连接池
		sem reserve;  // 信号量

	public:
		string m_ip;  // IP地址
		string m_port;  // 端口
};


// RAII: 资源获取即初始化
class nosql_connectionRAII{
	public:
		nosql_connectionRAII(redisContext **con, nosql_connection_pool *connPool);  // 构造函数
		~nosql_connectionRAII();  // 析构函数
		
	private:
		redisContext *conRAII;  // redis连接
		nosql_connection_pool *poolRAII;  // redis连接池
};


#endif