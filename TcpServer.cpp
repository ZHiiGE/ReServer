#include "TcpServer.h"

TcpServer::TcpServer(const std::string& ip, const uint16_t &port){
    m_acceptor = new Acceptor(&m_evloop, ip, port);
    m_acceptor->setnewConnCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
}

TcpServer::~TcpServer(){
    delete m_acceptor;
    for(auto &conn : m_conns){
        delete conn.second;
    }
}

void TcpServer::start(){
    m_evloop.runLoop();
}

void TcpServer::newConnection(Socket* clientsock){
    Connection* Conn = new Connection(&m_evloop, clientsock);
    Conn->setCloseCallback(std::bind(&TcpServer::closeConnection, this, Conn));
    Conn->setErrorCallback(std::bind(&TcpServer::errorConnection, this, Conn));
    Conn->setHandleMessageCallback(std::bind(&TcpServer::handleMessage, this, Conn, std::placeholders::_1));
    //log new socket accept
    printf("new socket accept: ip:%s port:%d\n", Conn->ip().c_str(), Conn->port());

    m_conns[Conn->fd()] = Conn;
}

//客户端关闭,在Connection类中回调该函数
void TcpServer::closeConnection(Connection* conn){
    //log close
    printf("client closed: %d\n", conn->fd());
    m_conns.erase(conn->fd());
    delete conn;
}
//客户端错误,在Connection类中回调该函数
void TcpServer::errorConnection(Connection* conn){
    //log error
    printf("error closed: %d\n", conn->fd());
    m_conns.erase(conn->fd());
    delete conn;
}

void TcpServer::handleMessage(Connection* conn, std::string message){
    printf("message(eventfd=%d, ip=%s, port=%d):%s\n", conn->fd(), conn->ip().c_str(), conn->port() ,message.c_str());
    message = "reply: " + message;
    int len = message.size();
    std::string tmp((char*)&len, 4);
    tmp.append(message);
    // send(conn->fd(), tmp.data(), tmp.size(), 0);
    conn->send(tmp.data(), tmp.size());

    // m_outputbuffer.clear();
    // m_outputbuffer = m_inputbuffer;
    // m_inputbuffer.clear();
    // send(fd(), m_outputbuffer.data(),m_outputbuffer.size(),0);
}