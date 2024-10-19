#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "Listener.hpp"
#include "InetAddr.hpp"


#define ListenConnection 0
#define NormalConnection 1

class Connection;
class Reactor;
using handler_t = std::function<void(Connection *conn)>;


// 未来我们的服务器，一切皆Connection，对我们来将listensockfd也是一样
class Connection
{
public:
    Connection(int sockfd) : _sockfd(sockfd)
    {
    }
    void RegisterHandler(handler_t recver, handler_t sender, handler_t excepter)
    {
        _handler_recver = recver;
        _handler_sender = sender;
        _handler_excepter = excepter;
    }
    void SetEvents(uint32_t events)
    {
        _events = events;
    }
    uint32_t Events()
    {
        return _events;
    }
    void SetConnectionType(int type)
    {
        _type = type;
    }
    int Type()
    {
        return _type;
    }
    int Sockfd()
    {
        return _sockfd;
    }
    void SetReactor(Reactor *R)
    {
        _R = R;
    }
    void SetAddr(const InetAddr &addr)
    {
        _addr = addr;
    }
    void AppendInbuffer(const std::string &in)
    {
        _inbuffer += in;
    }
    void AppendOutbuffer(const std::string &in)
    {
        _outbuffer += in;
    }
    std::string &Inbuffer()
    {
        return _inbuffer;
    }
    std::string &OutBuffer()
    {
        return _outbuffer;
    }
    void DiscardOutbuffer(int n)
    {
        _outbuffer.erase(0, n);
    }
    void Close()
    {
        if(_sockfd >= 0)
            ::close(_sockfd);
    }
    ~Connection()
    {
    }

private:
    int _sockfd;
    uint32_t _events;
    std::string _inbuffer; // 我们在这里，用string充当缓冲区
    std::string _outbuffer;
    int _type;

public:
    handler_t _handler_recver;   // 处理读取
    handler_t _handler_sender;   // 处理写入
    handler_t _handler_excepter; // 处理异常

    Reactor *_R; // 回指向自己所属的Reactor
    InetAddr _addr;
};