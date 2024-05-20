#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <functional>
#include "Epoll.h"
#include <memory>
#include "sys/syscall.h"
#include "unistd.h"
#include <queue>
#include <mutex>
#include <sys/eventfd.h>
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
    
    std::function<void(EventLoop*)> m_epollwaitTimeout;
    //当前事件循环线程id
    pid_t m_threadId;
    //任务队列
    std::queue<std::function<void()>> m_taskqueue;
    //任务队列的互斥锁锁
    std::mutex m_mutex;
    //eventfd
    int m_eventfd;

    std::unique_ptr<Channel> m_wakeupchannel;

public:

    EventLoop();
    ~EventLoop();
    void runLoop(int timeout=-1);//运行事件循环
    void updataChannel(Channel* ch);
    //设置epoll_wait超时回调函数
    void setEpollwaitTimeoutCallback(std::function<void(EventLoop*)> fn);
    //从epoll红黑树中删除一个channel
    void removeChannel(Channel*);
    //判断当前线程是否是事件循环线程
    bool isinLoopthread(){
        return m_threadId == syscall(SYS_gettid);
    }

    //把任务添加到队列
    void addTasktoqueue(std::function<void()>);
    //唤醒事件循环函数
    void wakeUp();
    void handleWakeup();
};

#endif