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

    //回调TcpServer::closeConnection()
    std::function<void()> m_closeCallback;
    //回调TcpServer::errorConnection()
    std::function<void()> m_errorCallback;
    //回调TcpServer::handleMessage()
    std::function<void(std::string)> m_handleMessageCallback;
    //回调TcpServer::sendComplete()
    std::function<void()> m_sendCompleteCallback;
public:
    Connection(EventLoop* loop, Socket* clientsock);
    ~Connection();

    int fd() const;
    std::string ip() const;
    uint16_t port() const;

    //读取接收缓冲区数据
    void onMessage(); 
    //供channel调用 关闭socket  
    void closeCallback();
    //供channel调用 错误 关闭socket
    void errorCallback();
    //写事件处理 供channel回调
    void writeCallback();
    //设置回调TcpServer::closeConnection()
    void setCloseCallback(std::function<void()> fn);
    //设置回调TcpServer::errorConnection()
    void setErrorCallback(std::function<void()> fn);
    //设置回调TcpServer::errorConnection()
    void setHandleMessageCallback(std::function<void(std::string)> fn);
    //设置回调TcpServer::sendComplete()
    void setSendCompleteCallback(std::function<void()> fn);
    //发送数据, 内部保证Tcp发送缓冲区不会溢出
    void send(const char* data, size_t size);
};



#endif