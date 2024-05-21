#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <functional>
#include "Epoll.h"
#include <memory>
#include "unistd.h"
#include <queue>
#include <map>
#include <mutex>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
// #include "Channel.h"
/**
 * @class:EventLoop
 * @brief:封装epoll的事件循环
*/

class Epoll;
class Channel;
class Connection;

class EventLoop{
private:
    //标志 true为主事件循环
    bool m_mainloopflag;
    //定时器间隔
    int m_timeval;
    //超时时间
    int m_timeout;
    std::unique_ptr<Epoll> m_ep;
    //超时事件的函数对象
    std::function<void(EventLoop*)> m_epollwaitTimeout;
    //当前事件循环线程id
    pid_t m_threadId;
    //任务队列
    std::queue<std::function<void()>> m_taskqueue;
    //任务队列的互斥锁锁
    std::mutex m_mutex;
    //eventfd
    int m_eventfd;
    //唤醒事件channel
    std::unique_ptr<Channel> m_wakeupchannel;
    //timerfd定时器fd
    int m_timerfd;
    //定时器channel
    std::unique_ptr<Channel> m_timerchannel;
    //事件循环中的Connection对象
    std::map<int, std::shared_ptr<Connection>> m_conns;
    //m_conns的互斥锁
    std::mutex m_connmutex;
    //conn超时，删除Tcpserver::map中的conn
    std::function<void(int)> m_ConnTimeout;



public:

    EventLoop(bool mainloopflag, int m_timeval=30, int m_timeout=60);
    ~EventLoop();
    bool isMainloop(){
        return m_mainloopflag;
    }
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
    //从TcpServer对象中删除conn
    void setHandleConnTimeout(std::function<void(int)>);
    //把任务添加到队列
    void addTasktoqueue(std::function<void()>);
    //唤醒事件循环函数
    void wakeUp();
    //唤醒事件回调函数
    void handleWakeup();
    //定时器回调函数
    void handleTimer();

    //添加connection对象
    void newConnection(std::shared_ptr<Connection>);
};

#endif