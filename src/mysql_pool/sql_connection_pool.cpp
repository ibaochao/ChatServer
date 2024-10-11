#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>

#include "sql_connection_pool.h"

using namespace std;


// 构造函数
sql_connection_pool::sql_connection_pool()
{
	m_CurConn = 0;
	m_FreeConn = 0;
}


// 单例
sql_connection_pool *sql_connection_pool::GetInstance()
{
	static sql_connection_pool connPool;  // 局部静态
	return &connPool;
}


// 初始化连接池
void sql_connection_pool::init(string url, string User, string PassWord, string DBName, int Port, int MaxConn)
{
	m_url = url;
	m_Port = Port;
	m_User = User;
	m_PassWord = PassWord;
	m_DatabaseName = DBName;

	for (int i = 0; i < MaxConn; i++)  // 生成若干初始连接
	{
		MYSQL *con = NULL;
		con = mysql_init(con);

		if (con == NULL)
		{	
			printf("mysql_init() 失败了, 原因: %s\n", mysql_error(con));
			exit(1);
		}
		con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), Port, NULL, 0);
		// con = 	mysql_real_connect(con,"localhost", "root", "123456", 
        //                       "yourdb", 0, NULL, 0);
		if (con == NULL)
		{ 
			printf("1mysql_query() 失败了, 原因: %s\n", mysql_error(con));
			exit(1);
		}
	
		connList.push_back(con);
		++m_FreeConn;
	}

	reserve = sem(m_FreeConn);
	m_MaxConn = m_FreeConn;
}


// 获取一个连接
MYSQL *sql_connection_pool::GetConnection()
{
	MYSQL *con = NULL;

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
bool sql_connection_pool::ReleaseConnection(MYSQL *con)
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


// 查看池中空闲连接数
int sql_connection_pool::GetFreeConn()
{
	return this->m_FreeConn;
}


// 关闭、销毁连接池
void sql_connection_pool::DestroyPool()
{

	lock.lock();
	if (connList.size() > 0)
	{
		list<MYSQL *>::iterator it;
		for (it = connList.begin(); it != connList.end(); ++it)
		{
			MYSQL *con = *it;
			mysql_close(con);
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connList.clear();
	}

	lock.unlock();
}


// 析构函数
sql_connection_pool::~sql_connection_pool()
{
	DestroyPool();
}


// 构造函数
sql_connectionRAII::sql_connectionRAII(MYSQL **SQL, sql_connection_pool *connPool){
	
	*SQL = connPool->GetConnection();  // 二级指针
	conRAII = *SQL;
	poolRAII = connPool;
}


// 析构函数
sql_connectionRAII::~sql_connectionRAII(){
	poolRAII->ReleaseConnection(conRAII);
}