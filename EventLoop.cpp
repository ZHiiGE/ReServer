#include "EventLoop.h"

EventLoop::EventLoop()
            :m_ep(new Epoll),
             m_eventfd(eventfd(0, EFD_NONBLOCK)),
             m_wakeupchannel(new Channel(this, m_eventfd)){
    m_wakeupchannel->setReadcallback(std::bind(&EventLoop::handleWakeup, this));
    m_wakeupchannel->enablereading();
}

EventLoop::~EventLoop(){

}

void EventLoop::runLoop(int timeout){
    m_threadId = syscall(SYS_gettid);
    while (true)
    {
        std::vector<Channel*> chs = m_ep->loop(timeout);

        if(chs.size()==0){//epoll_wait timeout
            m_epollwaitTimeout(this);
        }else{
            for(auto& ch:chs){
                ch->handleEvent();
            }
        }
    }
    
}


void EventLoop::updataChannel(Channel* ch){
    m_ep->updataChannel(ch);
}
void EventLoop::setEpollwaitTimeoutCallback(std::function<void(EventLoop*)> fn){
    m_epollwaitTimeout = fn;
}
void EventLoop::removeChannel(Channel* ch){
    m_ep->removeChannel(ch);
}

void EventLoop::addTasktoqueue(std::function<void()> fn){
    {
        std::lock_guard<std::mutex> gd(m_mutex);
        m_taskqueue.push(fn);
    }
    //唤醒事件循环
    wakeUp();
}

void EventLoop::wakeUp(){
    uint64_t val = 1;
    write(m_eventfd, &val, sizeof(val));
}

void EventLoop::handleWakeup(){
    printf("wakeup thread is: %d\n", syscall(SYS_gettid));

    uint64_t val;
    read(m_eventfd, &val, sizeof(val));

    std::function<void()> fn;
    std::lock_guard<std::mutex> gd(m_mutex);
    while(!m_taskqueue.empty()){
        fn = std::move(m_taskqueue.front());
        m_taskqueue.pop();
        fn();
    }
}