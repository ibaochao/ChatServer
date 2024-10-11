#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>

#include "nosql_connection_pool.h"

using namespace std;


// 构造函数
nosql_connection_pool::nosql_connection_pool()
{
	m_CurConn = 0;
	m_FreeConn = 0;
}


// 单例模式
nosql_connection_pool *nosql_connection_pool::GetInstance()
{
	static nosql_connection_pool connPool;  // 局部静态
	return &connPool;
}


// 初始化连接池
void nosql_connection_pool::init(string ip,string port, int MaxConn)
{
	m_ip = ip;
	m_port = port;

	for (int i = 0; i < MaxConn; i++)
	{	
		redisContext *con = redisConnect("127.0.0.1", 6379);
		if (con->err)
		{
			redisFree(con);
			cout << "连接redis失败" << endl;
		}

		connList.push_back(con);
		++m_FreeConn;
	}

	reserve = sem(m_FreeConn);

	m_MaxConn = m_FreeConn;
}


// 获取一个连接
redisContext *nosql_connection_pool::GetConnection()
{
	redisContext *con = NULL;

	if (0 == connList.size())
		return NULL;

	reserve.wait();  // 先看信号量，后加锁
	
	lock.lock();

	con = connList.front();
	connList.pop_front();

	--m_FreeConn;
	++m_CurConn;

	lock.unlock();
	return con;
}


// 释放一个连接
bool nosql_connection_pool::ReleaseConnection(redisContext *con)
{
	if (NULL == con)
		return false;

	lock.lock();

	connList.push_back(con);
	++m_FreeConn;
	--m_CurConn;

	lock.unlock();

	reserve.post();  // 先加锁，后看信号量
	return true;
}


// 关闭、销毁连接池
void nosql_connection_pool::DestroyPool()
{
	lock.lock();
	if (connList.size() > 0)
	{
		list<redisContext *>::iterator it;
		for (it = connList.begin(); it != connList.end(); ++it)
		{
			redisContext *con = *it;
			redisFree(con);
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connList.clear();
	}

	lock.unlock();
}


// 查看池中空闲连接数
int nosql_connection_pool::GetFreeConn()
{
	return this->m_FreeConn;
}


// 析构函数
nosql_connection_pool::~nosql_connection_pool()
{
	DestroyPool();
}


// 构造函数
nosql_connectionRAII::nosql_connectionRAII(redisContext **SQL, nosql_connection_pool *connPool){
	*SQL = connPool->GetConnection();
	
	conRAII = *SQL;
	poolRAII = connPool;
}

// 析构函数
nosql_connectionRAII::~nosql_connectionRAII(){
	poolRAII->ReleaseConnection(conRAII);
}