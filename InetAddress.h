#ifndef _INETADDRESS_HPP
#define _INETADDRESS_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
/**
 * @class:InetAddress
 * @brief:封装ip地址
*/
class InetAddress{
private:
    sockaddr_in m_addr;
public:
    InetAddress();   
    InetAddress(const std::string &ip, uint16_t port);
    InetAddress(const sockaddr_in addr):m_addr(addr){};//客户端地址构造
    ~InetAddress();

    const char* ip() const;//返回字符串IP地址
    uint16_t    port() const;//返回端口号
    const sockaddr* addr() const;//返回sockaddr结构
    void setaddr(sockaddr_in);
};

#endif