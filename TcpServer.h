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
    void newConnection(Socket* clientsock);
};

#endif