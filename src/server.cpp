#include<iostream>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<unistd.h>
#include<queue>
#include<chrono>
#include<boost/bind.hpp>
#include<boost/asio.hpp>

#include "HandleServer.h"

using namespace std;



int main(){
    string port = "3306";  // MySQL端口
    string user = "XXXX";  // MySQL用户名
    string passwd = "XXXX";  // MySQL密码
    string databasename = "XXXX";  // MySQL数据库名称
    int sql_num = 8;  // MySQL连接池中连接数量
    
    string nosql_port = "6379";  // Redis端口
    string nosql_ip = "127.0.0.1";  // Redis IP
    int nosql_num = 8;  // Redis连接池中连接数量

    HandleServer server(port, user, passwd, databasename, sql_num, nosql_ip, nosql_port, nosql_num);  // 创建server
    server.run();  // 启动server
}
