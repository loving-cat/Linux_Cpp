#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include"Connection.hpp"

//暂时叫做TcpServer->rename
class TcpServer
{
public:
    TcpServer()
    {}
    void AddConnection(int fd,uint32_t events)
    {
        //1.构建一个connection
        Connection *conn = new Connection(fd);
        conn->SetEvents(events);
        //TODO

        //2.托管给_connections
        _connections.insert(std::make_pair(fd,conn));

        //3.fd和events写透到内核中，托管给epoll
        //_epoller->addEvent
    }
    ~TcpServer()
    {}
private:
    //ket:sockfd
    //value:Connection
    std::unordered_map<int,Connection*> _connections;
};