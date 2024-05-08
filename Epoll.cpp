#include "Epoll.h"
#include "Channel.h"

Epoll::Epoll(){
    if((m_epollfd = epoll_create(1)) == -1){
        //log error
        printf("epoll creat error\n");
        exit(-1);
    }
}

Epoll::~Epoll(){
    ::close(m_epollfd);
}


//添加Channel进epollfd
void Epoll::updataChannel(Channel* ch){
    epoll_event ev;//声明事件结构体
    ev.data.ptr = ch;
    ev.events = ch->events();

    if(ch->inEopll()){//ch已经在epoll红黑树中
        if(epoll_ctl(m_epollfd, EPOLL_CTL_MOD, ch->fd(), &ev) == -1){
            //log error
            printf("event mod error\n");
            exit(-1);
        }
    }
    else{
        if(epoll_ctl(m_epollfd, EPOLL_CTL_ADD, ch->fd(), &ev) == -1){
            //log error
            printf("event add error\n");
            exit(-1);
        }
        ch->setInepoll();
    }


}
//运行epoll_wait(),等待事件
std::vector<Channel*> Epoll::loop(int timeout){
    std::vector<Channel* > chs;

    bzero(m_events, sizeof(m_events));
    int infds = epoll_wait(m_epollfd, m_events, MaxEvents, timeout);

    if(infds < 0){
        //log error
        exit(-1);
    }

    if(infds == 0){
        //log timeour
        return chs;
    }

    for(int i=0;i<infds;++i){
        Channel* ch = (Channel*)m_events[i].data.ptr;
        ch->setRevents(m_events[i].events);//设置channel触发的事件revents为events
        chs.push_back(ch);//保存发生事件的channel
    }

    return chs;
}
