#include "TcpServer.h"

TcpServer::TcpServer(const std::string& ip, const uint16_t &port){
    m_acceptor = new Acceptor(&m_evloop, ip, port);
    m_acceptor->setnewConnCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
}

TcpServer::~TcpServer(){
    delete m_acceptor;
}

void TcpServer::start(){
    m_evloop.runLoop();
}

void TcpServer::newConnection(Socket* clientsock){
        //new未释放,后面修改
    Connection* Conn = new Connection(&m_evloop, clientsock);
}