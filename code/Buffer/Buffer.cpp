#include "Buffer.h"
Buffer::Buffer(int buffersize):_buffer(buffersize),_writepos(0),_readpos(0)
{

}

void Buffer::Append(const std::string& str) {
    Append(str.data(), str.length());
}

char *Buffer::ToReadPos()
{
    return BeginPtr() + _readpos;
}

char *Buffer::ToReadEnd()
{
    return BeginPtr() + _writepos;
}

char* Buffer::BeginPtr() {
    return &*_buffer.begin();
}

char *Buffer::BeginWrite()
{
    return BeginPtr()+_writepos;
}

void Buffer::Append(const char* str, size_t len) {
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}

void Buffer::HasWritten(int len)
{
    _writepos+=len;
}

void Buffer::HasRead(int len)
{
    _readpos += len;
}

void Buffer::RetrieveUntil(char* pos)
{
    assert(pos == NULL);
    assert(ReadableBytes() <= (pos - ToReadPos()));
    HasRead(pos - ToReadPos());
}

void Buffer::EnsureWriteable(int len)
{
    if(WriteableBytes() < len)
    {
        MakeSpace(len);
    }
}

int Buffer::ReadFd(int fd)
{
    char buff[65535] = "\0";
    const int len = read(fd,buff,sizeof(buff));

    if(len < 0)
    {
        return -1;
    }
    else if(len <= WriteableBytes())
    {
        std::copy(buff,buff+len,_buffer.begin());
    }
    else
    {
        MakeSpace(len);
        std::copy(buff,buff+len,_buffer.begin());
    }
    _writepos += len;
    return len;
}

int Buffer::ReadableBytes()const
{
    return _writepos - _readpos;
}

int Buffer::WriteFd(int fd)
{
    int len = ReadableBytes();
    if(len <= 0)
    {
        return -1;
    }

    int WriteLen = write(fd,_buffer.data(),len);
    _readpos += WriteLen;
    return WriteLen;
}

void Buffer::MakeSpace(int size)
{
    _buffer.resize(size + 1);
}

int Buffer::WriteableBytes()const
{
    return _buffer.size() - _writepos;
}