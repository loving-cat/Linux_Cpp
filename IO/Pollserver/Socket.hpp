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

#include "Log.hpp"
#include "InetAddr.hpp"

namespace socket_ns
{
    using namespace log_ns;
    class Socket;
    using SockSPtr = std::shared_ptr<Socket>;

    enum
    {
        SOCKET_ERROR = 1,
        BIND_ERROR,
        LISTEN_ERR
    };
    const static int gblcklog = 8;
    // 模版方法模式
    class Socket
    {
    public:
        virtual void CreateSocketOrDie() = 0;
        virtual void CreateBindOrDie(uint16_t port) = 0;
        virtual void CreateListenOrDie(int backlog = gblcklog) = 0;
        virtual int Accepter(InetAddr *cliaddr) = 0;
        virtual bool Conntecor(const std::string &peerip, uint16_t peerport) = 0;
        virtual int Sockfd() = 0;
        virtual void Close() = 0;
        virtual ssize_t Recv(std::string *out) = 0;
        virtual ssize_t Send(const std::string &in) = 0;

        virtual void ReUseAddr() = 0;

    public:
        void BuildListenSocket(uint16_t port)
        {
            CreateSocketOrDie();
            ReUseAddr();
            CreateBindOrDie(port);
            CreateListenOrDie();
        }
        bool BuildClientSocket(const std::string &peerip, uint16_t peerport)
        {
            CreateSocketOrDie();
            return Conntecor(peerip, peerport);
        }
        // void BuildUdpSocket()
        // {}
    };

    class TcpSocket : public Socket
    {
    public:
        TcpSocket()
        {
        }
        TcpSocket(int sockfd) : _sockfd(sockfd)
        {
        }
        ~TcpSocket()
        {
        }
        void CreateSocketOrDie() override
        {
            // 1. 创建socket
            _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                LOG(FATAL, "socket create error\n");
                exit(SOCKET_ERROR);
            }
            LOG(INFO, "socket create success, sockfd: %d\n", _sockfd); // 3
        }
        void CreateBindOrDie(uint16_t port) override
        {
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(port);
            local.sin_addr.s_addr = INADDR_ANY;

            // 2. bind sockfd 和 Socket addr
            if (::bind(_sockfd, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                LOG(FATAL, "bind error\n");
                exit(BIND_ERROR);
            }
            // LOG(INFO, "bind success, sockfd: %d\n", _sockfd); // 3
        }
        void CreateListenOrDie(int backlog) override
        {
            // 3. 因为tcp是面向连接的，tcp需要未来不断地能够做到获取连接
            if (::listen(_sockfd, gblcklog) < 0)
            {
                LOG(FATAL, "listen error\n");
                exit(LISTEN_ERR);
            }
            // LOG(INFO, "listen success\n");
        }
        int Accepter(InetAddr *cliaddr) override
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            // 4. 获取新连接
            int sockfd = ::accept(_sockfd, (struct sockaddr *)&client, &len);
            if (sockfd < 0)
            {
                LOG(WARNING, "accept error\n");
                return -1;
            }
            *cliaddr = InetAddr(client);
            LOG(INFO, "get a new link, client info : %s, sockfd is : %d\n", cliaddr->AddrStr().c_str(), sockfd);
            // return std::make_shared<TcpSocket>(sockfd); // C++14
            return sockfd;
        }
        bool Conntecor(const std::string &peerip, uint16_t peerport) override
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons(peerport);
            ::inet_pton(AF_INET, peerip.c_str(), &server.sin_addr);

            int n = ::connect(_sockfd, (struct sockaddr *)&server, sizeof(server));
            if (n < 0)
            {
                return false;
            }
            return true;
        }
        int Sockfd()
        {
            return _sockfd;
        }
        void Close()
        {
            if (_sockfd > 0)
            {
                ::close(_sockfd);
            }
        }
        ssize_t Recv(std::string *out) override
        {
            char inbuffer[4096];
            ssize_t n = ::recv(_sockfd, inbuffer, sizeof(inbuffer) - 1, 0);
            if (n > 0)
            {
                inbuffer[n] = 0;
                *out += inbuffer;
            }
            return n;
        }
        ssize_t Send(const std::string &in) override
        {
            return ::send(_sockfd, in.c_str(), in.size(), 0);
        }
        void ReUseAddr() override
        {
            int opt = 1;
            ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        }

    private:
        int _sockfd; // 可以是listensock，普通socketfd
    };
    // class UdpSocket : public Socket
    // {};
} // namespace socket_n
