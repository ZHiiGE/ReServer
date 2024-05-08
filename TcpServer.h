#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "EventLoop.h"
#include "Acceptor.h"

/**
 * @class:TcpServer
 * @brief:封装eventloop
*/

class TcpServer{
private:
    EventLoop m_loop;
    
    Acceptor* m_acceptor;
public:
    TcpServer(const std::string &ip, const uint16_t &port);
    ~TcpServer();

    void start();
};

#endif