#ifndef _SOCKET_H
#define _SOCKET_H

// #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <string>
// #include <errno.h>
#include <unistd.h>
#include "InetAddress.h"

//创建服务端监听socket
int createListensocket();

/**
 * @class:Socket
 * @brief:封装socket相关数据
*/

class Socket{
private:
    const int m_fd;//socket的fd,文件描述符
    std::string m_ip;//listensock存访监听的ip, 客户端sock存访客户端ip
    uint16_t m_port;//listensock存访监听的port, 客户端sock存访客户端port
public:
    Socket(int fd);
    ~Socket();

    int fd() const;
    std::string ip() const;
    uint16_t port() const;
    
    void setReuseaddr(bool);//设置地址重使用,取消time_wait
    void setReuseport(bool);//设置端口重使用
    void setTcpnodelay(bool);//禁用 Nagle 算法，即禁用 TCP 的延迟确认机制，数据发送时立即发送而不缓存
    void setKeepalive(bool);//tcp保活

    void bind(const InetAddress&);//绑定socket与ip:port
    void listen(int nums = 128);//开启监听客户端连接
    int accept(InetAddress&);//收到客户端连接
};

#endif