#pragma once
#include <string>
#include "Buffer.h"
#include<algorithm>
class HttpRequest
{
public: enum CHECKSTATE {REQUEST=0,HEADER,BODY,FINISH};
        enum class CODESTATE {REQUESTSUCCESS=200,BADREQUEST=400,FORBIDDEN=403,NOTFOUND=404,UNIMPLEMENT=501};
public: HttpRequest();
        ~HttpRequest();
        void Init(std::string SrcDir);
        int parse(Buffer &buffer);
        const CODESTATE GetState()const;
        const std::string GetPath()const;
private:int ParseRequestLine(std::string &line);
        int ParseRequestHead(std::string &line);
        int ParseRequestBody(std::string &line);
        int GetRequestLineElement(std::string &line);
        std::string _method;
        std::string _url;
        std::string _vision;
        std::string _SrcDir;
        std::string _path;
        CHECKSTATE _state;
        CODESTATE _HttpStatus;
};