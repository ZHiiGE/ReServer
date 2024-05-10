#include "TcpServer.h"

TcpServer::TcpServer(const std::string& ip, const uint16_t &port){
    m_evloop.setEpollwaitTimeoutCallback(std::bind(&TcpServer::handleEpollTimeout, this, std::placeholders::_1));
    m_acceptor = new Acceptor(&m_evloop, ip, port);
    m_acceptor->setnewConnCallback(std::bind(&TcpServer::handleNewConnection, this, std::placeholders::_1));
}

TcpServer::TcpServer():TcpServer("0.0.0.0", 8111){

}

TcpServer::~TcpServer(){
    delete m_acceptor;
    for(auto &conn : m_conns){
        delete conn.second;
    }
}

//     //返回EventLoop
// EventLoop TcpServer::evloop() const{
//     return m_evloop;
// }
// //返回Acceptor
// Acceptor* TcpServer::acceptor() const{
//     return m_acceptor;
// }
//     //返回connections
// std::map<int, Connection*> TcpServer::conns() const{
//     return m_conns;
// }

void TcpServer::start(int timeout){
    m_evloop.runLoop(timeout);
}

void TcpServer::handleNewConnection(Socket* clientsock){
    Connection* Conn = new Connection(&m_evloop, clientsock);
    Conn->setCloseCallback(std::bind(&TcpServer::handleCloseConnection, this, Conn));
    Conn->setErrorCallback(std::bind(&TcpServer::handleErrorConnection, this, Conn));
    Conn->setHandleMessageCallback(std::bind(&TcpServer::handleMessage, this, Conn, std::placeholders::_1));
    Conn->setSendCompleteCallback(std::bind(&TcpServer::handleSendComplete, this, Conn));
    //log new socket accept
    printf("new socket accept: ip:%s port:%d\n", Conn->ip().c_str(), Conn->port());

    m_conns[Conn->fd()] = Conn;
    if(m_newconnectionCb) m_newconnectionCb(Conn);
}

//客户端关闭,在Connection类中回调该函数
void TcpServer::handleCloseConnection(Connection* conn){
    if(m_closeconnectionCb) m_closeconnectionCb(conn);
    //log close
    printf("client closed: %d\n", conn->fd());
    m_conns.erase(conn->fd());
    delete conn;
}
//客户端错误,在Connection类中回调该函数
void TcpServer::handleErrorConnection(Connection* conn){
<<<<<<< HEAD
=======
    if(m_errorconnectionCb) m_errorconnectionCb(conn);
    printf("error closed: %d\n", conn->fd());
    delete conn;
}

void TcpServer::handleMessage(Connection* conn, std::string message){
    if(m_onmessageCb) m_onmessageCb(conn, message);
}

void TcpServer::handleSendComplete(Connection* conn){
    printf("send completed\n");
    if(m_sendcompleteCb) m_sendcompleteCb(conn);
}

void TcpServer::handleEpollTimeout(EventLoop* evloop){
    printf("eopll_wait timeout\n");
    if(m_timeoutCb)m_timeoutCb(evloop);
}