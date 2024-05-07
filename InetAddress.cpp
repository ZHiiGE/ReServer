#include "InetAddress.h"
//本地监听地址构造
InetAddress::InetAddress(const std::string& ip, uint16_t port){
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    m_addr.sin_port = htons(port);

}
//客户端地址构造
InetAddress::InetAddress(const sockaddr_in addr):m_addr(addr)
{

}
//析构
InetAddress::~InetAddress()
{

}

//返回字符串ip
const char* InetAddress::ip() const{
    return inet_ntoa(m_addr.sin_addr);
}

//返回端口号
uint16_t InetAddress::port() const{
    return ntohs(m_addr.sin_port);
}

//返回sockaddr结构地址
const sockaddr* InetAddress::addr() const{
    return (sockaddr*)&m_addr;
}