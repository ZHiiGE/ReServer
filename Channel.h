#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "EventLoop.h"
// #include "Epoll.h"
#include "Socket.h"
#include <functional>

/**
 * @class:Channel类
 * @brief:封装TCP连接建立成功的socket通道
*/
class EventLoop;

class Channel{
private:
    int m_fd = -1;//通道对应的socket描述符fd
    // Epoll* m_ep = nullptr;//对应的epoll描述符(红黑树)
    EventLoop* m_evloop;//对应的epoll的事件循环
    bool m_inepoll = false;//是否加入epoll红黑树中
    uint32_t m_events = 0; //需要监视的事件
    uint32_t m_revents = 0;//已发生的事件
    std::function<void()> m_readCallback;


public:
    Channel(EventLoop* evloop, int fd);
    ~Channel();

    int fd();//返回socket对应的fd
    void useET();//设置边缘触发
    void enablereading();//让epoll_wait()监听m_fd的读事件
    void setInepoll();//m_inepoll为true;
    void setRevents(uint32_t ev);//m_revent为ev
    bool inEopll();
    uint32_t events();
    uint32_t revents();
    void handleEvent();//事件处理函数,epoll_wait()返回调用

    void onMessage();
    void setReadcallback(std::function<void()> fn);
};

#endif