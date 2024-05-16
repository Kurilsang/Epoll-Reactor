#include "Reactor.h"
#include "log.h"
#include"minIni.h"
// 初始化各项模块
using namespace std;
using namespace kuril::utility;

#define inifile "Sconfig.ini"

int ac(int epfd, int fd, uint32_t EEVENT);// accpet回调 it代表一个fd对应的item

int rd(int epfd, int fd, uint32_t EEVENT);// read回调

int cl(int epfd, int fd, uint32_t EEVENT);// close回调

int wt(int epfd, int fd, uint32_t EEVENT);// wrtie回调
