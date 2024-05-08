#include "EventLoop.h"

EventLoop::EventLoop():m_ep(new Epoll()){

}

EventLoop::~EventLoop(){
    delete m_ep;
}

void EventLoop::runLoop(){
    while (true)
    {
        std::vector<Channel*> chs = m_ep->loop();
        for(auto& ch:chs){
            ch->handleEvent();
        }
    }
    
}


void EventLoop::updataChannel(Channel* ch){
    m_ep->updataChannel(ch);
}