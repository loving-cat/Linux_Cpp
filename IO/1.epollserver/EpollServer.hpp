#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <sys/epoll.h>
#include "Log.hpp"
#include "Socket.hpp"

using namespace socket_ns;

class EpollServer
{
    const static int size = 128;
    const static int num = 128;

public:
    EpollServer(uint16_t port) : _port(port), _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildListenSocket(port);
        _epfd = ::epoll_create(size);
        if (_epfd < 0)
        {
            LOG(FATAL, "epoll_create error!\n");
            exit(1);
        }
        LOG(INFO, "epoll create success, epfd: %d\n", _epfd);
    }
    void InitServer()
    {
        // 新链接到来，我们认为是读事件就绪
        struct epoll_event ev;
        ev.events = EPOLLIN;
        // ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = _listensock->Sockfd(); // 为了在事件就绪的时候，得到是那一个fd就绪了
        // 必须先把listensock 添加到epoll中.
        int n = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock->Sockfd(), &ev);
        if (n < 0)
        {
            LOG(FATAL, "epoll_ctl error!\n");
            exit(2);
        }
        LOG(INFO, "epoll_ctl success, add new sockfd : %d\n", _listensock->Sockfd());
    }
    std::string EventsToString(uint32_t events)
    {
        std::string eventstr;
        if (events & EPOLLIN)
            eventstr = "EPOLLIN";
        if (events & EPOLLOUT)
            eventstr += "|EPOLLOUT";
        return eventstr;
    }
    void Accepter()
    {
        InetAddr addr;
        int sockfd = _listensock->Accepter(&addr); // 肯定不会被阻塞
        if (sockfd < 0)
        {
            LOG(ERROR, "获取连接失败\n");
            return;
        }
        LOG(INFO, "得到一个新的连接: %d, 客户端信息: %s:%d\n", sockfd, addr.Ip().c_str(), addr.Port());
        // 得到了一个新的sockfd，我们能不能要进行read、recv？不能.
        // 等底层有数据(读事件就绪), read/recv才不会被阻塞
        // 底层有数据 谁最清楚呢？epoll!
        // 将新的sockfd添加到epoll中！怎么做呢？
        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = EPOLLIN;
        ::epoll_ctl(_epfd, EPOLL_CTL_ADD, sockfd, &ev);
        LOG(INFO, "epoll_ctl success, add new sockfd : %d\n", sockfd);
    }
    void HandlerIO(int fd)
    {
        char buffer[4096];
        // 你怎么保证buffer就是一个完整的请求？或者有多个请求？？
        // 一个fd，都要有一个自己的缓冲区
        // 引入协议
        int n = ::recv(fd, buffer, sizeof(buffer) - 1, 0); // 会阻塞吗？不会
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << buffer;
            std::string response = "HTTP/1.0 200 OK\r\n";
            std::string content = "<html><body><h1>hello bite, hello world</h1></body></html>";
            response += "Content-Type: text/html\r\n";
            response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
            response += "\r\n";
            response += content;

            ::send(fd, response.c_str(), response.size(), 0);
        }
        else if (n == 0)
        {
            LOG(INFO, "client quit, close fd: %d\n", fd);
            // 1. 从epoll中移除, 从epoll中移除fd，这个fd必须是健康&&合法的fd. 否则会移除出错
            ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
            // 2. 关闭fd
            ::close(fd);
        }
        else
        {
            LOG(ERROR, "recv error, close fd: %d\n", fd);
            // 1. 从epoll中移除, 从epoll中移除fd，这个fd必须是健康&&合法的fd. 否则会移除出错
            ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
            // 2. 关闭fd
            ::close(fd);
        }
    }
    void HandlerEvent(int n)
    {
        for (int i = 0; i < n; i++)
        {
            int fd = revs[i].data.fd;
            uint32_t revents = revs[i].events;
            LOG(INFO, "%d 上面有事件就绪了,具体事件是: %s\n", fd, EventsToString(revents).c_str());
            if (revents & EPOLLIN)
            {
                // listensock 读事件就绪, 新连接到来了
                if (fd == _listensock->Sockfd())
                    Accepter();
                else
                    HandlerIO(fd);
            }
        }
    }
    void Loop()
    {
        int timeout = -1;
        while (true)
        {
            // 事件通知，事件派发
            int n = ::epoll_wait(_epfd, revs, num, timeout);
            switch (n)
            {
            case 0:
                LOG(INFO, "epoll time out...\n");
                break;
            case -1:
                LOG(ERROR, "epoll error\n");
                break;
            default:
                LOG(INFO, "haved event happend!, n : %d\n", n);
                // HandlerEvent(n);
                break;
            }
        }
    }
    ~EpollServer()
    {
        if (_epfd >= 0)
            ::close(_epfd);
        _listensock->Close();
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
    int _epfd;
    struct epoll_event revs[num];
};