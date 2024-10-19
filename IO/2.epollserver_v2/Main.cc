#include"TcpServer.hpp"
#include<memory>


int main(int arc,char* argv[])
{
    if(argc != 2)
    {
        std::cerr<<"Usage: "<<argv[0]<<"local-port"<<std::endl;
        exit(0);
    }
    uint16_t port = std::stoi(argv[1]);
    EnableScreen();
    Listener listener(port);
    return 0;
}