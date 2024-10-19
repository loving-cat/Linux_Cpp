#pragma once
#include<iostream>
#include<sys/epoll.h>


static const int gsize = 128;
using namespace log_ns;


class Multiplex
{
public:
};

class Epoller : public Multiplex
{
public:
    Epoller()
    {
        _epfd = ::epoll_create(gsize);
        if(_epfd < 0)
        {
            LOG(FATAL,"epoll_create error\n");
            exit(EPOLL_CREATE_ERROR);
        }
    }
    ~Epoller()
    {}
};

class Poller : public Multiplex
{

};

class Selectot : public Multiplex
{

};