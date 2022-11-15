#include"webserver.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <thread>
#include <cassert>

WebServer::WebServer(int port, int ThreadNum):_port(port),_isClose(true),_pool(ThreadNum)
{
    char *pBuff = NULL;
    pBuff = getcwd(NULL,1024);
    assert(pBuff != NULL);
    std::string tempstr(pBuff);
    int pos = tempstr.find_last_of('/');
    assert(pos != std::string::npos);

    _resourcePath = std::string(tempstr,0,pos);
    _resourcePath += "/httpdocs/";
    HttpConn::_SrcPath = _resourcePath;

    free(pBuff);
    if(!InitSocket())
		_isClose = false;
}

WebServer::~WebServer()
{
	close(_listenFD);
}

void WebServer::Start()
{
	int clientsocket = -1;
	struct sockaddr_in client_name;
	 socklen_t client_name_len = sizeof(client_name);

	if(_isClose)
	{
		while(1)
		{
			clientsocket = accept(_listenFD,(struct sockaddr *)&client_name,&client_name_len);
			if(clientsocket == -1)
				printf("clientsocket err\n");
			AddClient(clientsocket, client_name);
			_pool.AddTask(HttpConn::process, (_user[clientsocket]));
		}
		
	}
	
}

bool WebServer::InitSocket()
{
     int option;
	 struct sockaddr_in name;
	//设置http socket
	 _listenFD = socket(PF_INET, SOCK_STREAM, 0);
	 if (_listenFD == -1)
		return false;//连接失败
	
	socklen_t optlen;
	optlen = sizeof(option);
    option = 1;
    setsockopt(_listenFD, SOL_SOCKET, SO_REUSEADDR, (void *)&option, optlen);
	
	
	 memset(&name, 0, sizeof(name));
	 name.sin_family = AF_INET;
	 name.sin_port = htons(_port);
	 name.sin_addr.s_addr = htonl(INADDR_ANY);

	 if (bind(_listenFD, (struct sockaddr *)&name, sizeof(name)) < 0)
	  return false;//绑定失败

	 if (listen(_listenFD, 5) < 0)
	  return false;
	 return true;
}

void WebServer::AddClient(int fd, sockaddr_in& addr)
{
	assert(fd > 0);
	_user[fd].init(fd, addr);
}