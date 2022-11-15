#include"httpresponse.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <memory>
 const std::unordered_map<int,std::string> HttpResponse::_CodeStatus = {{200,"OK"},
                                                                {400,"BAD REQUEST"},
                                                                {403,"FORBIDDENT"},
                                                                {404,"NOT FOUND"},
                                                                {500,"Method Not Implemented"}};
 const std::unordered_map<int,std::string> HttpResponse::_CodePath = {{200,"test.html"},
                                                              {400,"400.html"},
                                                              {403,"403.html"},
                                                              {404,"403.html"},
                                                              {501,"501.html"}};
 const std::unordered_map<int,std::string> HttpResponse::_CodeForm = {{400,"400.html"},
                                                              {403,"403.html"},
                                                              {404,"404.html"},
                                                              {501,"501.html"}};
HttpResponse::HttpResponse():_path("")
{

}

HttpResponse::~HttpResponse()
{

}

void HttpResponse::Init(std::string path, HttpRequest::CODESTATE state)
{
    _path = path;
    _state = state;
}

void HttpResponse::Response(Buffer &buffer)
{
    AddStateLine(buffer);
    AddHeader(buffer);
    AddContent(buffer);
}

void HttpResponse::AddStateLine(Buffer &buff)
{
    buff.Append("HTTP/1.1 "+ std::to_string(static_cast<int>(_state))+" "+_CodeStatus.find(static_cast<int>(_state))->second + "\r\n");
}

void HttpResponse::AddHeader(Buffer &buff)
{
    buff.Append("Content-Type: text/html\r\n");
}

void HttpResponse::ReadMmapData(char* pData)
{
    std::string path = _path+_CodePath.find(static_cast<int>(_state))->second;
    int fd = open(path.data(),  O_RDWR);
    if(fd == -1)
    {
        printf("open file error\n");
        return;
    }

    int len = lseek(fd,0,SEEK_END);
    if(len == -1)
    {
        printf("lseek error\n");
        return;
    }

    char *addr = (char *) mmap(NULL, len, PROT_READ, MAP_PRIVATE,fd, 0);
    if(static_cast<int>(*addr) == -1)
    {
        printf("mmaping error\n");
        return;
    }
    close(fd);

    memcpy(pData, addr, len);

    munmap(addr, len);
}

void HttpResponse::AddContent(Buffer &buff)
{
    std::shared_ptr<char> pData = std::make_shared<char>();
    ReadMmapData(pData.get());

    buff.Append(pData.get());
}

