#include"callback.h"
#include<vector>
#include<sstream>

class requestPort//请求报文的内容
{
    public:
    string data;
};

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
return 0;// no use here
}

int cl(int epfd, int fd, uint32_t EEVENT)// close回调
{
    item* it = get_item_by_fd(fd);
    printf("客户端%s:%d已经断开连接...\n",it->ip,it->port);
    info("客户端%s:%d已经断开连接...\n",it->ip,it->port);
    close(fd);
    return 0;
}

