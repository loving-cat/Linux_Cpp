#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "nocopy.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"

using namespace log_ns;

static const int gsockfd = -1;
static const uint16_t glocalport = 8888;

enum
{
    SOCKET_ERROR = 1,
    BIND_ERROR
};

// UdpServer user("192.1.1.1", 8899);
// 一般服务器主要是用来进行网络数据读取和写入的。IO的
// 服务器IO逻辑 和 业务逻辑 解耦
class UdpServer : public nocopy
{
public:
    UdpServer(uint16_t localport = glocalport)
        : _sockfd(gsockfd),
          _localport(localport),
          _isrunning(false)
    {
    }
    void InitServer()
    {
        // 1. 创建socket文件
        _sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(FATAL, "socket error\n");
            exit(SOCKET_ERROR);
        }
        LOG(DEBUG, "socket create success, _sockfd: %d\n", _sockfd); // 3

        // 2. bind
        //地址信息
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        //表明我是一个IPV4的一个通信网络字节字
        local.sin_family = AF_INET;
        //服务器需要有自己的端口号//htons 主机序列转网络序列
        local.sin_port = htons(_localport);
        // local.sin_addr.s_addr = inet_addr(_localip.c_str()); // 1. 需要4字节IP 2. 需要网络序列的IP -- 暂时
        //需要一个IP地址
        //local.sin_addr.s_addr = inet_addr(_localip.c_str());
        //in_addr_t inet_addr(const char *cp)负责把一个字符串地址转换成int，4字节
        local.sin_addr.s_addr = INADDR_ANY; // 服务器端，进行任意IP地址绑定
        //                      local强转后就能拿到sin的数据
        int n = ::bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(FATAL, "bind error\n");
            exit(BIND_ERROR);
        }
        LOG(DEBUG, "socket bind success\n");
    }
    void Start()
    {
        _isrunning = true;
        char inbuffer[1024];
        while (_isrunning)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t n = recvfrom(_sockfd, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (n > 0)
            {
                InetAddr addr(peer);
                inbuffer[n] = 0;
                std::cout << "[" << addr.Ip() << ":" << addr.Port() << "]# " << inbuffer << std::endl;


                std::string echo_string = "[udp_server echo] #";
                echo_string += inbuffer;
                sendto(_sockfd, echo_string.c_str(), echo_string.size(), 0, (struct sockaddr *)&peer, len);
            }
            else
            {
                std::cout << "recvfrom ,  error"  << std::endl;
            }
        }
    }
    ~UdpServer()
    {
        if(_sockfd > gsockfd) ::close(_sockfd);
    }

private:
    int _sockfd;
    uint16_t _localport;
    // std::string _localip; // TODO:后面专门要处理一下这个IP
    bool _isrunning;
};