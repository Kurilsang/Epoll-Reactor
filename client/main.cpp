#include<Csocket.h>
#include<iostream>
#include<string.h>

using namespace std;

#include<log.h>
using namespace kuril::utility;

class part
{
	public:
	char status[16];
	char data[1024];
};

part temp;

void parsec(const char* buffer)
{
	memset(&temp,0,sizeof temp);
	sscanf(buffer,"%15[^ ]%s",temp.status,temp.data);
	printf("响应情况:%s\n返回内容:%s\n",temp.status,temp.data);
}

int main()
{
	// 初始化日志模块
	log::getInstance()->open("CLOG.log");
	// 初始化客户端
	int clientfd = getClientfd();
	if(clientfd == -1)
	{
		perror("socket error");
		fatal("socket error");
		return -1;
	}
	int ret = connectServer("192.168.148.129",6969, clientfd);
	if (ret == -1)
	{
		perror("connect error");
		fatal("connect error");
		return -1;
	}
	// 工作处理
	working(clientfd,parsec);

	return 0;
}