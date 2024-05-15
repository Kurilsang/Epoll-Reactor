#pragma once
//使用的有关网络的库
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>// close
#include <iostream>// perror
#include <string.h>// memset

#include<functional>

using namespace std;

// typedef int NCALLBACK(int epfd, int fd, uint32_t EEVENT);
// 回调函数模板
typedef function<int(int epfd, int fd, uint32_t EEVENT)> NCALLBACK;

class item
{
    public:
    int epfd;
    int fd;
    char buffer[1024];
    char ip[128];
    int port;
    uint32_t E;
    item(int t_epfd,int t_fd,NCALLBACK ac,NCALLBACK rd, NCALLBACK wt, NCALLBACK cl):acceptCB(ac),readCB(rd),writeCB(wt),closeCB(cl)
    {
        epfd=t_epfd;
        fd=t_fd;
    }
    item(int t_epfd,int t_fd)
    {
        epfd=t_epfd;
        fd=t_fd;
    }
    void excuteAC(int epfd_, int fd_, uint32_t EEVENT)
    {
        E=EEVENT;epfd=epfd_;fd=fd_;
        acceptCB(epfd,fd,E);
    }
    //回调函数们
    NCALLBACK acceptCB;
    NCALLBACK readCB;
    NCALLBACK writeCB;
    NCALLBACK closeCB;
};

// int start_epoll(int port);
int getServerfd();// 得到监听的文件描述符的函数

int startListen(int num,int port,int serverfd);// 绑定并开始监听，返回-1说明失败

int addToEPFD(int fd, int epfd, uint32_t EEVENT,epoll_event ev);// 把某个fd上树 EEVENT为上树时具体的事件

int delFromEPFD(int fd,int epfd);// 把某个fd从树上删除

int modInEPFD(int fd,int epfd, uint32_t EEVENT,epoll_event ev);// 把某个fd的事件修改 EEVENT为修改后的事件

item* get_item_by_fd(int t_fd);// 通过fd获取它对应的item

int StartAccept(NCALLBACK t_acceptcb,int t_epfd, int t_fd, uint32_t t_EEVENT);// 接受的函数多加一层包装，减少外部调用的代码
