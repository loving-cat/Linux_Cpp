#include "namedPipe.hpp"

//server read : 管理命名管道的整个生命周期
int main()
{

    NamePiped fifo(comm_path,Creater);
    //对于读端而言，如果我们打开文件，但是写端还没来，我会阻塞在open调用中，直到对方打开
    //进程同步
    if(fifo.OpenForRead())
    {
        std::cout<<"server open named pipe done"<<std::endl;
        while(true)
        {
            std::string message;
            int n = fifo.ReadNamedPipe(&message);
            if(n>0)
            {
                std::cout<<"Client say"<<message<<std::endl;
            }
            else if(n == 0)//如果写端关闭，读端在读，就会读到0，代表读到文件结尾
            {
                std::cout<<"Client Quit,Server Too!"<<std::endl;
                break;
            }
            else
            {
                std::cout<<"fifo.ReadNamedPipe Error"<<std::endl;
                break;
            }
        }
        
    }
    //CreateNamedPipe(comm_path);
    //sleep(5);
    //RemoveNamePipe(comm_path);
    return 0;
}