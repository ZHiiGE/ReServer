#include "EchoServer.h"

EchoServer::EchoServer(const std::string &ip, const uint16_t &port,int loopthreadnums, int workthreadnums):m_threadpool(workthreadnums, "Works"), m_tcpserver(ip,port,loopthreadnums){
    m_tcpserver.setNewconnectionCb(std::bind(&EchoServer::handleNewConnection, this, std::placeholders::_1));
    m_tcpserver.setCloseconnectionCb(std::bind(&EchoServer::handleCloseConnection, this, std::placeholders::_1));
    m_tcpserver.setErrorconnectionCb(std::bind(&EchoServer::handleErrorConnection, this, std::placeholders::_1));
    m_tcpserver.setOnmessageCb(std::bind(&EchoServer::handleMessage, this, std::placeholders::_1, std::placeholders::_2));
    m_tcpserver.setSendcompleteCb(std::bind(&EchoServer::handleSendComplete, this, std::placeholders::_1));
    m_tcpserver.setTimeoutCb(std::bind(&EchoServer::handleEpollTimeout, this, std::placeholders::_1));

    // m_threadpool = new ThreadPool(workthreadnums);

}

EchoServer::~EchoServer(){

}


//启动事件循环
void EchoServer::start(int timeout){
    m_tcpserver.start(timeout);
}
//处理客户端新连接
void EchoServer::handleNewConnection(Connection* conn){
    // printf("NewConnection, thread is:%d\n", syscall(SYS_gettid));
    std::cout<<"new connection!"<<std::endl;

}
//客户端关闭,在Connection类中回调该函数
void EchoServer::handleCloseConnection(Connection* conn){
    std::cout<<"Conn Closed !"<<std::endl;
}
//客户端错误,在Connection类中回调该函数
void EchoServer::handleErrorConnection(Connection* conn){
    std::cout<<"Conn Error !"<<std::endl;
}
//Connection类处理数据的调用函数
void EchoServer::handleMessage(Connection* conn, std::string& message){
    // printf("message(eventfd=%d, ip=%s, port=%d):%s\n", conn->fd(), conn->ip().c_str(), conn->port() ,message.c_str());
    // printf("loop线程, thread is:%d\n", syscall(SYS_gettid));
     //把业务添加到任务队列
    m_threadpool.addTask(std::bind(&EchoServer::onMessage, this, conn, message));
   
}

//具体业务
void EchoServer::onMessage(Connection* conn, std::string& message){
    // printf("task线程, thread is:%d\n", syscall(SYS_gettid));
    message = "reply: " + message;
    conn->send(message.data(), message.size());
}

//发送完成的回调函数
void EchoServer::handleSendComplete(Connection* conn){
    std::cout<<"Send completed !"<<std::endl;
}
//epoll_wait超时回调函数
void EchoServer::handleEpollTimeout(EventLoop* evloop){
    std::cout<<"Epoll waitTimeout !"<<std::endl;
}