#include "Channel.h"
#include "Connection.h"
Channel::Channel(const std::unique_ptr<EventLoop>& evloop, int fd)
            :m_evloop(evloop), m_fd(fd){
    
};
Channel::~Channel(){
    //ep fd不属于Channel类 ,channel只是管理记录
};

int Channel::fd(){
    return m_fd;
}

//设置边缘触发
void Channel::useET(){
    m_events |= EPOLLET;
}

//让事件循环eventloop中注册该channel连接读事件
void Channel::enablereading(){
    m_events |= EPOLLIN;
    m_evloop->updataChannel(this);
}
//让事件循环eventloop中取消监听该channel连接读事件
void Channel::disablereading(){
    m_events &= ~EPOLLIN;
    m_evloop->updataChannel(this);
}
//让事件循环eventloop中注册该channel连接写事件
void Channel::enablewriting(){
    m_events |= EPOLLOUT;
    m_evloop->updataChannel(this);
}
//让事件循环eventloop中取消监听该channel连接写事件
void Channel::disablewriting(){
    m_events &= ~EPOLLOUT;
    m_evloop->updataChannel(this);
}
//取消所有事件
void Channel::disableAll(){
    m_events = 0;
    m_evloop->updataChannel(this);
}
//从事件循环中删除channel
void Channel::remove(){
    disableAll();                   //取消全部事件
    m_evloop->removeChannel(this);  //从epoll红黑树中删除该channel

}
//m_inepoll为true
void Channel::setInepoll(){
    m_inepoll = true;
}

//m_revent为ev
void Channel::setRevents(uint32_t ev){
    m_revents = ev;
}

bool Channel::inEopll(){
    return m_inepoll;
}

uint32_t Channel::events(){
    return m_events;
}

uint32_t Channel::revents(){
    return m_revents;
}

//事件处理函数,epoll_wait()返回调用
void Channel::handleEvent(){
    if(m_revents & EPOLLRDHUP){//对方已关闭 好像读不到
        // printf("EPOLLRDHUP\n");
        m_closeCallback();
    }
    else if(m_revents & (EPOLLIN|EPOLLPRI)){ //缓冲区由数据可读
        // printf("EPOLLIN\n");
        m_readCallback();
    }
    else if(m_revents & EPOLLOUT){//写事件
        // printf("EPOLLOUT\n");
        m_writeCallback();
    }
    else{//其他事件视为错误
        m_errorCallback();
    }
}

//设置读事件回调函数
void Channel::setReadcallback(std::function<void()> fn){
    m_readCallback = fn;
}

//设置写事件回调函数
void Channel::setWritecallback(std::function<void()> fn){
    m_writeCallback = fn;
}

void Channel::setClosecallback(std::function<void()> fn){
    m_closeCallback = fn;
}

void Channel::setErrorcallback(std::function<void()> fn){
    m_errorCallback = fn;
}