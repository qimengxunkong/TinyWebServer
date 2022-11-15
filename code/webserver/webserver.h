#pragma once
#include <stdio.h>
#include <string.h>
#include <string>
#include "httpconn.h"
#include "ThreadPool.h"
class WebServer
{
public: WebServer(int port,int ThreadNum);
        ~WebServer();
        void Start();
        
private:
        bool InitSocket();
        void AddClient(int fd, sockaddr_in& addr);
        std::string _resourcePath;
        int _port;//端口号
        int _listenFD;//监听文件描述符
        HttpConn _conn;
        static const int MaxFdNum = 65536;//最大文件描述符数量
        std::unordered_map<int, HttpConn> _user;
        ThreadPool _pool;
        bool _isClose;
};