#include <cstring>
#include <sys/stat.h>
#include"httprequest.h"

HttpRequest::HttpRequest():_method(""),_url(""),_path(""),_vision(""),_state(REQUEST),_HttpStatus(CODESTATE::REQUESTSUCCESS)
{

}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::Init(std::string SrcDir)
{
    _method = "";
    _url = "";
    _SrcDir = SrcDir;
}

int HttpRequest::parse(Buffer &buffer)
{
    const char CRLF[] = "\r\n";
    char *lineEnd = std::search(buffer.BeginPtr(),buffer.ToReadEnd(),CRLF,CRLF+2);
    if(lineEnd == buffer.ToReadEnd())
    {
        //badrequest();
    }

    std::string line(buffer.BeginPtr(),lineEnd);
    while(_state != FINISH)
    {
        switch(_state)
        {
            case REQUEST:
                if(ParseRequestLine(line))
                {
                    return -1;
                }
                return 0;
                break;
            case HEADER:
                break;
            case BODY:
                break;
            default:break;
        }
    }
    return 0;
}

int HttpRequest::ParseRequestLine(std::string &line)
{
    int MethodSpacePosition = line.find(" ");
    if(MethodSpacePosition == line.npos)
    {
        _HttpStatus = CODESTATE::BADREQUEST;
        return -1;
    }
    _method = std::string(line,0,MethodSpacePosition);
    int UrlSpacePosition = line.find(" ",MethodSpacePosition+1);
    if(UrlSpacePosition == line.npos)
    {
        _HttpStatus = CODESTATE::BADREQUEST;
        return -1;
    }
    _url = std::string(line,MethodSpacePosition+1,UrlSpacePosition-MethodSpacePosition-1);
    if(!strcmp(_url.data(),"/"))
    {
        _url = "test.html";
    }
    _vision = std::string(line,UrlSpacePosition+1);

    if(strcasecmp(_method.data(), "GET"))
    {
        _HttpStatus = CODESTATE::UNIMPLEMENT;
        return -1;
    }
    std::string resource = _SrcDir + _url;
    struct stat file_stat;
    if(stat(resource.data(),&file_stat))
    {
        _HttpStatus = CODESTATE::NOTFOUND;
        return -1;
    }
    if(!(file_stat.st_mode & S_IROTH))
    {
        _HttpStatus = CODESTATE::FORBIDDEN;
        return -1;
    }
    if(S_ISDIR(file_stat.st_mode))
    {
        _HttpStatus = CODESTATE::BADREQUEST;
        return -1;
    }
    if(strcasecmp(_vision.data(),"HTTP/1.1"))
    {
        _HttpStatus = CODESTATE::BADREQUEST;
        return -1;
    }
    _HttpStatus = CODESTATE::REQUESTSUCCESS;
    return 0;
}

const HttpRequest::CODESTATE HttpRequest::GetState()const
{
    return _HttpStatus;
}

const std::string HttpRequest::GetPath()const
{
    return _path;
}

int HttpRequest::ParseRequestHead(std::string &line)
{

}

int HttpRequest::ParseRequestBody(std::string &line)
{

}
