#include "SelectServer.hpp"
#include <memory>


// ./tcpserver 8888
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " local-port" << std::endl;
        exit(0);
    }
    uint16_t port = std::stoi(argv[1]);
    EnableScreen();
    std::unique_ptr<SelectServer> svr = std::make_unique<SelectServer>(port);
    svr->InitServer();
    svr->Loop();

    return 0;
}