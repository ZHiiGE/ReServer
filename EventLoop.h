#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <functional>
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
    std::function<void(EventLoop*)> m_epollwaitTimeout;

    void runLoop(int timeout=-1);//运行事件循环
    void updataChannel(Channel* ch);
    //设置epoll_wait超时回调函数
    void setEpollwaitTimeoutCallback(std::function<void(EventLoop*)> fn);
};

#endif