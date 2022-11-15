#include<iostream>
using namespace std;
#include"webserver/webserver.h"

int main()
{
    WebServer web(8080,6);
    web.Start();
    return 0;
}