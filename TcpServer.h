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
    ~TcpServer();

    void start();
    void newConnection(Socket* clientsock);//处理客户端新连接
    void closeConnection(Connection* conn);//客户端关闭,在Connection类中回调该函数
    void errorConnection(Connection* conn);//客户端错误,在Connection类中回调该函数
};

#endif