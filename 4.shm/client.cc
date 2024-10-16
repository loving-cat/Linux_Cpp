#include"Shm.hpp"
#include"namedPipe.hpp"
int main()
{
    //1.创建共享内存
    Shm shm(gpathname,gproj_id,gUser);
    shm.Zero();
    char *shmaddr = (char*)shm.Addr();
    //sleep(5);
    //2.打开管道
    NamePiped fifo(comm_path,User);
    fifo.OpenForWrite();

    //当成string
    char ch = 'A';
    while(ch<='Z')
    {
        shmaddr[ch-'A'] = ch;
        ch++;
        std::string temp = "wakeup";
        fifo.WriteNamedPipe(temp);
       // sleep(1);
    }
    return 0;
}