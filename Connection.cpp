#include "Connection.h"

Connection::Connection(EventLoop* loop, Socket* clientsock):m_loop(loop), m_clientsock(clientsock){

    m_clientChannel = new Channel(m_loop, clientsock->fd());
    m_clientChannel->setReadcallback(std::bind(&Channel::onMessage, m_clientChannel));
    m_clientChannel->useET();//边缘触发
    m_clientChannel->enablereading();//epoll_wait监听该channel
}

Connection::~Connection(){
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