#include "TcpServer.h"

TcpServer::TcpServer(const std::string& ip, const uint16_t &port){
    m_acceptor = new Acceptor(&m_loop, ip, port);   
}

TcpServer::~TcpServer(){
    delete m_acceptor;
}

void TcpServer::start(){
    m_loop.runLoop();
}