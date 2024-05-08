#include "Acceptor.h"

Acceptor::Acceptor(EventLoop* loop, const std::string &ip, const uint16_t &port):m_loop(loop){
    m_servsock = new Socket(createListensocket());//创建监听socket;
    InetAddress servaddr(ip, port);
    m_servsock->setReuseaddr(true);
    m_servsock->setReuseport(true);
    m_servsock->setTcpnodelay(true);
    m_servsock->setKeepalive(true);
    m_servsock->bind(servaddr);
    m_servsock->listen();

    m_acceptChannel = new Channel(m_loop, m_servsock->fd());
    m_acceptChannel->enablereading();
    m_acceptChannel->setReadcallback(std::bind(&Channel::newConnection, m_acceptChannel, m_servsock));

}

Acceptor::~Acceptor()
{
    delete m_servsock;
    delete m_acceptChannel;
}