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