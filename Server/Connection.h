#ifndef _CONNECTION_H
#define _CONNECTION_H

#include "EventLoop.h"
#include "Buffer.h"
#include <memory>
#include <atomic>
#include "Timestamp.h"
/**
 * @class:Connection
 * @brief:封装客户端连接进行消息传递的Channel类
*/

class Connection:public std::enable_shared_from_this<Connection>
{
private:
    EventLoop* m_loop;          //对应事件循环
    //对应连接Socket
    std::unique_ptr<Socket> m_clientsock;
    //对应连接Channel
    std::unique_ptr<Channel> m_clientChannel; 
    //报文分隔符
    const uint16_t m_sep;  
    Buffer m_inputbuffer;       //接收缓冲区
    Buffer m_outputbuffer;      //发送缓冲区
    std::atomic_bool m_disconnected; //连接是否已断开

    //回调TcpServer::closeConnection()
    std::function<void(std::shared_ptr<Connection>)> m_closeCallback;
    //回调TcpServer::errorConnection()
    std::function<void(std::shared_ptr<Connection>)> m_errorCallback;
    //回调TcpServer::handleMessage()
    std::function<void(std::shared_ptr<Connection>, std::string&)> m_handleMessageCallback;
    //回调TcpServer::sendComplete()
    std::function<void(std::shared_ptr<Connection>)> m_sendCompleteCallback;

    //时间戳类
    Timestamp m_lasttime;
public:
    Connection(EventLoop* loop, std::unique_ptr<Socket> clientsock, uint16_t m_sep);
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
    void setCloseCallback(std::function<void(std::shared_ptr<Connection>)> fn);
    //设置回调TcpServer::errorConnection()
    void setErrorCallback(std::function<void(std::shared_ptr<Connection>)> fn);
    //设置回调TcpServer::errorConnection()
    void setHandleMessageCallback(std::function<void(std::shared_ptr<Connection>, std::string&)> fn);
    //设置回调TcpServer::sendComplete()
    void setSendCompleteCallback(std::function<void(std::shared_ptr<Connection>)> fn);
    //发送数据, 内部保证Tcp发送缓冲区不会溢出
    void send(const char* data, size_t size);
    //在IO线程中
    void sendinLoop(const char* data, size_t size);
    //不在IO线程中
    void sendinLoop(const std::shared_ptr<char[]> data, size_t size);
    //判断是否长时间无消息
    bool timeOut(time_t now, int val=10);
};



#endif