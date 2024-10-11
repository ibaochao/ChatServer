#ifndef _HANDLECLIENT_H
#define _HANDLECLIENT_H

#include<iostream>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <pthread.h>
#include <fcntl.h>
#include <fstream>

using namespace std;


class HandleClient{
	public:
		HandleClient(){

		}
		
		~HandleClient(){	
			close(cfd);
		}
		
		void init();  // 初始化
		void run();  // 运行
		
		static void *handle_send(void *arg);  // 发送消息
		static void *handle_recv(void *arg);  // 接收消息

	private:
		int cfd;  // 套接字
};


#endif