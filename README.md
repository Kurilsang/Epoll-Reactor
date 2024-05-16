# Epoll-Reactor
一个基础的Reactor模型封装
# 目录
+ 食用指南
+ 实现功能
+ 想说的话
***
## 食用指南
+ 运行环境：Linux（Ubuntu64位）
+ 使用语言：C/C++
+ 具体步骤：修改server文件夹和client文件夹下的Sconfig.ini和Cconfig.ini，注意使得port是同样的，要将Cconfig中的ip设置为本地ip（可通过Linux的ifconfig指令查看）
## 实现功能
+ 通过socket编程，使用epoll封装成了基础的Reactor模型，即IO多路复用+回调函数
+ 使用了日志模块，日志模块的使用指令分别为debug("测试%s",变量)，info("测试%s",变量)，error("测试%s",变量)，fatal("测试%s",变量).
+ 从配置文件中读取信息载入程序，不用修改源代码就可以修改关键参数
+ 实现的指令：ping
## 想说的话
一天之内只做出了这么点内容，感觉有点不好意思交
<br>
<br>
（之前写了几天的Reactor在14号晚上的时候由于虚拟机内存爆了，开不了机，自己瞎折腾之后不管啥都不凑效，彻底用不了了，本地仓库的东西全部遗失了）
<br>
<br>
算是一个教训，下次弄虚拟机得多弄点快照，之前都没有这个习惯的。
<br>
<br>
    本来的话这种完成度其实是不打算交的，但是听说有可能是最后一轮考核，决定还是把目前的情况发一下，算是对自己的一个总结。
<br>
<br>
    首先嘛这一次我的考核做的并不是很好因此也没办法说些什么，因此我就想讲讲这一个月半以来我大概学了些什么，以及我的一些想法。
<br>
<br>
    首先是去隔壁算法组参加机考，为此学了一段时间的算法，一直持续到机考结束，懂了一些基础的算法，例如贪心，暴力，dfs，双指针，gcd，lcm，前缀和差分啥的（不能说一定写的出来但是至少看懂是没压力了）。
虽然机考的时候也考的感觉不咋样，不过进步倒也确实是有的，从最开始的一窍不通变得能够写出一些基本的解法，并且开始逐渐对时间复杂度有了概念，但是代价是没有啥基础，学的时候花了比较长的时间。<br>
此外就是借着碎片时间学了一点unity，看了有一段时间的课，毕竟对这个方向是有点兴趣的。
然后弄到linux虚拟机也是花了几天研究了一下基本用法和配置环境。<br><br>
    再后来就是来学后端了，但是吧。。。回头看看其实我学的方向有点问题。最开始我是先做了一点考核，但是有点压力，就想从网课里寻找一些灵感，因为我觉得我这部分内容没有经过一个很系统的学习，写出来的东西很野鸡，大概看了有段时间，虽然对代码有帮助，但是确实不大orz，因为我看的好像是偏理论的，要么讲八股文要么讲底层网络原理，而这些又占了我很大部分时间，我后面才意识到时间有点不够去学着做考核了。听了这么久感觉最大的收获比如说什么三次握手四次挥手，客户端与服务端读写的缓冲区，cs和bs模型，请求报文和响应报文啥的。<br><br>
    总的来说呢....我个人觉得是学确实有学，也不少，但是分散下来感觉每一个都做的不是很好，我认为是我规划的问题。此外我也开始遇到一些c++的问题，经常由于各种细节报错。有种遇到瓶颈的感觉，而且c++后端的课程很多都是后面就付费的，而且有价值的蛮难找的，生态环境感觉不是很好，之后我可能会考虑转java来写后端。c++还有再学的话往硬件方向去学。此外就是我有点忽略了实践了，回头来看这点非常不好，至少在拍代码这件事上绝对不行。脱离实际学了一堆东西套回来的结果就是不停的报错和奇怪的bug。<br><br>
说了这么多总的来说我也不是很满意我这次考核的情况哈哈哈，不管之后怎样这对我来说都是一个教训。至少下次做项目肯定是得要面对项目来学的，跑去学计算机网络跑的有点远了。<br><br>
感谢能看到这里的师兄师姐。
