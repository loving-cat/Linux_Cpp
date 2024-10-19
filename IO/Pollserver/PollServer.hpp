#pragma once

#include <iostream>
#include <poll.h>
#include "Socket.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"

using namespace socket_ns;

class PollServer
{
    const static int gnum = sizeof(fd_set) * 8;
    const static int gdefaultfd = -1;

public:
    PollServer(uint16_t port) : _port(port), _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildListenSocket(_port);
    }
    void InitServer()
    {
        for (int i = 0; i < gnum; i++)
        {
            fd_events[i].fd = gdefaultfd;
            fd_events[i].events = 0;
            fd_events[i].revents = 0;

        }
        fd_events[0].fd = _listensock->Sockfd(); // 默认直接添加listensock到数组中
        fd_events[0].events = POLLIN;
    }
    // 处理新连接的
    void Accepter()
    {
        // 我们叫做连接事件就绪，等价于读事件就绪
        InetAddr addr;
        int sockfd = _listensock->Accepter(&addr); // 会不会被阻塞！一定不会！
        if (sockfd > 0)
        {
            LOG(DEBUG, "get a new link, client info %s:%d\n", addr.Ip().c_str(), addr.Port());
            // 已经获得了一个新的sockfd
            // 接下来我们可以读取吗？绝对不能读！读取的时候，条件不一定满足
            // 谁最清楚底层fd的数据是否就绪了呢？？通过select！
            // 想办法把新的fd添加给select，由select统一进行监管。怎么做到？？
            // select 为什么等待的fd会越来越多？？
            // 只要将新的fd，添加到fd_array中即可！
            bool flag = false;
            for (int pos = 1; pos < gnum; pos++)
            {
                if (fd_events[pos].fd == gdefaultfd)
                {
                    flag = true;
                    fd_events[pos].fd = sockfd;
                    fd_events[pos].events = POLLIN;
                    LOG(INFO, "add %d to fd_array success!\n", sockfd);
                    break;
                }
            }
            if (!flag)
            {
                LOG(WARNING, "Server Is Full!\n");
                ::close(sockfd);
                // 扩容
                // 添加
            }
        }
    }
    // 处理普通的fd就绪的
    void HandlerIO(int i)
    {
        // 下面的读写对吗？
        // 普通的文件描述符，正常的读写
        char buffer[1024];
        ssize_t n = ::recv(fd_events[i].fd, buffer, sizeof(buffer) - 1, 0); // 这里读取会阻塞吗？不会
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "client say# " << buffer << std::endl;
            std::string content = "<html><body><h1>hello bite</h1></body></html>";
            std::string echo_str = "HTTP/1.0 200 OK\r\n";
            echo_str += "Content-Type: text/html\r\n";
            echo_str += "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n";
            echo_str += content;
            // echo_str += buffer;
            ::send(fd_events[i].fd, echo_str.c_str(), echo_str.size(), 0); // 临时方案
        }
        else if (n == 0)
        {
            LOG(INFO, "client quit...\n");
            // 关闭fd
            ::close(fd_events[i].fd);
            // select 不要在关心这个fd了
            fd_events[i].fd = gdefaultfd;
            fd_events[i].events = 0;
            fd_events[i].revents = 0;
        }
        else
        {
            LOG(ERROR, "recv error\n");
            // 关闭fd
            ::close(fd_events[i].fd);
            // select 不要在关心这个fd了
            fd_events[i].fd = gdefaultfd;
            fd_events[i].events = 0;
            fd_events[i].revents = 0;
        }
    }
    // 一定会存在大量的fd就绪，可能是普通sockfd，也可能是listensockfd
    void HandlerEvent()
    {
        // 事件派发
        for (int i = 0; i < gnum; i++)
        {
            if (fd_events[i].fd == gdefaultfd)
                continue;
            // fd一定是合法的fd
            // 合法的fd不一定就绪, 判断fd是否就绪？
            if (fd_events[i].revents & POLLIN)
            {
                // 读事件就绪
                // 1. listensockfd 2. normal sockfd就绪？
                if (_listensock->Sockfd() == fd_events[i].fd)
                {
                    Accepter();
                }
                else
                {
                    HandlerIO(i);
                }
            }
        }
    }
    void Loop()
    {
        int timeout = -1;
        while (true)
        {
            // _listensock->Accepter();// 不能，listensock && accept 我们把他也看做IO类的函数。只关心新链接到来，等价于读事件就绪！
            int n = ::poll(fd_events, gnum, timeout); // 临时
            switch (n)
            {
            case 0:
                LOG(DEBUG, "time out\n");
                break;
            case -1:
                LOG(ERROR, "poll error\n");
                break;
            default:
                // LOG(DEBUG, "time out, %d.%d\n", timeout.tv_sec, timeout.tv_usec);
                LOG(INFO, "haved event ready, n : %d\n", n); // 如果事件就绪，但是不处理，select会一直通知我，直到我处理了！
                HandlerEvent();
                PrintDebug();
                // sleep(1);
                break;
            }
        }
    }

    void PrintDebug()
    {
        std::cout << "fd list: ";
        for (int i = 0; i < gnum; i++)
        {
            if (fd_events[i].fd == gdefaultfd)
                continue;
            std::cout << fd_events[i].fd << " ";
        }
        std::cout << "\n";
    }
    ~PollServer() {}

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;

    // 1. select要正常工作，需要借助一个辅助数组，来保存所有合法fd
    struct pollfd fd_events[gnum];
};