#include "Connection.h"

Connection::Connection(EventLoop* loop, Socket* clientsock):m_loop(loop), m_clientsock(clientsock){

    m_clientChannel = new Channel(m_loop, clientsock->fd());
    m_clientChannel->setReadcallback(std::bind(&Connection::onMessage, this));      //读事件回调对应connection的onMessage
    m_clientChannel->setWritecallback(std::bind(&Connection::writeCallback, this));      //读事件回调对应connection的onMessage
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
    m_closeCallback();
}
//供channel调用
void Connection::errorCallback(){
    m_errorCallback();
}
//设置回调函数CloseCallback为TcpServer::closeconnection
void Connection::setCloseCallback(std::function<void()> fn){
    m_closeCallback = fn;
}
//设置回调函数errorCallback为TcpServer::ererorconnection
void Connection::setErrorCallback(std::function<void()> fn){
    m_errorCallback = fn;
}
//设置回调函数handleMessageCallback为TcpServer::handleMessage
void Connection::setHandleMessageCallback(std::function<void(std::string)> fn){
    m_handleMessageCallback = fn;
}

void Connection::onMessage(){
    char buffer[1024];
    while(true){
        bzero(&buffer, sizeof(buffer));
        ssize_t nread = read(fd(), buffer, sizeof(buffer));
        if(nread >0 ){
            m_inputbuffer.append(buffer, nread);
        }
        else if(nread == -1 && errno == EINTR){//由信号中断 则继续读取
            continue;
        }
        else if(nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//读取完毕
            while(true){
                int len;//获取报文长度
                memcpy(&len, m_inputbuffer.data(), 4);
                if(m_inputbuffer.size()<len+4) break; //报文长度不足

                std::string message(m_inputbuffer.data()+4, len);//获取报文体
                m_inputbuffer.erase(0, len+4);//从缓冲区中删除以获取报文
                //数据处理
                m_handleMessageCallback(message);//回调TcpServer::handleMessage()
            }

            break;
        }
        else if(nread == 0){//客户端断开连接
            closeCallback();
            break;
        }
    }
}

void Connection::send(const char* data, size_t size){
    m_outputbuffer.append(data, size);
    m_clientChannel->enablewriting();
}

void Connection::writeCallback(){
    //尝试全部发送m_outputbuffer中的数据
    int write = ::send(fd(), m_outputbuffer.data(), m_outputbuffer.size(), 0);
    if(write>0){
        m_outputbuffer.erase(0, write);//删除已发送数据
    }
    if(m_outputbuffer.size() == 0){
        m_clientChannel->disablewriting();//发送完毕,取消监听写事件
    }
}