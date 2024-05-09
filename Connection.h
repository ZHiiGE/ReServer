#ifndef _CONNECTION_H
#define _CONNECTION_H

#include "EventLoop.h"
#include "Buffer.h"
/**
 * @class:Connection
 * @brief:封装客户端连接进行消息传递的Channel类
*/

class Connection
{
private:
    EventLoop* m_loop;          //对应事件循环
    Socket* m_clientsock;       //对应连接Socket
    Channel* m_clientChannel;   //对应连接Channel
    Buffer m_inputbuffer;       //接收缓冲区
    Buffer m_outputbuffer;      //发送缓冲区

    std::function<void(Connection*)> m_closeCallback;//回调TcpServer::closeConnection()
    std::function<void(Connection*)> m_errorCallback;//回调TcpServer::errorConnection()
public:
    Connection(EventLoop* loop, Socket* clientsock);
    ~Connection();

    int fd() const;
    std::string ip() const;
    uint16_t port() const;

    void onMessage();   //读取接收缓冲区数据

    void closeCallback();//供channel调用
    void errorCallback();//供channel调用
    void setCloseCallback(std::function<void(Connection*)> fn);//回调TcpServer::closeConnection()
    void setErrorCallback(std::function<void(Connection*)> fn);//回调TcpServer::errorConnection()

};



#endif