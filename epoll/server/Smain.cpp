#include "Reactor.h"
#include "log.h"
#include<minIni.h>
#include"minGlue.h"
#include"callback.h"

// 初始化各项模块
using namespace std;
using namespace kuril::utility;

#define inifile "Sconfig.ini"
#define sizearray(a) (sizeof(a)/sizeof(a[0]))

#define MAX_LENGTH listenNum
#define MAX_EVS maxEvent

int main()
{
    // 读取配置文件 
    int port = ini_getl("network","port",-1,inifile);
    int maxEvent = ini_getl("network","MAX_EVENTS",-1,inifile);
    int listenNum = ini_getl("network","listenNum",-1,inifile);
    // 初始化日志模块
    log::getInstance()->open("testLog.txt");
    log::getInstance()->level(log::DEBUG);

    // 初始化Reactor模块
    // 设置回调函数
    item A(0,0,ac,rd,wt,cl);

    // 走流程
    int serverfd = getServerfd();
    int ret = startListen(MAX_LENGTH,port,serverfd);
    if(ret == -1)
    {
        perror("listen error");
    }

    int epfd = epoll_create(1);
    // 将监听节点拉进epoll红黑树;
    epoll_event ev={0};
    addToEPFD(serverfd,epfd,EPOLLIN,ev);

    // 给后续使用的事件开辟空间
    epoll_event evs[MAX_EVS];
    int size = sizeof(evs)/sizeof(epoll_event);
    debug("进入循环");
    while(1)
    {
        int num = epoll_wait(epfd,evs,size,-1);// -1不阻塞
        for(int i = 0; i < num; ++i)
        {         
            // 监听fd
            int curfd = evs[i].data.fd;
            if(curfd == serverfd)
            {
                StartAccept(ac,epfd,serverfd,EPOLLIN,ac,rd,wt,cl);

            }else{// 处理业务fd
                item* it = get_item_by_fd(curfd);
                // 处理读事件
                if(evs[i].events&EPOLLIN) 
                {
                    
                    it->readCB(epfd,curfd,EPOLLIN);
                }
                // 处理关闭事件
                if(evs[i].events&EPOLLHUP)
                {
                    it->closeCB(epfd,curfd,NULL);
                }
                
            }

            
        }
    }

    
    return 0;
}