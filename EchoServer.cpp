#include "EchoServer.h"

EchoServer::EchoServer(const std::string &ip, const uint16_t &port):TcpServer(ip, port){

}

EchoServer::~EchoServer(){

}

void EchoServer::handleMessage(Connection* conn, std::string message){
    printf("message(eventfd=%d, ip=%s, port=%d):%s\n", conn->fd(), conn->ip().c_str(), conn->port() ,message.c_str());
    message = "reply: " + message;
    int len = message.size();
    std::string tmp((char*)&len, 4);
    tmp.append(message);
    conn->send(tmp.data(), tmp.size());
}