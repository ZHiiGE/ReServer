#ifndef _ECHOSERVER_H
#define _ECHOSERVER_H

#include "TcpServer.h"
/**
 * @class:EchoServer
 * @brief:回显业务类
*/
class EchoServer{
private:
    TcpServer m_tcpserver;
public:
    EchoServer(const std::string &ip, const uint16_t &port);
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
    void handleMessage(Connection* conn, std::string message);
    //发送完成的回调函数
    void handleSendComplete(Connection* conn);
    //epoll_wait超时回调函数
    void handleEpollTimeout(EventLoop* evloop);
};
#endif