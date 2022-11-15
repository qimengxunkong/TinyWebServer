

#include"httpconn.h"
std::string HttpConn::_SrcPath;
HttpConn::HttpConn():_readBuff(1024), _writeBuff(1024)
{

}

HttpConn::~HttpConn()
{

}


void HttpConn::init(int sockFd, const sockaddr_in& addr)
{
    assert(sockFd>0);
    _fd = sockFd;
    _addr = addr;
}


ssize_t HttpConn::read()
{
    return _readBuff.ReadFd(_fd);
}


ssize_t HttpConn::write()
{
    do{
        _writeBuff.WriteFd(_fd);
    }while(_writeBuff.ReadableBytes() >= 0);
    return 0;
}


void HttpConn::Close()
{

}


int HttpConn::GetFd() const
{
    return _fd;
}


int HttpConn::GetPort() const
{
    return _addr.sin_port;
}


const char* HttpConn::GetIP() const
{
    return inet_ntoa(_addr.sin_addr);
}


sockaddr_in HttpConn::GetAddr() const
{
    return _addr;
}


bool HttpConn::process(HttpConn& httpcon)
{
    httpcon._request.Init(HttpConn::_SrcPath);
    if(httpcon.read() <= 0)
    {
        return false;
    }

    httpcon._request.parse(httpcon._readBuff);
    httpcon._response.Init(httpcon._SrcPath, httpcon._request.GetState());
    httpcon._response.Response(httpcon._writeBuff);
    //int len = httpcon.write();
}
