#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "Comm.hpp"

#include <sys/select.h>

int main()
{
    fd_set;

    // struct bitmap
    // {
    //     uint32_t bits[100]; // 101     101/32 101%32
    // };
    


    char buffer[1024];
    SetNonBlock(0);
    while(true)
    {
        // printf("Enter# ");
        // fflush(stdout);
        ssize_t n = ::read(0, buffer, sizeof(buffer)-1);
        if(n > 0)
        {
            buffer[n] = 0;
            printf("echo# %s", buffer);
        }
        else if(n == 0)  // ctrl + d
        {
            printf("read done\n");
            break;
        }
        else
        {
            // 如果是非阻塞，底层数据没有就绪，IO接口，会以出错形式返回
            // 所以，如何区分  底层不就绪  vs   真的出错了？ 根据errno错误码

            if(errno == EWOULDBLOCK)
            {
                sleep(1);
                std::cout << "底层数据没有就绪，开始轮询检测" << std::endl;
                std::cout << "可以做其他事情" << std::endl;
                // do other thing
                continue;
            }
            else if(errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("read");
                break;
            }
            // perror("read\n"); 
            // printf("n=%ld\n", n);
            // //底层数据没有就绪： errno 会被设置成为 EWOULDBLOCK EAGAIN
            // printf("errno=%d\n", errno); 
            // break;
        }
    }

    return 0;
}