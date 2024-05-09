#include "Socket.h"
// #include "stdio.h"
//创建服务端监听socket
int createListensocket(){
    int listenfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, IPPROTO_TCP);
    if(listenfd<0){//创建失败,程序退出
        //log
        exit(-1);
    }
    return listenfd;
}

Socket::Socket(int fd):m_fd(fd){
    // printf("copy construct\n");
};

Socket::~Socket(){
    ::close(m_fd);
}

void Socket::setIp(const std::string ip){
    m_ip = ip;
}

void Socket::setPort(const uint16_t port){
    m_port = port;
}

void Socket::setTcpnodelay(bool on){
    int optval = on ? 1 : 0;
    ::setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)); // TCP_NODELAY包含头文件 <netinet/tcp.h>
}

void Socket::setReuseaddr(bool on){
    int optval = on ? 1 : 0;
    ::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); 
}

void Socket::setReuseport(bool on){
    int optval = on ? 1 : 0;
    ::setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); 
}

void Socket::setKeepalive(bool on){
    int optval = on ? 1 : 0;
    ::setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)); 
}

void Socket::bind(const InetAddress& servaddr){
    if(::bind(m_fd, servaddr.addr(), sizeof(sockaddr))<0){
        printf("bind error\n");
        ::close(m_fd);
        exit(-1);
    }
    m_ip = servaddr.ip();
    m_port = servaddr.port();
}

void Socket::listen(int nums){
    if(::listen(m_fd, nums) !=0 ){
        //log error
        printf("listen error\n");
        ::close(m_fd);
        exit(-1);
    }
}

int Socket::accept(InetAddress& clientaddr){
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    int clientfd = accept4(m_fd, (sockaddr*)&peeraddr, &len, SOCK_NONBLOCK);

    clientaddr.setaddr(peeraddr);
    // m_ip = 
    return clientfd;
}

int Socket::fd() const {
    return m_fd;
}

std::string Socket::ip() const{
    return m_ip;
}

uint16_t Socket::port() const{
    return m_port;
}