#include "Shm.hpp"
#include"namedPipe.hpp"
int main()
{
    //引入管道级别的同步
    //1.创建共享内存
    Shm shm(gpathname,gproj_id,gCreater);
    char *shmaddr = (char*)shm.Addr();
    //2.创建管道
    NamePiped fifo(comm_path,Creater);
    fifo.OpenForRead();

    while(true)
    {
        std::string temp;
        fifo.ReadNamedPipe(&temp);
        std::cout <<"shm memory content: "<<shmaddr<<std::endl;
    }
    // key_t Key= GetCommKey(gpathname,gproj_id);
    // std::cout<<"key : "<< ToHex(Key) <<std::endl; 

    // int shmid = ShmGet(Key,4096);
    // std::cout<<"shmid : "<< shmid <<std::endl; 

    return 0;
}