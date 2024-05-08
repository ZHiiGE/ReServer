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
    std::function<void(Connection*)> m_closeCallback;//回调TcpServer::closeConnection()
    std::function<void(Connection*)> m_errorCallback;//回调TcpServer::errorConnection()
public:
    Connection(EventLoop* loop, Socket* clientsock);
    ~Connection();

    int fd() const;
    std::string ip() const;
    uint16_t port() const;

    void clossCallback();//供channel调用
    void errorCallback();
    void setCloseCallback(std::function<void(Connection*)> fn);
    void setErrorCallback(std::function<void(Connection*)> fn);

};



#endif