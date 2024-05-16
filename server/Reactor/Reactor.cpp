#include "Reactor.h"
#include "log.h"
#include<vector>
using namespace std;
using namespace kuril::utility;

static vector<item> items;

void StartAccept(NCALLBACK t_acceptcb,int t_epfd, int t_fd, uint32_t t_EEVENT,NCALLBACK ac, NCALLBACK rd, NCALLBACK wt, NCALLBACK cl)// 接受的函数多加一层包装，减少外部调用的代码
    {
        debug("进入ac回调的包装");
        item* temp = new item(t_epfd,t_fd,ac,rd,wt,cl);
        item it = *temp;
        items.push_back(*temp);
        t_acceptcb(t_epfd,t_fd,t_EEVENT);
        delete(temp);
    }


    item* get_item_by_fd(int t_fd)
    {
        if (items.empty())
        {
            return NULL;
        }
        // 查找 找到了就返回找不到就返回NULL
        int count=0;
        for(int i = 0; i <=items.size(); i++)
        {
            if (items[i].fd&t_fd)
            {
                return &items[i];
            }
        }
        return NULL;
}

int getServerfd()
{
    int serverfd = socket(AF_INET,SOCK_STREAM,0);

    return serverfd;
}

int startListen(int num,int port,int serverfd)// num是最大监听数 port是监听端口 serverfd是监听用的server文件描述符
{
    // bind
    sockaddr_in addr={0};
    addr.sin_family=AF_INET;addr.sin_addr.s_addr=INADDR_ANY;addr.sin_port=htons(port);
    int ret = bind(serverfd,(sockaddr*)&addr,sizeof addr);
    if (ret == -1)
    {
        perror("bind error");
        fatal("bind error");
        close(serverfd);
        return -1;
    }
    // 设置端口复用
    int opt = 1;
    setsockopt(serverfd, SOL_SOCKET,SO_REUSEADDR,&opt,sizeof opt);
    // listen
    ret = listen(serverfd,num);
    if (ret == -1)
    {
        perror("listen error");
        fatal("listen error");
        close(serverfd);
        return -1;
    }
    return 0;
}

int addToEPFD(int fd, int epfd, uint32_t EEVENT,epoll_event ev)
{
    ev.data.fd = fd;
    ev.events = EEVENT;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    
    return ret;
}
int delFromEPFD(int fd, int epfd)
{
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);

    return ret;
}
int modInEPFD(int fd, int epfd, uint32_t EEVENT, epoll_event ev)
{
    ev.data.fd = fd;
    ev.events = EEVENT;
    int ret = epoll_ctl(epfd,EPOLL_CTL_MOD,ev.data.fd,&ev);

    return ret;
}

