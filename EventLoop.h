#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include "Epoll.h"
// #include "Channel.h"
/**
 * @class:EventLoop
 * @brief:封装epoll的事件循环
*/

class Epoll;
class Channel;

class EventLoop{
private:
    Epoll *m_ep = nullptr;
public:
    EventLoop();
    ~EventLoop();

    void runLoop();//运行事件循环
    void updataChannel(Channel* ch);
};

#endif