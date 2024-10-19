#pragma once

#include <iostream>
#include <cstring>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <memory>


#include "InetAddr.hpp"
#include "Log.hpp"

namespace socket_ns
{
    // 模板方法类模式
    class Socket
    {
    public:
        // 创建
        virtual void CreateSocketOrDie() = 0;
        // 绑定
        virtual void CreateBindOrDie(uint16_t) = 0;
        // 监听
        virtual void CreateListenOrDie(int backlog) = 0;
        // 接收
        virtual SockSPtr Accepter(InetAddr *cliaddr) = 0;
        // 连接
        virtual bool Conntecor(const std::string &peerip,uint16_t peerport) = 0;

    public:
        void BuildListenSocket()
        {
            CreateSocketOrDie();
            CreateBindOrDie();
            CreateListenOrDie();
        }
    };

    enum
    {
        SOCKET_ERROR = 1,
        BIND_ERROR,
        LISTEN_ERROR
    };

    using namespace log_ns;
    const static int gblcklog = 8;
    
    //别名
    using SockSPtr = std::shared_ptr<Socket>;

    class TcpSocket : public Socket
    {
    public:
        TcpSocket()
        {}
        TcpSocket(int sockfd):_sockfd(sockfd)
        {}
        // 创建
        void CreateSocketOrDie() override
        {
            // 1.创建socket
            _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                LOG(FATAL, "socket create error\n");
                exit(SOCKET_ERROR);
            }
            LOG(INFO, "socket create success,sockfd:%d\n", _sockfd);
        }
        // 绑定
        void CreateBindOrDie(uint16_t port) override
        {
            struct sockaddr_in local;
            memset(&local,0,sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(port);
            local.sin_addr.s_addr=INADDR_ANY;

            if(::bind(_sockfd,(struct sockaddr *)&local,sizeof(local)) < 0)
            {
                LOG(FATAL,"bind error\n");
                exit(BIND_ERROR);
            }
            LOG(INFO,"bind success,sockfd:%d\n",_sockfd);
        }
        // 监听
        void CreateListenOrDie(int backlog = gblcklog) override
        {
            if(::listen(_sockfd,gblcklog) < 0)
            {
                LOG(FATAL,"listen error\n");
                exit(LISTEN_ERROR);
            }
            LOG(INFO,"listen success\n");
        }
        // 接收
        SockSPtr Accepter(InetAddr *cliaddr) override
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            //4.获取新连接
            int sockfd = ::accept(_sockfd,(struct sockaddr *)&client,&len);
            if(sockfd<0)
            {
                LOG(WARNING,"accept error\n");
                return nullptr;
            }
            *cliaddr = InetAddr(client);
            LOG(INFO,"get a new link,client info : %s,sockfd is: %d\n",cliaddr->AddrStr().c_str(),sockfd);

            return std::make_shared<TcpSocket>(sockfd);//C++14语法
        }
        // 连接
        bool Conntecor(const std::string &peerip,uint16_t peerport) override
        {
            struct sockaddr_in server;
            memset(&server,0,sizeof(server));//初始化server结构体的所有字段为0
            server.sin_family = AF_INET;//使用IPV4
            server.sin_port = htons(peerport);//将peerport 转换为网络字节序并赋值给sin_port
            //inet_pton 将字符串形式的IP地址转换成二进制格式
            ::inet_pton(AF_INET,peerip.c_str(),&server.sin_addr); // 将 peerip 转换为二进制格式并赋值给 sin_addr
            //connect()作用是让客户端套接字_sockfd连接到目标服务器，如果成功返回0，否则返回负数并且设置errno错误码
            //第一个参数，使用我们本地sockfd，第二个参数使用我们要连接的远程服务器信息
            int n = ::connect(_sockfd,(struct sockaddr *)&server,sizeof(server));
            if(n < 0)
            {
                std::cerr << "connect socket error" << std::endl;
                exit(2);
            }
        }

    private:
        int _sockfd; // 可以是listensock,普通socketfd都行
    };

    // Socket *sock = new TcpSocket();
    // sock->BuildListenSocket();
}