#pragma once
#include"httpresponse.h"
#include"httprequest.h"
#include"../Buffer/Buffer.h"
#include <netinet/in.h>
#include <cassert>
#include <arpa/inet.h>
class HttpConn
{
public:
    HttpConn();

    ~HttpConn();

    void init(int sockFd, const sockaddr_in& addr);

    ssize_t read();

    ssize_t write();

    void Close();

    int GetFd() const;

    int GetPort() const;

    const char* GetIP() const;

    sockaddr_in GetAddr() const;

    static bool process(HttpConn& httpcon);


    static std::string _SrcPath;
private:

    int _fd;
    struct  sockaddr_in _addr;

    bool _isClose;

    Buffer _readBuff;
    Buffer _writeBuff;

    HttpRequest _request;
    HttpResponse _response;
};