#ifndef _ECHOSERVER_H
#define _ECHOSERVER_H

#include "TcpServer.h"
#include "ThreadPool.h"
/**
 * @class:EchoServer
 * @brief:回显业务类
*/
class EchoServer{
private:
    TcpServer m_tcpserver;
    //工作业务线程池
    ThreadPool m_threadpool;

public:
    EchoServer(const std::string &ip, const uint16_t &port, int loopthreadnums=3, int workthreadnums=5);
    ~EchoServer();
    
    //启动事件循环
    void start(int timeout = -1);
    //处理客户端新连接
    void handleNewConnection(Connection* conn);
    //客户端关闭,在Connection类中回调该函数
    void handleCloseConnection(Connection* conn);
    //客户端错误,在Connection类中回调该函数
    void handleErrorConnection(Connection* conn);
    //Connection类处理数据的调用函数
    void handleMessage(Connection* conn, std::string& message);
    //发送完成的回调函数
    void handleSendComplete(Connection* conn);
    //epoll_wait超时回调函数
    void handleEpollTimeout(EventLoop* evloop);

    //具体业务, 在handleMessage中将其添加到线程池
    void onMessage(Connection* conn, std::string& message);
};
#endif