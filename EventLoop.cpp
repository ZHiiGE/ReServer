#include "EventLoop.h"
#include "Connection.h"

int createTimer(int sec=5){
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
    struct itimerspec timeout;
    memset(&timeout, 0, sizeof(struct itimerspec));
    timeout.it_value.tv_sec = sec;
    timeout.it_value.tv_nsec = 0;
    timerfd_settime(tfd, 0, &timeout, 0);
    return tfd;
}

EventLoop::EventLoop(bool mainloopflag, int timeval, int timeout)
            :m_stop(false),
             m_mainloopflag(mainloopflag),
             m_timeval(timeval),
             m_timeout(timeout),
             m_ep(new Epoll),
             m_eventfd(eventfd(0, EFD_NONBLOCK)),
             m_wakeupchannel(new Channel(this, m_eventfd)),
             m_timerfd(createTimer(m_timeval)),
             m_timerchannel(new Channel(this, m_timerfd)){
    m_wakeupchannel->setReadcallback(std::bind(&EventLoop::handleWakeup, this));
    m_wakeupchannel->enablereading();

    m_timerchannel->setReadcallback(std::bind(&EventLoop::handleTimer, this));
    m_timerchannel->enablereading();
}

EventLoop::~EventLoop(){

}

void EventLoop::runLoop(int timeout){
    m_threadId = syscall(SYS_gettid);
    while (!m_stop)
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

void EventLoop::stopLoop(){
    m_stop = true;
    //唤醒epoll
    wakeUp();
}

void EventLoop::updataChannel(Channel *ch)
{
    m_ep->updataChannel(ch);
}
void EventLoop::setEpollwaitTimeoutCallback(std::function<void(EventLoop*)> fn){
    m_epollwaitTimeout = fn;
}
void EventLoop::removeChannel(Channel* ch){
    m_ep->removeChannel(ch);
}

void EventLoop::setHandleConnTimeout(std::function<void(int)> fn){
    m_ConnTimeout = fn;
}

void EventLoop::addTasktoqueue(std::function<void()> fn)
{
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
    printf("wakeup thread is: %ld\n", syscall(SYS_gettid));

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

void EventLoop::handleTimer(){
    // std::cout<<"handleTimer"<<std::endl;
    struct itimerspec timeout;
    memset(&timeout, 0, sizeof(struct itimerspec));
    timeout.it_value.tv_sec = m_timeval;
    timeout.it_value.tv_nsec = 0;
    timerfd_settime(m_timerfd, 0, &timeout, 0);
    if(m_mainloopflag){
        
    }
    else{

        time_t now = time(0);//获取当前时间

        for(auto it=m_conns.begin();it!=m_conns.end();){
            if((it->second->timeOut(now, m_timeout)) == true){
                int fd = it->first;
                {
                    std::lock_guard<std::mutex> gd(m_connmutex);
                    m_conns.erase(it++);
                }
                
                m_ConnTimeout(fd);
            }else{
                it++;
            }

        }
        // printf("\n");
    }
}

void EventLoop::newConnection(std::shared_ptr<Connection> conn){
    std::lock_guard<std::mutex> gd(m_connmutex);
    m_conns[conn->fd()] = conn;
}
