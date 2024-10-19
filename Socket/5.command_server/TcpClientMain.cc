#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ./tcpclient server-ip server-port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server-ip server-port" << std::endl;
        exit(0);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    // 1. 创建socket
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "create socket error" << std::endl;
        exit(1);
    }

    // 注意：不需要显示的bind，但是一定要有自己的IP和port，所以需要隐式的bind，OS会自动bind sockfd，用自己的IP和随机端口号
    // 什么时候进行自动bind？If the connection or binding succeeds
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    ::inet_pton(AF_INET, serverip.c_str(), &server.sin_addr);

    int n = ::connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    if (n < 0)
    {
        std::cerr << "connect socket error" << std::endl;
        exit(2);
    }

    while(true)
    {
        std::string message;
        std::cout << "Enter #";
        std::getline(std::cin, message);

        write(sockfd, message.c_str(), message.size());

        char echo_buffer[1024];
        n = read(sockfd, echo_buffer, sizeof(echo_buffer));
        if(n > 0)
        {
            echo_buffer[n] = 0;
            std::cout << echo_buffer << std::endl;
        }
        else
        {
            break;
        }
    }
    ::close(sockfd);
    return 0;
}