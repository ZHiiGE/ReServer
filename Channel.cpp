#include "Channel.h"
#include "Connection.h"
Channel::Channel(EventLoop* evloop, int fd):m_evloop(evloop), m_fd(fd){
    
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

//让事件循环eventloop中添加该channel连接
void Channel::enablereading(){
    m_events |= EPOLLIN;
    m_evloop->updataChannel(this);
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
    if(m_revents & EPOLLRDHUP){//对方已关闭
        //log
        ::close(m_fd);
    }
    else if(m_revents & (EPOLLIN|EPOLLPRI)){ //缓冲区由数据可读
        m_readCallback();
    }
    else if(m_revents & EPOLLOUT){//写事件

    }
    else{//其他事件视为错误
        //log undefine event
        ::close(m_fd);
    }
}

void Channel::newConnection(Socket* servsock){
    InetAddress clientaddr;

    //传给对应Connection类,在Connection析构中释放
    Socket *clientsock = new Socket(servsock->accept(clientaddr));

    //log accept
    printf("accept:%s:%d\n", clientaddr.ip(), clientaddr.port());

    //new未释放,后面修改
    Connection* Conn = new Connection(m_evloop, clientsock);

    
}

void Channel::onMessage(){
    char buffer[1024];
    while(true){
        bzero(&buffer, sizeof(buffer));
        ssize_t nread = read(m_fd, buffer, sizeof(buffer));
        if(nread >0 ){//success
            printf("recv(eventfd=%d):%s\n", m_fd ,buffer);
            send(m_fd, buffer, strlen(buffer), 0);
        }
        else if(nread == -1 && errno == EINTR){//由信号中断 则继续读取
            continue;
        }
        else if(nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//读取完毕
            break;
        }
        else if(nread == 0){//客户端断开连接
            //log disconn
            printf("client closed\n");
            ::close(m_fd);
            break;
        }
    }
}
//设置读事件回调函数
void Channel::setReadcallback(std::function<void()> fn){
    m_readCallback = fn;
}