#pragma once

#include<iostream>
#include<string>
#include<cstdio>
#include<cerrno>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

const std::string comm_path="./myfifo";
#define DefaultFd -1
#define Creater 1
#define User    2
#define Read    O_RDONLY
#define Write   O_WRONLY
#define BaseSize 4096    //基本字节块大小

class NamePiped
{
private:
    //打开管道
    bool OpenNamedPipe(int mode)
    {
        _fd = open(_fifo_path.c_str(),mode);
        if(_fd < 0) 
            return false;
        return true;
    }
public:
    NamePiped(const std::string &path,int who)
    :_fifo_path(path),_id(who),_fd(DefaultFd)
    {
        if(_id == Creater)
        {
            int res = mkfifo(path.c_str(),0666);
            //创建管道失败
            if(res != 0)
            {
                perror("mkfifo");
            }
            std::cout<<"creater create named pipe"<<std::endl;
        }
    }
    bool OpenForRead()
    {
        return OpenNamedPipe(Read);
    }
    bool OpenForWrite()
    {
        return OpenNamedPipe(Write);
    }
    //纯输入参数 const & ： std::string &xxx
    //输出型参数* :         std::string *
    //输入输出型& :         std::string &
    int ReadNamedPipe(std::string *out)
    {
        char buffer[BaseSize];
        int n = read(_fd,buffer,sizeof(buffer));
        if(n > 0)
        {
            buffer[n] = 0;
            *out = buffer;
        }
        return n;
    }
    int WriteNamedPipe(const std::string &in)
    {
                //in的C风格的字符串
        return write(_fd,in.c_str(),in.size());
    }
    ~NamePiped()
    {
        if(_id==Creater)
        {
            //移除管道
            int res = unlink(_fifo_path.c_str());
            if(res != 0)
            {
                perror("unlink");
            }
            std::cout<<"creater create named pipe"<<std::endl;
        }
        if(_fd != DefaultFd)
            close(_fd);
    }

private:
    //管道对应的路径
    const std::string _fifo_path;
    int _id;
    int _fd;
};


// int CreateNamedPipe(const std::string &path)
// {
//     int res = mkfifo(path.c_str(),0666);
//     //创建管道失败
//     if(res != 0)
//     {
//         perror("mkfifo");
//     }
//     return res;
// }

// int RemoveNamePipe(const std::string &path)
// {
//     //移除管道
//     int res = unlink(path.c_str());
//     if(res != 0)
//     {
//         perror("unlink");
//     }
//     return res;
// }