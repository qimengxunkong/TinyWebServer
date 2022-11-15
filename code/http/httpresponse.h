#pragma once
#include "httprequest.h"
#include <unordered_map>
#include "Buffer.h"


class HttpResponse
{
public:HttpResponse();
       ~HttpResponse();
       void Init(std::string path, HttpRequest::CODESTATE state);
       void Response(Buffer &buffer);

private:
        void AddStateLine(Buffer &buff);
        void AddHeader(Buffer &buff);
        void AddContent(Buffer &buff);
        void ReadMmapData(char* pData);
        static const std::unordered_map<int,std::string> _CodeStatus;
        static const std::unordered_map<int,std::string> _CodePath;
        static const std::unordered_map<int,std::string> _CodeForm;
        HttpRequest::CODESTATE _state;
        std::string _path;
};