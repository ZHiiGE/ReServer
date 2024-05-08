#include "Acceptor.h"

Acceptor::Acceptor(EventLoop* loop, const std::string &ip, const uint16_t &port):m_evloop(loop){
    m_servsock = new Socket(createListensocket());//创建监听socket;
    InetAddress servaddr(ip, port);
    m_servsock->setReuseaddr(true);
    m_servsock->setReuseport(true);
    m_servsock->setTcpnodelay(true);
    m_servsock->setKeepalive(true);
    m_servsock->bind(servaddr);
    m_servsock->listen();

    m_acceptChannel = new Channel(m_evloop, m_servsock->fd());
    m_acceptChannel->setReadcallback(std::bind(&Acceptor::newConnection, this));
    m_acceptChannel->enablereading();


}

Acceptor::~Acceptor()
{
    delete m_servsock;
    delete m_acceptChannel;
}

void Acceptor::newConnection(){
    InetAddress clientaddr;

    //传给对应Connection类,在Connection析构中释放
    Socket *clientsock = new Socket(m_servsock->accept(clientaddr));
    m_newConnCallback(clientsock);//调用TcpServer::newConnection

}

void Acceptor::setnewConnCallback(std::function<void(Socket*)> fn){
    m_newConnCallback = fn;
}