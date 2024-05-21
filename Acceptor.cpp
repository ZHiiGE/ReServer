#include "Acceptor.h"

Acceptor::Acceptor(EventLoop* loop, const std::string &ip, const uint16_t &port)
                    :m_evloop(loop), 
                     m_servsock(createListensocket()),
                     m_acceptChannel(m_evloop, m_servsock.fd()){

    InetAddress servaddr(ip, port);
    m_servsock.setReuseaddr(true);
    m_servsock.setReuseport(true);
    m_servsock.setTcpnodelay(true);
    m_servsock.setKeepalive(true);
    m_servsock.bind(servaddr);
    m_servsock.listen();

    m_acceptChannel.setReadcallback(std::bind(&Acceptor::newConnection, this));
    m_acceptChannel.enablereading();


}

Acceptor::~Acceptor()
{
}

void Acceptor::newConnection(){
    InetAddress clientaddr;

    std::unique_ptr<Socket> clientsock(new Socket(m_servsock.accept(clientaddr)));
    clientsock->setIp(clientaddr.ip());
    clientsock->setPort(clientaddr.port());
    m_newConnCallback(std::move(clientsock));//调用TcpServer::newConnection

}

void Acceptor::setnewConnCallback(std::function<void(std::unique_ptr<Socket>)> fn){
    m_newConnCallback = fn;
}