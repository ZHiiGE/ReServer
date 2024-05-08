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
        //new未释放,后面修改
    Connection* Conn = new Connection(&m_evloop, clientsock);
    Conn->setCloseCallback(std::bind(&TcpServer::closeConnection, this, Conn));
    Conn->setErrorCallback(std::bind(&TcpServer::errorConnection, this, Conn));
    //log accept
    printf("accept: ip:%s port:%d\n", Conn->ip().c_str(), Conn->port());

    m_conns[Conn->fd()] = Conn;
}

//客户端关闭,在Connection类中回调该函数
void TcpServer::closeConnection(Connection* conn){
    printf("client closed: %d\n", conn->fd());
    m_conns.erase(conn->fd());
    delete conn;
}
//客户端错误,在Connection类中回调该函数
void TcpServer::errorConnection(Connection* conn){
    printf("error closed: %d\n", conn->fd());
    m_conns.erase(conn->fd());
    delete conn;
}