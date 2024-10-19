#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 客户端在未来一定要知道服务器的IP地址和端口号
// ./udp_client server-ip server-port
// ./udp_client 127.0.0.1 8888
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server-ip server-port" << std::endl;
        exit(0);
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    int sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        std::cerr << "create socket error" << std::endl;
        exit(1);
    }

    // client的端口号，一般不让用户自己设定，而是让client OS随机选择？怎么选择，什么时候选择呢？
    // client 需要 bind它自己的IP和端口， 但是client 不需要 “显示” bind它自己的IP和端口， 
    // client 在首次向服务器发送数据的时候，OS会自动给client bind它自己的IP和端口

    //服务器对应的套接字信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());
    while(1)
    {
        std::string line;
        std::cout << "Please Enter# ";
        std::getline(std::cin, line);

        // std::cout << "line message is@ " << line << std::endl;
        //sendto发送消息成功 返回值>0
        int n = sendto(sockfd, line.c_str(), line.size(), 0, (struct sockaddr*)&server, sizeof(server)); // 你要发送消息，你得知道你要发给谁啊！
        if(n > 0)
        {
            struct sockaddr_in temp;
            socklen_t len = sizeof(temp);
            char buffer[1024];
            //读的消息放到buffer里，读的方式为0
            int m = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&temp, &len);
            if(m > 0)
            {
                buffer[m] = 0;
                std::cout << buffer << std::endl;
            }
            else
            {
                std::cout << "recvfrom error" << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "sendto error" << std::endl;
            break;
        }
    }

    ::close(sockfd);
    return 0;
}