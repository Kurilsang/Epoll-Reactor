#include "epoll.h"
#include "log.h"
#include<vector>
using namespace std;
using namespace kuril::utility;

static vector<item> items;

// int start_epoll(int port)
// {
//     //socket创建
//     int server = socket(AF_INET, SOCK_STREAM, 0);
//     if (-1 == server)
//     {
//         perror("server socket error");
//         return -1;
//     }

//     //绑定地址簇
//     struct sockaddr_in ser_addr;//server_addr
//     memset(&ser_addr,0,sizeof(ser_addr));
//     ser_addr.sin_family = AF_INET;
//     ser_addr.sin_port = htons(port);
//     ser_addr.sin_addr.s_addr = INADDR_ANY;//获取本地所有 与 addr_inet("0.0.0.0")等同
//     int ret = bind(server,(sockaddr*)&ser_addr, sizeof(ser_addr));
//     if (-1 == ret)
//     {
//         perror("bind error");
//         fatal("bind失败");
//         return -1;
//     }
//     //设置端口复用
//     int opt = 1;
//     setsockopt(server, SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
//     //listen
//     ret = listen(server,128);
//     if (-1 == ret)
//     {
//         perror("listen error");
//         fatal("listen失败");
//         return -1;
//     }
//     //epoll模型
//     int epfd = epoll_create(128);
//     if (-1 == epfd)
//     {
//         perror("epoll create error");
//         fatal("epoll模型创建失败");
//         return -1;
//     }
//     //添加检测节点
//     struct epoll_event ev;
//     ev.events = EPOLLIN;
//     ev.data.fd = server;
//     ret = epoll_ctl(epfd,EPOLL_CTL_ADD,server,&ev);
//     if (-1 == ret)
//     {
//         perror("epoll_ctl error");
//         fatal("epoll模型添加事件失败");
//         return -1;
//     }
//     //事件集及其个数
//     struct epoll_event evs[1024];
//     int size = sizeof(evs) / sizeof(struct epoll_event);
//     //用于获取客户端ip和端口
//     struct sockaddr_in client_addr;
//     memset(&client_addr,0,sizeof(sockaddr_in));
//     client_addr.sin_family = AF_INET;
//     int len_c = sizeof(sockaddr_in);//转化为左值，后面转换类型能用
//     debug("epoll模型建立完成,开始进入循环工作...");
//     //accept
//     while(1)
//     {
//         //每轮开始先检测
//         int num = epoll_wait(epfd,evs,size,-1);
//         for (int i = 0; i < num; ++i)
//         {
//             int curfd = evs[i].data.fd;
//             if (server == curfd)//若为监听描述符
//             {
//                 int acfd = accept(server,(sockaddr*)&client_addr,(socklen_t*)&len_c);
//                 //接收到了就将其加入epoll
//                 ev.events = EPOLLIN;
//                 ev.data.fd = acfd;
//                 ret = epoll_ctl(epfd,EPOLL_CTL_ADD, acfd, &ev);
//                 if (-1 == ret)
//                 {
//                     perror("accept epoll_ctl error");
//                     return -1;
//                 }
//             }
//             else//处理业务
//             {
//                 char buf[1024];
//                 memset(&buf, 0 , sizeof(buf));
//                 int len = recv(curfd, buf, sizeof(buf), 0);
//                 if (0 == len)//断开
//                 {
//                     cout << "客户端断开连接" << endl;
//                     epoll_ctl(epfd,EPOLL_CTL_DEL,curfd,NULL);
//                     close(curfd);
//                 }
//                 else if (0 < len)//正常工作
//                 {
//                     cout<<"客户端：" << buf << endl;
//                     send(curfd, "200 你好亚", sizeof("200 你好亚"), 0);
//                 }
//                 else
//                 {
//                     perror("recv error");
//                     return -1;
//                 }
//             }
//         }
//     }
//     return 0;
// }

void StartAccept(NCALLBACK t_acceptcb,int t_epfd, int t_fd, uint32_t t_EEVENT,NCALLBACK ac, NCALLBACK rd, NCALLBACK wt, NCALLBACK cl)// 接受的函数多加一层包装，减少外部调用的代码
    {
        debug("进入ac回调的包装");
        item* temp = new item(t_epfd,t_fd,ac,rd,wt,cl);
        item it = *temp;
        items.push_back(*temp);
        cout<<"1"<<endl;
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

