#ifndef _EPOLL_H
#define _EPOLL_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <sys/epoll.h>
#include <unistd.h>//close()
#include <strings.h>//bzero()
#include <string.h>//strlen()
#include "Channel.h"

/**
 * @class:Epoll
 * @brief:封装epoll的api操作
*/

class Channel;

class Epoll{
private:
    static const int MaxEvents = 100;//最大事件数
    
    int m_epollfd = -1;//epoll句柄
    epoll_event m_events[MaxEvents];


public:
    Epoll();
    ~Epoll();

    void updataChannel(Channel* ch);
    //删除某个channel
    void removeChannel(Channel* ch);
    std::vector<Channel*> loop(int timeout = -1);
    // int epollfd();
};

#endif