#include "Csocket.h"
#include <sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
using namespace std;
#include<string.h>

#include<log.h>
using namespace kuril::utility;

typedef void NCALLBACK(const char* buffer);

int getClientfd()
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    return clientfd;
}

int connectServer(const char *ip, int port, int clientfd)
{
    struct sockaddr_in addr;// 要连接的服务器的地址族信息
    memset(&addr,0,sizeof addr);
    addr.sin_family=AF_INET;addr.sin_port=htons(port);addr.sin_addr.s_addr=inet_addr(ip);

    int len=sizeof addr;
    int ret = connect(clientfd, (struct sockaddr*)&addr,len);
    return ret;
}

void working(int clientfd, NCALLBACK callback)
{
    char buffer[1024];
    int ret;
    while(1)
    {
        cin.getline(buffer,sizeof buffer);
        ret = write(clientfd, buffer, sizeof buffer);
        if (ret == -1)
        {
            perror("client write error");
            fatal("client write error");
        }
        memset(buffer,0,sizeof buffer);
        int ret = read(clientfd,buffer,sizeof buffer);
        if(ret > 0)
        {
            // 正常处理业务流程
            callback(buffer);
        }
        else if(ret == 0)
        {
            // 与服务器断开连接
            printf("与服务器已经断开连接\n");
            info("客户端与服务器断开连接");
        }
        else
        {
            // 发生异常
            perror("working error");
            fatal("异常发生");
        }
    }
}
