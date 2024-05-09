#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "EventLoop.h"
#include "Acceptor.h"
#include "Connection.h"
#include <map>
/**
 * @class:TcpServer
 * @brief:封装eventloop
*/

class TcpServer{
private:
    EventLoop m_evloop;
    Acceptor* m_acceptor;
    std::map<int, Connection*> m_conns;
public:
    TcpServer(const std::string &ip, const uint16_t &port);
    virtual ~TcpServer();

    // //返回EventLoop
    // EventLoop evloop() const;
    // //返回Acceptor
    // Acceptor* acceptor() const;
    // //返回connections
    // std::map<int, Connection*> conns() const;
    
    //启动事件循环
    void start(int timeout = -1);
    //处理客户端新连接
    void handleNewConnection(Socket* clientsock);
    //客户端关闭,在Connection类中回调该函数
    void handleCloseConnection(Connection* conn);
    //客户端错误,在Connection类中回调该函数
    void handleErrorConnection(Connection* conn);
    //Connection类处理数据的调用函数
    virtual void handleMessage(Connection* conn, std::string message);
    //发送完成的回调函数
    void handleSendComplete(Connection* conn);
    //epoll_wait超时回调函数
    void handleEpollTimeout(EventLoop* evloop);
};

#endif