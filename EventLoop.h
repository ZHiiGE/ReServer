#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <functional>
#include "Epoll.h"
#include <memory>
// #include "Channel.h"
/**
 * @class:EventLoop
 * @brief:封装epoll的事件循环
*/

class Epoll;
class Channel;

class EventLoop{
private:
    std::unique_ptr<Epoll> m_ep;
public:
    EventLoop();
    ~EventLoop();
    std::function<void(EventLoop*)> m_epollwaitTimeout;

    void runLoop(int timeout=-1);//运行事件循环
    void updataChannel(Channel* ch);
    //设置epoll_wait超时回调函数
    void setEpollwaitTimeoutCallback(std::function<void(EventLoop*)> fn);
    //从epoll红黑树中删除一个channel
    void removeChannel(Channel*);
};

#endif