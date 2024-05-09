#include "EventLoop.h"

EventLoop::EventLoop():m_ep(new Epoll()){

}

EventLoop::~EventLoop(){
    delete m_ep;
}

void EventLoop::runLoop(int timeout){
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