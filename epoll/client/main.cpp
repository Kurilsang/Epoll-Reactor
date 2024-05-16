#include<Csocket.h>
#include<iostream>
#include<minIni.h>
#include<minGlue.h>

#define sizeArray(a) sizeof(a)/sizeof(a[0])
#define inifile "Cconfig.ini"

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
	sscanf(buffer,"%[^ ]%s",temp.status,temp.data);
	printf("响应情况:%s\n返回内容:%s\n",temp.status,temp.data);
}

int main()
{
	// 读取配置文件 
	char ip[64];
	ini_gets("network","tagetip","dummy",ip,sizeArray(ip),inifile);
	int port = ini_getl("network","port",0,inifile);
	
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
	int ret = connectServer(ip,port, clientfd);
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