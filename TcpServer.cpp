#include "TcpServer.h"

TcpServer::TcpServer(const std::string& ip, const uint16_t &port, int threadsnum)
            :m_threadsnum(threadsnum),
             m_mainloop(new EventLoop(true)),
             m_acceptor(m_mainloop.get(), ip, port),
             m_threadpool(m_threadsnum, "IO"){
    //创建主事件循环
    m_mainloop->setEpollwaitTimeoutCallback(std::bind(&TcpServer::handleEpollTimeout, this, std::placeholders::_1));
    m_acceptor.setnewConnCallback(std::bind(&TcpServer::handleNewConnection, this, std::placeholders::_1));

    //创建从事件循环
    for(int ii = 0;ii<m_threadsnum;++ii){
        m_subloops.emplace_back(new EventLoop(false));
        m_subloops[ii]->setEpollwaitTimeoutCallback(std::bind(&TcpServer::handleEpollTimeout, this, std::placeholders::_1));
        m_subloops[ii]->setHandleConnTimeout(std::bind(&TcpServer::removeConn, this, std::placeholders::_1));
        m_threadpool.addTask(std::bind(&EventLoop::runLoop, m_subloops[ii].get(), -1));
        // sleep(1);
    }

}

TcpServer::TcpServer():TcpServer("0.0.0.0", 8111){

}

TcpServer::~TcpServer(){

}



void TcpServer::start(int timeout){
    m_mainloop->runLoop(timeout);
}

void TcpServer::stop(){
    //停止主事件循环
    m_mainloop->stopLoop();
    //停止IO事件循环
    for(auto& loop:m_subloops){
        loop->stopLoop();
    }
    //停止IO线程
    m_threadpool.stop();
}

void TcpServer::handleNewConnection(std::unique_ptr<Socket> clientsock){
    std::shared_ptr<Connection> Conn(new Connection(m_subloops[clientsock->fd()%m_threadsnum].get(), std::move(clientsock)));
    Conn->setCloseCallback(std::bind(&TcpServer::handleCloseConnection, this, std::placeholders::_1));
    Conn->setErrorCallback(std::bind(&TcpServer::handleErrorConnection, this, std::placeholders::_1));
    Conn->setHandleMessageCallback(std::bind(&TcpServer::handleMessage, this, std::placeholders::_1, std::placeholders::_2));
    Conn->setSendCompleteCallback(std::bind(&TcpServer::handleSendComplete, this, std::placeholders::_1));
    //log new socket accept
    // printf("new socket accept: ip:%s port:%d\n", Conn->ip().c_str(), Conn->port());

    //Connection添加到TcpServer的map中
    {
        std::lock_guard<std::mutex> gd(m_mutex);
        m_conns[Conn->fd()] = Conn;
    }
    
    //Connection添加到对应事件循环的map中
    m_subloops[Conn->fd()%m_threadsnum]->newConnection(Conn);
    if(m_newconnectionCb) m_newconnectionCb(Conn);
}

//客户端关闭,在Connection类中回调该函数
void TcpServer::handleCloseConnection(std::shared_ptr<Connection> conn){
    if(m_closeconnectionCb) m_closeconnectionCb(conn);
    std::lock_guard<std::mutex> gd(m_mutex);
    m_conns.erase(conn->fd());
}
//客户端错误,在Connection类中回调该函数
void TcpServer::handleErrorConnection(std::shared_ptr<Connection> conn){
    if(m_errorconnectionCb) m_errorconnectionCb(conn);
    //log error
    printf("error closed: %d\n", conn->fd());
    std::lock_guard<std::mutex> gd(m_mutex);
    m_conns.erase(conn->fd());
}

void TcpServer::handleMessage(std::shared_ptr<Connection> conn, std::string& message){
    if(m_onmessageCb){
        // printf("message: %d\n", conn->fd());
        m_onmessageCb(conn, message);
    } 
}

void TcpServer::handleSendComplete(std::shared_ptr<Connection> conn){
    if(m_sendcompleteCb) m_sendcompleteCb(conn);
}

void TcpServer::handleEpollTimeout(EventLoop* evloop){
    if(m_timeoutCb)m_timeoutCb(evloop);
}

void TcpServer::removeConn(int fd){
    std::lock_guard<std::mutex> gd(m_mutex);
    m_conns.erase(fd);
}
