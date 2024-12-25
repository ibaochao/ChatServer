# ChatServer

## 简介

技术支持：C++、 CMake、 C/S 模式、 Linux 环境、 boost、 布隆过滤器

功能： 

- 用户注册、 登录、 单聊、 群聊； 
- Redis 保存用户令牌短时间可免登录； 
- MySQL、 Redis 数据库连接池避免连接频繁创建和销毁； 
- boost 线程池实现多并发； 
- 布隆过滤器检测用户不在数据库中， 避免无权访问数据库  

## 编译运行

下载asio

```txt
https://pan.baidu.com/s/1uvZSBXs1ijGO3j8mhEQCdA?pwd=7777
```

编译

```txt
mkdir build
cd build
cmake ..
make
```

编译后项目目录

![image-20241011160940667](./ChatServer.assets/image-20241011160940667.png)

bin目录

![image-20241011160839161](./ChatServer.assets/image-20241011160839161.png)

src目录

![image-20241011161008272](./ChatServer.assets/image-20241011161008272.png)

user表

![image-20241011161121781](./ChatServer.assets/image-20241011161121781.png)

![image-20241011161239902](./ChatServer.assets/image-20241011161239902.png)

启动

```txt
./bin/server
./bin/client
```

服务端

![image-20241011161035097](./ChatServer.assets/image-20241011161035097.png)

客户端

![image-20241011161322157](./ChatServer.assets/image-20241011161322157.png)

## 测试

### 注册

注册两个用户：zhangsan lisi

![image-20241011161532323](./ChatServer.assets/image-20241011161532323.png)

服务端显示

![image-20241011162141276](./ChatServer.assets/image-20241011162141276.png)

查看user表

![image-20241011161609689](./ChatServer.assets/image-20241011161609689.png)

### 登录

登录用户zhangsan

![image-20241011161704763](./ChatServer.assets/image-20241011161704763.png)

登录用户lisi

这里是用另一个linux用户登录的，所以提示文件无写入权限，无妨

![ps_2024-10-11_16-18-45](./ChatServer.assets/ps_2024-10-11_16-18-45.png)

服务端显示

![image-20241011162213475](./ChatServer.assets/image-20241011162213475.png)

### 单聊

用户zhangsan显示

![image-20241011163454629](./ChatServer.assets/image-20241011163454629.png)

用户lisi显示

![ps_2024-10-11_16-35-24](./ChatServer.assets/ps_2024-10-11_16-35-24.png)

服务端显示

![image-20241011163856081](./ChatServer.assets/image-20241011163856081.png)

### 群聊

用户zhangsan显示

![image-20241011163740021](./ChatServer.assets/image-20241011163740021.png)

用户lisi显示

![ps_2024-10-11_16-37-52](./ChatServer.assets/ps_2024-10-11_16-37-52.png)

服务端显示

![image-20241011163924148](./ChatServer.assets/image-20241011163924148.png)

## 参考

```c++
chat_server: https://github.com/zk1556/chat_server_boost
```

# End
