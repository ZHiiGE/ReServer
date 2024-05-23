#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "EventLoop.h"
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include <map>
#include <vector>
/**
 * @class:TcpServer
 * @brief:封装eventloop
*/

class TcpServer{
private:
    //主事件循环
    std::unique_ptr<EventLoop> m_mainloop;
    //从事件循环
    std::vector<std::unique_ptr<EventLoop>> m_subloops;

    //线程数量,即从时间循环个数
    int m_threadsnum;
    //线程池
    ThreadPool m_threadpool;

    //报文分隔符
    const uint16_t m_sep;


    Acceptor m_acceptor;
    //m_conns的互斥锁
    std::mutex m_mutex;
    std::map<int, std::shared_ptr<Connection>> m_conns;

    //回调具体业务类的newconnect
    std::function<void(std::shared_ptr<Connection>)> m_newconnectionCb;
    //回调具体业务类的closeconnection
    std::function<void(std::shared_ptr<Connection>)> m_closeconnectionCb;
    //回调具体业务类的errorconnect
    std::function<void(std::shared_ptr<Connection>)> m_errorconnectionCb;
    //回调具体业务类的handleconnect
    std::function<void(std::shared_ptr<Connection>, std::string&)> m_onmessageCb;
    //回调具体业务类的sendcomplete
    std::function<void(std::shared_ptr<Connection>)> m_sendcompleteCb;
    //回调具体业务类的timeout
    std::function<void(EventLoop*)> m_timeoutCb;
public:
    TcpServer(uint16_t m_sep = 1);
    TcpServer(const std::string &ip, const uint16_t &port, int threadsnum=3, uint16_t m_sep=1);
    ~TcpServer();

    // //返回EventLoop
    // EventLoop evloop() const;
    // //返回Acceptor
    // Acceptor* acceptor() const;
    // //返回connections
    // std::map<int, std::shared_ptr<Connection>> conns() const;
    
    //启动事件循环
    void start(int timeout = -1);
    //停止IO事件循环
    void stop();
    //处理客户端新连接
    void handleNewConnection(std::unique_ptr<Socket> clientsock);
    //客户端关闭,在Connection类中回调该函数
    void handleCloseConnection(std::shared_ptr<Connection> conn);
    //客户端错误,在Connection类中回调该函数
    void handleErrorConnection(std::shared_ptr<Connection> conn);
    //Connection类处理数据的调用函数
    void handleMessage(std::shared_ptr<Connection> conn, std::string& message);
    //发送完成的回调函数
    void handleSendComplete(std::shared_ptr<Connection> conn);
    //epoll_wait超时回调函数
    void handleEpollTimeout(EventLoop* evloop);

    //设置回调函数对象
    void setNewconnectionCb(std::function<void(std::shared_ptr<Connection>)> fn){
        m_newconnectionCb = fn;
    };
    void setCloseconnectionCb(std::function<void(std::shared_ptr<Connection>)> fn){
        m_closeconnectionCb = fn;
    };
    void setErrorconnectionCb(std::function<void(std::shared_ptr<Connection>)> fn){
        m_errorconnectionCb = fn;
    };
    void setOnmessageCb(std::function<void(std::shared_ptr<Connection>, std::string&)> fn){
        m_onmessageCb = fn;
    };
    void setSendcompleteCb(std::function<void(std::shared_ptr<Connection>)> fn){
        m_sendcompleteCb = fn;
    };
    void setTimeoutCb(std::function<void(EventLoop*)> fn){
        m_timeoutCb = fn;
    };
    //删除map中的conn
    void removeConn(int fd);

};

#endif