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

    //回调具体业务类的newconnect
    std::function<void(Connection*)> m_newconnectionCb;
    //回调具体业务类的closeconnection
    std::function<void(Connection*)> m_closeconnectionCb;
    //回调具体业务类的errorconnect
    std::function<void(Connection*)> m_errorconnectionCb;
    //回调具体业务类的handleconnect
    std::function<void(Connection*, std::string)> m_onmessageCb;
    //回调具体业务类的sendcomplete
    std::function<void(Connection*)> m_sendcompleteCb;
    //回调具体业务类的timeout
    std::function<void(EventLoop*)> m_timeoutCb;
public:
    TcpServer();
    TcpServer(const std::string &ip, const uint16_t &port);
    ~TcpServer();

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
    void handleMessage(Connection* conn, std::string message);
    //发送完成的回调函数
    void handleSendComplete(Connection* conn);
    //epoll_wait超时回调函数
    void handleEpollTimeout(EventLoop* evloop);

    //设置回调函数对象
    void setNewconnectionCb(std::function<void(Connection*)> fn){
        m_newconnectionCb = fn;
    };
    void setCloseconnectionCb(std::function<void(Connection*)> fn){
        m_closeconnectionCb = fn;
    };
    void setErrorconnectionCb(std::function<void(Connection*)> fn){
        m_errorconnectionCb = fn;
    };
    void setOnmessageCb(std::function<void(Connection*, std::string)> fn){
        m_onmessageCb = fn;
    };
    void setSendcompleteCb(std::function<void(Connection*)> fn){
        m_sendcompleteCb = fn;
    };
    void setTimeoutCb(std::function<void(EventLoop*)> fn){
        m_timeoutCb = fn;
    };

};

#endif