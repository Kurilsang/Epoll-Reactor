#include "epoll.h"
#include "log.h"
// 初始化各项模块
using namespace std;
using namespace kuril::utility;

#define MAX_LENGTH 128
#define MAX_EVS 1024




int ac(int epfd, int fd, uint32_t EEVENT)// accpet回调 it代表一个fd对应的item
{
    debug("进入ac回调");
    sockaddr_in Caddr={0};
    Caddr.sin_family=AF_INET;
    socklen_t len = sizeof Caddr;
    int Cfd = accept(fd, (sockaddr*)&Caddr, &len);// 获取客户端fd和地址族

    char t_ip[128]={0};int t_port;
    inet_ntop(AF_INET,&Caddr.sin_addr.s_addr,t_ip,sizeof t_ip);
    t_port = ntohs(Caddr.sin_port);
    item* it = get_item_by_fd(fd);
    strcpy(it->ip,t_ip);it->port=t_port;

    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    addToEPFD(Cfd, epfd, EPOLLIN, ev);// 上树

    printf("客户端:%s:%d连接成功！\n",it->ip,it->port);
    return 0;
}
int rd(int epfd, int fd, uint32_t EEVENT)// read回调
{
    item* it = get_item_by_fd(fd);
    memset(it->buffer,0,sizeof it->buffer);// 防止数据污染
    int len = read(fd,it->buffer,sizeof it->buffer);
    if(len > 0)
    {
        printf("客户端%s:%d发来内容:\n%s\n",it->ip,it->port,it->buffer);
        string origin = it->buffer;// 记录一下最开始的原始数据
        char pos1[128],pos2[128],pos3[128],pos4[128],pos5[128]={0};
        sscanf(origin.c_str(),"%[^ ]",pos1);
        string temp;
        if(strcmp(pos1,"ping")==0)
        {
            temp = "200 ";temp.append("pong");
        }   
        else
        {
            temp = "400 ";temp.append("语法错误");
        }
        printf("响应：\n%s\n",temp.c_str());
        write(fd,temp.c_str(),sizeof temp);
    }
    else if(len == 0)
    {
        printf("客户端%s:%d丢失...", it->ip,it->port);
        close(fd);
    }
    else{
        perror("read error");
        close(fd);
    }

    epoll_event ev={0};
    ev.data.fd = fd;
    ev.events = EEVENT;

    epoll_ctl(epfd,EPOLL_CTL_MOD, fd, &ev);
    return 0;
}
int wt(int epfd, int fd, uint32_t EEVENT)// write回调
{
//     item* it = get_item_by_fd(fd);
    // memset(it->buffer,0,sizeof it->buffer);
    // string origin = it->buffer;// 记录一下最开始的原始数据
    // string temp = "200 ";
    // temp.append(it->buffer);
    // strcpy(it->buffer,temp.c_str());
    // int len = write(fd,it->buffer,sizeof it->buffer);
    // if(len <= 0)
    // {
    //     perror("error");
    //     temp = "404 ";
    //     temp.append("网络已断开......");
    //     int len = write(fd,it->buffer,sizeof it->buffer);
    //     printf("已经断开与客户端%s:%d的连接\n");
    //     close(fd);
    // }
    // else if(len > 0)
    // {
    //     // 正常工作
    //     char pos1[128],pos2[128],pos3[128],pos4[128],pos5[128]={0};
    //     sscanf(origin.c_str(),"%[^ ]",pos1);
    //     if(strcmp(pos1,"ping")==0)
    //     {
    //         memset(it->buffer,0,sizeof it->buffer);
    //         temp.append("pong");
    //         write(fd,temp.c_str(),sizeof temp.c_str());
    //     }
        
    // }

    // epoll_event ev={0};
    // ev.data.fd = fd;
    // ev.events = EEVENT;

    // epoll_ctl(epfd,EPOLL_CTL_MOD, fd, &ev);
    // return 0;
}

int cl(int epfd, int fd, uint32_t EEVENT)// close回调
{
    item* it = get_item_by_fd(fd);
    printf("客户端%s:%d已经断开连接...\n",it->ip,it->port);
    info("客户端%s:%d已经断开连接...\n",it->ip,it->port);
    close(fd);
    return 0;
}

int main()
{
    // 初始化日志模块
    log::getInstance()->open("testLog.txt");
    log::getInstance()->level(log::DEBUG);

    // 初始化Reactor模块
    // 设置回调函数
    item A(0,0,ac,rd,wt,cl);

    // 走流程
    int serverfd = getServerfd();
    int ret = startListen(MAX_LENGTH,6969,serverfd);
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
                // // 处理写事件
                // if(evs[i].events&EPOLLOUT)
                // {
                //     it->writeCB(epfd,curfd,EPOLLIN);
                // }
                
            }

            
        }
    }

    
    return 0;
}