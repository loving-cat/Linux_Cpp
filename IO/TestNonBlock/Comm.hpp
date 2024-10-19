#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>

void SetNonBlock(int fd)
{
    int fl = ::fcntl(fd, F_GETFL);
    if(fl < 0)
    {
        std::cout << "fcntl error" << std::endl;
        return;
    }
    ::fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}