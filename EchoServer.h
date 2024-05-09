#ifndef _ECHOSERVER_H
#define _ECHOSERVER_H

#include "TcpServer.h"
/**
 * @class:EchoServer
 * @brief:回显业务类
*/
class EchoServer:public TcpServer{
public:
    EchoServer(const std::string &ip, const uint16_t &port);
    virtual ~EchoServer();
    virtual void handleMessage(Connection* conn, std::string message);
};
#endif