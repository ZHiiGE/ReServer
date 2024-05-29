#include "EchoServer.h"
#include <signal.h>


EchoServer *server;

void Stop(int sig){
    std::cout<<"sig="<<sig<<std::endl;
    //调用Echoserver::stop()
    server->stop();
    std::cout<<"echoserver服务已停止"<<std::endl;
    delete server;
    exit(0);
}

int main(int argc, char *argv[]){
    if (argc != 3) 
    { 
        printf("usage: ./tcpepoll ip port\n"); 
        printf("example: ./tcpepoll 192.168.150.128 5085\n\n"); 
        return -1; 
    }

    signal(SIGTERM, Stop);
    signal(SIGINT, Stop);

    server = new EchoServer(argv[1], atoi(argv[2]), 3, 2, 1);

    server->start();

    return 0;
}