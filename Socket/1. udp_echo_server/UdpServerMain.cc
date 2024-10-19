#include "UdpServer.hpp"

#include <memory>

// ./udp_server local-port
// ./udp_server 8888
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " local-port" << std::endl;
        exit(0);
    }
    uint16_t port = std::stoi(argv[1]);

    EnableScreen();  
    //                                      make_unique工厂函数
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port); //C++14的标准
    usvr->InitServer();
    usvr->Start();
    return 0;
}