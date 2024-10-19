#pragma once
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<cstring>
#include"InetAddr.hpp"
#include"Log.hpp"

using namespace log_ns;

const static int gport = 8888;
const static int gsock = -1;
//链接队列长度
const static int gblcklog = 8;
enum
{
    SOCKET_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR
};

class TcpServer
{
public:
    TcpServer(uint16_t port = gport):_port(port),_sockfd(gsock),_isrunning(false)
    {}
    void InitServer()
    {
        //创建socket
        int sockfd = ::socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
        {
            exit(SOCKET_ERROR);
            LOG(FATAL,"SOCKET CREATE ERROR\n");
        }
        LOG(INFO,"socket create success,sockfd:%d\n",sockfd);
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;

        //绑定sockfd和socket addr
        if(::bind(_sockfd,(struct sockaddr*)&local,sizeof(local))<0)
        {
            LOG(FATAL,"bind error\n");
            exit(BIND_ERROR);
        }
        //3.因为tcp是面向连接的，tcp需要未来不断地能够做到获取链接
        if(::listen(_sockfd,gblcklog)<0)
        {
            LOG(FATAL,"LISTEN ERROR\n");
            exit(LISTEN_ERROR);
        }
    }
    void Loop()
    {
        _isrunning = true;
        while(_isrunning)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            //获取新链接
            int sockfd = ::accept(_listensockfd,(struct sockaddr*)&client,&len);
            if(sockfd<0)
            {
                LOG(WARNING,"ACCEPT ERROR\n");
                continue;
            }
            InetAddr addr(client);
            LOG(INFO,"GET A NEW LINK,CLIENT INFO:%s\n",addr.AddrStr(),c.str());
            //version 0 ---不靠谱版本
            Service(sockfd,addr);

        }
        _isrunning = false;

    }
    void Service(int sockfd,InetAddr addr)
    {
        //长服务
        while(true)
        {
            char inbuffer[1024];//当做字符串
            ssize_t n = ::read(sockfd,inbuffer,sizeof(inbuffer)-1);
            if(n>0)
            {
                std::string echo_string = "[server echo]#";
                write(sockfd,echo_string.c_str(),echo_string.size());
            }
            else if(n == 0)
            {
                LOG(INFO,"client %s quit\n",addr.AddrStr().c_str());
                break;
            }
            else
            {
                LOG(ERROR,"READ ERROR: %s\n",addr.AddrStr().c_str());
                break;
            }
        }
        ::close(sockfd);
    }
    ~TcpServer()
    {}
private:
    uint16_t _port;//端口
    int _sockfd;
    int _listensockfd;
    bool _isrunning;
};