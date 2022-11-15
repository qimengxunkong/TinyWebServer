#pragma once
#include<vector>
#include <sys/uio.h>
#include <unistd.h>
#include <algorithm>
#include <string>
#include<assert.h>
class Buffer
{
public:Buffer(int buffersize);
       char *BeginPtr();
       char *BeginWrite();
       char *ToReadPos();
       char *ToReadEnd();
       void Append(const std::string& str);
       void Append(const char *str, size_t len);
       void RetrieveUntil(char *pos);
       int ReadFd(int fd);
       int WriteFd(int fd);
       int WriteableBytes()const;
       int ReadableBytes()const;
       void EnsureWriteable(int len);
private:void HasWritten(int len);
        void HasRead(int len);
        void MakeSpace(int size);
        std::vector<char> _buffer;
        int _writepos;
        int _readpos;
};

