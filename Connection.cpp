#include "Connection.h"

Connection::Connection(EventLoop* loop, Socket* clientsock):m_loop(loop), m_clientsock(clientsock){

    m_clientChannel = new Channel(m_loop, clientsock->fd());
    m_clientChannel->setReadcallback(std::bind(&Channel::onMessage, m_clientChannel));
    m_clientChannel->setClosecallback(std::bind(&Connection::clossCallback, this));
    m_clientChannel->setErrorcallback(std::bind(&Connection::errorCallback, this));
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
void Connection::clossCallback(){
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