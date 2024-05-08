#ifndef _CONNECTION_H
#define _CONNECTION_H

#include "EventLoop.h"
/**
 * @class:Connection
 * @brief:封装客户端连接进行消息传递的Channel类
*/

class Connection
{
private:
    EventLoop* m_loop;
    Socket* m_clientsock;
    Channel* m_clientChannel;
public:
    Connection(EventLoop* loop, Socket* clientsock);
    ~Connection();

    int fd() const;
    std::string ip() const;
    uint16_t port() const;
};



#endif