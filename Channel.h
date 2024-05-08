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
    std::function<void()> m_readCallback;//socket读事件回调函数
    std::function<void()> m_closeCallback;//socket客户端关闭回调函数
    std::function<void()> m_errorCallback;//错误事件回调函数

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
    void handleEvent();//事件处理函数,epoll_wait()中使用

    void onMessage();
    void setReadcallback(std::function<void()> fn);//设置channel读事件回调函数,acceptor所属channel回调acceptor创建channel函数,connection所属channel回调onMessage()成员函数
    void setClosecallback(std::function<void()> fn);//设置channel连接关闭回调函数
    void setErrorcallback(std::function<void()> fn);//设置channel错误事件回调函数
};

#endif