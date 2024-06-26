#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "Channel.h"
#include <memory>
/**
 * @class:Acceptor
 * @brief:对负责监听的Channel类进行封装
*/

class Acceptor
{
private:

    EventLoop* m_evloop;
    Socket m_servsock;
    Channel m_acceptChannel;

    std::function<void(std::unique_ptr<Socket>)> m_newConnCallback;
    
public:
    Acceptor(EventLoop* loop, const std::string &ip, const uint16_t &port);
    ~Acceptor();

    void newConnection();
    void setnewConnCallback(std::function<void(std::unique_ptr<Socket>)> fn);
};




#endif