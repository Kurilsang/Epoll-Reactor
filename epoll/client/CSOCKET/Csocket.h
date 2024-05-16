#pragma once

typedef void NCALLBACK(const char* buffer);

int getClientfd();//返回socket的fd

int connectServer(const char* ip, int port, int clientfd);// 连接函数，返回-1代表失败

void working(int clientfd, NCALLBACK callback);// 正常工作状态