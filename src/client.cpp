#include <iostream>

#include "HandleClient.h"

using namespace std;


int main(){
    HandleClient client;  // 创建client
    client.init();  // 初始化client
    client.run();  // 启动client
}
