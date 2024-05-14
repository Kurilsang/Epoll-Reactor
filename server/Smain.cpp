#include "epoll.h"
#include "log.h"
// 初始化各项模块
using namespace std;
using namespace kuril::utility;

#define MAX_LENGTH 128
#define MAX_EVS 1024

int ac(int epfd, int fd, uint32_t EEVENT)// accpet回调
{
    sockaddr_in Caddr={0};
    Caddr.sin_family=AF_INET;
    socklen_t len = sizeof Caddr;
    int Cfd = accept(fd, (sockaddr*)&Caddr, &len);// 获取客户端fd
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    addToEPFD(Cfd, epfd, EPOLLIN, ev);// 上树
}
int rd(int epfd, int fd, uint32_t EEVENT)// read回调
{
    

    epoll_event ev={0};
    ev.data.fd = fd;
    ev.events = EEVENT;
    epoll_ctl(epfd,EPOLL_CTL_MOD, fd, &ev);
}
int wt(int epfd, int fd, uint32_t EEVENT)// write回调
{
    

    epoll_event ev={0};
    ev.data.fd = fd;
    ev.events = EEVENT;
    epoll_ctl(epfd,EPOLL_CTL_MOD, fd, &ev);
}

int main()
{
    // 初始化日志模块
    log::getInstance()->open("testLog.txt");
    log::getInstance()->level(log::DEBUG);

    // 初始化Reactor模块
    // 设置回调函数

    // 走流程
    int serverfd = getServerfd();
    startListen(MAX_LENGTH,6969,serverfd);

    int epfd = epoll_create1(1);
    // 将监听节点拉进epoll红黑树;
    epoll_event ev={0};
    addToEPFD(serverfd,epfd,EPOLLIN,ev);

    // 给后续使用的事件开辟空间
    epoll_event evs[MAX_EVS]={0};
    int size = sizeof(evs)/sizeof(epoll_event);


    // 获取客户端地址
    sockaddr_in Caddr={0};
    Caddr.sin_family=AF_INET;
    socklen_t len = sizeof Caddr;


    while(1)
    {
        int num = epoll_wait(epfd,evs,size,-1);// -1不阻塞
        for(int i = 0; i < num; ++i)
        {         
            // 监听fd
            int curfd = evs[i].data.fd;
            if(curfd == serverfd)
            {
                StartAccept(ac,epfd,serverfd,EPOLLIN);

            }else{// 处理业务fd
                item* it = get_item_by_fd(curfd);
                // 处理读事件
                if(evs[i].events&EPOLLIN) 
                {
                    it->readCB(epfd,curfd,EPOLLOUT);
                }
                // 处理写事件
                if(evs[i].events&EPOLLOUT)
                {
                    it->writeCB(epfd,curfd,EPOLLIN);
                }
                
            }

            
        }
    }
    close(serverfd);
    close(epfd);
    
    return 0;
}