#include "Connection.h"

Connection::Connection(EventLoop* loop, Socket* clientsock):m_loop(loop), m_clientsock(clientsock){

    m_clientChannel = new Channel(m_loop, clientsock->fd());
    m_clientChannel->setReadcallback(std::bind(&Connection::onMessage, this));      //读事件回调对应connection的onMessage
    m_clientChannel->setClosecallback(std::bind(&Connection::closeCallback, this)); //关闭事件回调对应connection的clossCallback
    m_clientChannel->setErrorcallback(std::bind(&Connection::errorCallback, this)); //错误事件回调对应connection的errorCallback
    m_clientChannel->useET();//边缘触发
    m_clientChannel->enablereading();//epoll_wait监听该channel
}

Connection::~Connection(){
    delete m_clientsock;
    delete m_clientChannel;
}

int Connection::fd() const {
    return m_clientsock->fd();
}

std::string Connection::ip() const{
    return m_clientsock->ip();
}

uint16_t Connection::port() const{
    return m_clientsock->port();
}
//供channel调用
void Connection::closeCallback(){
    m_closeCallback(this);
}
//供channel调用
void Connection::errorCallback(){
    m_errorCallback(this);
}
//设置回调函数CloseCallback为TcpServer::closeconnection
void Connection::setCloseCallback(std::function<void(Connection*)> fn){
    m_closeCallback = fn;
}
//设置回调函数errorCallback为TcpServer::ererorconnection
void Connection::setErrorCallback(std::function<void(Connection*)> fn){
    m_errorCallback = fn;
}

void Connection::onMessage(){
    char buffer[1024];
    while(true){
        bzero(&buffer, sizeof(buffer));
        ssize_t nread = read(fd(), buffer, sizeof(buffer));
        if(nread >0 ){//success
            // printf("recv(eventfd=%d):%s\n", fd() ,buffer);
            // send(fd(), buffer, strlen(buffer), 0);
            m_inputbuffer.append(buffer, nread);
        }
        else if(nread == -1 && errno == EINTR){//由信号中断 则继续读取
            continue;
        }
        else if(nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//读取完毕
            printf("recv(eventfd=%d):%s\n", fd() ,m_inputbuffer.data());
            /*
            数据处理
            */
            m_outputbuffer.clear();
            m_outputbuffer = m_inputbuffer;
            m_inputbuffer.clear();
            send(fd(), m_outputbuffer.data(),m_outputbuffer.size(),0);
            break;
        }
        else if(nread == 0){//客户端断开连接
            closeCallback();
        }
    }
}