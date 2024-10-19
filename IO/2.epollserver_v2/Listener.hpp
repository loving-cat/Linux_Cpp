#pragma once
#include<iostream>
#include<memory>
#include"Socket.hpp"

//该类未来统一进行listensock的管理工作，获取新链接
using namespace socket_ns;

class Listener
{
public:
    Listener(uint16_t port):_port(port),_listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildListenSocket(_port);
    }
    int ListenSockfd()
    {
        return _listensock->Sockfd();
    }


    ~Listener()
    {}

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
};